#include "script/AnmScript.hpp"

extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <string>

#include <SDL3/SDL_stdinc.h>

#include "core/GameErrorContext.hpp"
#include "data/AssetStore.hpp"
#include "script/AnmOp.hpp"
#include "script/AnmVm.hpp"
#include "script/AnmVmDebug.hpp"

namespace AnmScript
{
namespace
{
// Every opcode, by the name its Lua method carries. Generated from AnmVm.hpp by
// tools/gen_lua_methods.py; tools/check_method_table.py checks the numbers against the
// enumeration, which a hand-written table got wrong for the whole arithmetic group.
const struct OpcodeName
{
    const char *name;
    i32 opcode;
} kNames[] = {
    {"NOP", 0}, {"EXIT_HIDE", -1}, {"EXIT_HIDE2", 1}, {"EXIT", 2}, {"SET_ACTIVE_SPRITE", 3},
    {"JUMP", 4}, {"DEC_JUMP", 5}, {"SET_TRANSLATION", 6}, {"SET_SCALE", 7}, {"SET_ALPHA", 8},
    {"SET_COLOR", 9}, {"FLIP_X", 10}, {"FLIP_Y", 11}, {"SET_ROTATION", 12},
    {"SET_ANGLE_VEL", 13}, {"SET_SCALE_SPEED", 14}, {"FADE", 15}, {"SET_BLEND", 16},
    {"POS_TIME_LINEAR", 17}, {"POS_TIME_DECEL", 18}, {"POS_TIME_ACCEL", 19}, {"STOP", 20},
    {"NOP22", 22}, {"STOP_HIDE", 23}, {"SET_USE_OFFSET", 24}, {"SET_AUTO_ROTATE", 25},
    {"SET_SCROLL_POS_X", 26}, {"SET_SCROLL_POS_Y", 27}, {"SET_VISIBILITY", 28},
    {"INTERP_SCALE", 29}, {"SET_ZWRITE_DISABLE", 30}, {"SET_CAMERA_MODE", 31},
    {"INTERP_POS", 32}, {"INTERP_COLOR", 33}, {"INTERP_ALPHA", 34}, {"INTERP_ROTATE", 35},
    {"INTERP_SCALE_2", 36}, {"MOV", 37}, {"MOV_FLOAT", 38}, {"ADD", 39}, {"ADD_FLOAT", 40},
    {"SUB", 41}, {"SUB_FLOAT", 42}, {"MUL", 43}, {"MUL_FLOAT", 44}, {"DIV", 45},
    {"DIV_FLOAT", 46}, {"MOD", 47}, {"MOD_FLOAT", 48}, {"ADD_2", 49}, {"ADD_FLOAT_2", 50},
    {"SUB_2", 51}, {"SUB_FLOAT_2", 52}, {"MUL_2", 53}, {"MUL_FLOAT_2", 54}, {"DIV_2", 55},
    {"DIV_FLOAT_2", 56}, {"MOD_2", 57}, {"MOD_FLOAT_2", 58}, {"RAND", 59},
    {"RAND_FLOAT", 60}, {"SIN", 61}, {"COS", 62}, {"TAN", 63}, {"ACOS", 64}, {"ATAN", 65},
    {"NORMALIZE_ANGLE", 66}, {"JUMP_IF_EQ", 67}, {"JUMP_IF_EQ_FLOAT", 68},
    {"JUMP_IF_NEQ", 69}, {"JUMP_IF_NEQ_FLOAT", 70}, {"JUMP_IF_LT", 71},
    {"JUMP_IF_LT_FLOAT", 72}, {"JUMP_IF_LEQ", 73}, {"JUMP_IF_LEQ_FLOAT", 74},
    {"JUMP_IF_GT", 75}, {"JUMP_IF_GT_FLOAT", 76}, {"JUMP_IF_GEQ", 77},
    {"JUMP_IF_GEQ_FLOAT", 78}, {"WAIT", 79}, {"SET_SCROLLVEL_X", 80}, {"SET_SCROLLVEL_Y", 81},
};

// The Lua state for the animation being run, shared by every VM using it.
struct State
{
    std::string name;
    lua_State *lua = nullptr;
    int animationRef = LUA_NOREF;
};

State g_state;

// What a VM holds while it runs. Kept outside the VM because AnmVm memsets itself and cannot
// carry a std::string.
struct Running
{
    lua_State *thread = nullptr;
    int threadRef = LUA_NOREF;
    // Whether the coroutine has been started: the first resume hands it `s`, later ones continue
    // from inside at() and pass nothing.
    bool started = false;
    // What the stream was built from, so an interrupt can build a new one at a label.
    std::string animation;
    i32 entryIndex = 0;
    i32 scriptIndex = 0;
    i32 entry = 0;
};

std::map<const AnmVm *, std::unique_ptr<Running>> g_running;

std::string PathFor(const char *anmName)
{
    return std::string("anm/") + anmName + "/animation.lua";
}

bool ReadAsset(const std::string &path, std::string &out)
{
    u32 size = 0;
    u8 *data = AssetStore::Read(path.c_str(), &size);
    if (!data)
    {
        return false;
    }
    out.assign((const char *)data, size);
    free(data);
    return true;
}

void CloseState()
{
    if (g_state.lua)
    {
        lua_close(g_state.lua);
        g_state.lua = nullptr;
        g_state.animationRef = LUA_NOREF;
    }
    g_state.name.clear();
}

bool OpenState(const char *anmName)
{
    if (g_state.lua && g_state.name == anmName)
    {
        return true;
    }
    CloseState();

    std::string text;
    if (!ReadAsset(PathFor(anmName), text))
    {
        return false;
    }

    lua_State *lua = luaL_newstate();
    if (!lua)
    {
        return false;
    }
    luaL_openlibs(lua);

    if (luaL_loadbuffer(lua, text.c_str(), text.size(), PathFor(anmName).c_str()) != LUA_OK ||
        lua_pcall(lua, 0, 1, 0) != LUA_OK)
    {
        g_GameErrorContext.Log("アニメ %s: %s\n", anmName, lua_tostring(lua, -1));
        lua_close(lua);
        return false;
    }
    if (!lua_istable(lua, -1))
    {
        g_GameErrorContext.Log("アニメ %s: 記述がテーブルではありません\n", anmName);
        lua_close(lua);
        return false;
    }

    g_state.name = anmName;
    g_state.lua = lua;
    g_state.animationRef = luaL_ref(lua, LUA_REGISTRYINDEX);
    return true;
}

AnmVm *VmOf(lua_State *L)
{
    // `s` is the table BuildCoroutine makes: its `vm` field is the VM the stream acts on.
    if (!lua_istable(L, 1))
    {
        return nullptr;
    }
    lua_getfield(L, 1, "vm");
    AnmVm *vm = (AnmVm *)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return vm;
}

// at(t): wait until the VM's clock reaches t.
//
// The interpreter's `while (instr->time <= currentTime)` decision, moved into the coroutine. If
// the time has come it returns at once and the stream runs on -- the interpreter running several
// instructions in one frame. If it has not, it yields and Run resumes next frame.
int Lua_at(lua_State *L)
{
    AnmVm *vm = VmOf(L);
    const i32 requested = (i32)luaL_checkinteger(L, 2);
    if (!vm)
    {
        return 0;
    }
    if (requested > vm->currentTimeInScript.GetCurrent())
    {
        lua_pushinteger(L, requested);
        return lua_yield(L, 1);
    }
    return 0;
}

// entry(): where this run of the stream begins.
//
// Every generated stream reads this and jumps to the matching label, so a stream built in answer
// to an interrupt resumes at that label as the interpreter's scan did. A run not started by an
// interrupt gets 0 and falls through to the top.
int Lua_entry(lua_State *L)
{
    i32 entry = 0;
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "startAt");
        entry = (i32)lua_tointeger(L, -1);
        lua_pop(L, 1);
    }
    lua_pushinteger(L, entry);
    return 1;
}

// LABEL(n): an interrupt label.
//
// The Lua label beside it is where the entry dispatch jumps to; this call is what running the
// stream reports, so a run of the stream yields the same instruction sequence the binary holds.
// It does not wait -- the interpreter fell through a label too, and an interrupt did the work of
// moving the program.
int Lua_Label(lua_State *L)
{
    return 0;
}

// An instruction method: every instruction reaches AnmOp::Run, the same code the interpreter's
// loop called, so the two cannot drift apart.
int Lua_Instruction(lua_State *L)
{
    AnmVm *vm = VmOf(L);
    const char *name = lua_tostring(L, lua_upvalueindex(1));
    const i32 opcode = (i32)lua_tointeger(L, lua_upvalueindex(2));
    if (!vm)
    {
        return 0;
    }

    // The call has the arguments as ordinary values; AnmOp reads the argument array the binary
    // form carried. AnmRawInstr ends in a flexible array so it cannot be a local; the widest
    // instruction the format defines is five arguments.
    struct
    {
        i16 opcode;
        u16 size;
        i16 time;
        u16 flags;
        AnyArg args[5];
    } instruction;

    memset(&instruction, 0, sizeof(instruction));
    instruction.opcode = (i16)opcode;
    instruction.time = (i16)vm->currentTimeInScript.GetCurrent();

    const int count = lua_gettop(L) - 1;
    const int capacity = (int)(sizeof(instruction.args) / sizeof(instruction.args[0]));
    for (int index = 0; index < count && index < capacity; index++)
    {
        const int type = lua_type(L, index + 2);
        if (type == LUA_TNUMBER)
        {
            // Lua distinguishes an integer from a float, and the distinction matters here: the
            // instruction's argument is a 32-bit slot that the opcode reads either as an int or
            // as a float, and the two have different bit patterns. Storing -48 as an integer
            // where SET_TRANSLATION reads a float gives a NaN, which is what a position became
            // when every number was stored as an integer.
            if (lua_isinteger(L, index + 2))
            {
                instruction.args[index].i = (i32)lua_tointeger(L, index + 2);
            }
            else
            {
                instruction.args[index].f = (f32)lua_tonumber(L, index + 2);
            }
        }
        else if (type == LUA_TTABLE)
        {
            // A register reference: `{r = N}` says the argument names a register rather than
            // carrying a value, and the flags bit is what tells AnmOp to read it as one.
            lua_getfield(L, index + 2, "r");
            instruction.args[index].i = (i32)lua_tointeger(L, -1);
            instruction.flags |= (u16)(1 << index);
            lua_pop(L, 1);
        }
        else
        {
            g_GameErrorContext.Log("アニメ: %s の引数 %d が数値ではありません\n", name, index);
        }
    }
    instruction.size = (u16)(8 + 4 * (count < capacity ? count : capacity));

    const OpResult result = AnmOp::Run(vm, (const AnmRawInstr *)&instruction);

    // An instruction can say the frame is over. STOP parks a script where it is, and STOP_HIDE
    // both hides and parks; the interpreter stepped the clock back and left the loop, so the same
    // instruction ran again next frame. In Lua the coroutine yields here and Run stops resuming
    // it while the VM is stopped, which is the same thing without going backwards -- resuming it
    // would carry on past STOP, and for most streams that means running into their exit.
    if (result == OpResult::EndFrame)
    {
        lua_pushinteger(L, -1);
        return lua_yield(L, 1);
    }

    if (result == OpResult::NotHandled)
    {
        AnmVmDebug::Note("unhandled opcode %d (%s)", (int)opcode, name);
    }
    return 0;
}

void set_method(lua_State *L, int table, const char *name, i32 opcode)
{
    lua_pushstring(L, name);
    lua_pushinteger(L, opcode);
    lua_pushcclosure(L, Lua_Instruction, 2);
    lua_setfield(L, table, name);
}

void RegisterMethods(lua_State *L, int table)
{
    lua_pushcfunction(L, Lua_at);
    lua_setfield(L, table, "at");
    lua_pushcfunction(L, Lua_entry);
    lua_setfield(L, table, "entry");
    lua_pushcfunction(L, Lua_Label);
    lua_setfield(L, table, "LABEL");

    for (const auto &entry : kNames)
    {
        set_method(L, table, entry.name, entry.opcode);
    }
}

// Builds the coroutine for a stream, entered at `entry`. An existing coroutine is dropped, which
// is what an interrupt does: the interpreter jumped to a label, and this starts a new run of the
// same stream at the label's entry point.
bool BuildCoroutine(AnmVm *vm, Running *running, i32 entry)
{
    lua_State *L = g_state.lua;
    if (!L)
    {
        return false;
    }

    if (running->threadRef != LUA_NOREF)
    {
        luaL_unref(L, LUA_REGISTRYINDEX, running->threadRef);
        running->threadRef = LUA_NOREF;
    }
    running->thread = nullptr;
    running->started = false;

    lua_rawgeti(L, LUA_REGISTRYINDEX, g_state.animationRef);
    if (!lua_istable(L, -1))
    {
        lua_pop(L, 1);
        return false;
    }
    lua_geti(L, -1, running->entryIndex);
    if (!lua_istable(L, -1))
    {
        lua_pop(L, 2);
        return false;
    }
    lua_geti(L, -1, running->scriptIndex);
    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 3);
        return false;
    }

    lua_State *thread = lua_newthread(L);
    lua_pushvalue(L, -2);           // the stream function
    lua_xmove(L, thread, 1);

    // The object the stream receives as `s`. `vm` is what the instruction methods act on;
    // `startAt` is what the dispatch at the top of every stream reads through s:entry().
    lua_newtable(thread);
    lua_pushlightuserdata(thread, vm);
    lua_setfield(thread, -2, "vm");
    lua_pushinteger(thread, entry);
    lua_setfield(thread, -2, "startAt");
    RegisterMethods(thread, lua_gettop(thread));

    running->thread = thread;
    // luaL_ref takes the thread off the stack; what is left is the function, the entry table and
    // the animation table, so three are popped. Popping four would take something that is not
    // there and leave the main state's stack wrong for everything after it.
    running->threadRef = luaL_ref(L, LUA_REGISTRYINDEX);
    running->entry = entry;

    lua_pop(L, 3);                  // function, entry table, animation table
    return true;
}
} // namespace

bool Exists(const char *anmName)
{
    std::string text;
    return ReadAsset(PathFor(anmName), text);
}

bool ShouldUseLua(i32 slot)
{
    // TH07_ANM_LUA_SLOT names one slot, TH07_ANM_LUA_FIRST and _LAST name a range, so more than
    // one stream can be compared at a time while every other slot keeps the binary path.
    static const i32 first = [] {
        const char *value = SDL_getenv("TH07_ANM_LUA_FIRST");
        if (!value)
        {
            value = SDL_getenv("TH07_ANM_LUA_SLOT");
        }
        return value ? (i32)atoi(value) : -1;
    }();

    static const i32 last = [] {
        const char *value = SDL_getenv("TH07_ANM_LUA_LAST");
        if (!value)
        {
            value = SDL_getenv("TH07_ANM_LUA_SLOT");
        }
        return value ? (i32)atoi(value) : -1;
    }();

    return first >= 0 && slot >= first && slot <= last;
}

bool Start(AnmVm *vm, const char *anmName, i32 entryIndex, i32 scriptIndex)
{
    AnmVmDebug::Note("start: called for %s", anmName);
    if (!OpenState(anmName))
    {
        AnmVmDebug::Note("start: cannot open %s", anmName);
        return false;
    }
    AnmVmDebug::Note("start: opened %s", anmName);

    auto running = std::make_unique<Running>();
    running->animation = anmName;
    running->entryIndex = entryIndex;
    running->scriptIndex = scriptIndex;

    if (!BuildCoroutine(vm, running.get(), 0))
    {
        AnmVmDebug::Note("start: cannot build %s entry %d stream %d", anmName, (int)entryIndex,
                         (int)scriptIndex);
        return false;
    }

    Release(vm);
    g_running[vm] = std::move(running);
    AnmVmDebug::Note("start: built %s entry %d stream %d", anmName, (int)entryIndex,
                     (int)scriptIndex);
    return true;
}

bool HasCoroutine(const AnmVm *vm)
{
    return g_running.find(vm) != g_running.end();
}

i32 Run(AnmVm *vm, i32 time)
{
    (void)time;

    auto it = g_running.find(vm);
    if (it == g_running.end())
    {
        return 1;
    }
    Running *running = it->second.get();

    // A stopped script stays where it is until an interrupt names a label. The interpreter ran it
    // again every frame only to compare a time it had already passed; here not resuming is the
    // same thing, and it keeps the coroutine from running past the STOP it is sitting on.
    if (vm->pendingInterrupt == 0 && vm->isStopped != 0)
    {
        return 0;
    }

    // An interrupt resumes the stream at a label rather than where it stopped.
    if (vm->pendingInterrupt != 0)
    {
        const i32 wanted = (i32)vm->pendingInterrupt;
        vm->pendingInterrupt = 0;

        // Two attempts, and the second is the fallback the interpreter used: a label numbered -1
        // accepted an interrupt no other label matched. BuildCoroutine leaves the main state's
        // stack as it found it, so trying twice in a row is safe.
        bool entered = BuildCoroutine(vm, running, wanted);
        if (!entered)
        {
            entered = BuildCoroutine(vm, running, -1);
        }

        if (!entered)
        {
            AnmVmDebug::Note("interrupt %d: slot %d has no such label", (int)wanted,
                             (int)vm->anmFileIdx);
            return 0;
        }
        vm->isStopped = 0;
        vm->visible = 1;
        AnmVmDebug::Note("interrupt %d resumes slot %d at a label", (int)wanted,
                         (int)vm->anmFileIdx);
    }
if (!running->thread)
    {
        return 1;
    }

    // The interpreter ran as many instructions as the clock allowed and stopped at the first one
    // whose time had not come. Resuming once per call would run one instruction per frame.
    for (int step = 0; step < 256; step++)
    {
        const bool first = !running->started;
        int results = 0;
        const int status = lua_resume(running->thread, nullptr, first ? 1 : 0, &results);
        running->started = true;

        if (status == LUA_OK)
        {
            AnmVmDebug::Note("slot %d: the stream ran to its end", (int)vm->anmFileIdx);
            Release(vm);
            return 1;
        }
        if (status != LUA_YIELD)
        {
            g_GameErrorContext.Log("アニメの実行に失敗: %s\n",
                                   lua_tostring(running->thread, -1));
            AnmVmDebug::Note("lua error in slot %d: %s", (int)vm->anmFileIdx,
                             lua_tostring(running->thread, -1));
            Release(vm);
            return 1;
        }

        // A yield carries the time at(t) is waiting for, or -1 for "this frame is over".
        i32 requested = -1;
        if (lua_gettop(running->thread) > 0 && lua_isinteger(running->thread, -1))
        {
            requested = (i32)lua_tointeger(running->thread, -1);
        }
        lua_pop(running->thread, results > 0 ? results : 1);

        if (requested < 0 || requested > vm->currentTimeInScript.GetCurrent())
        {
            return 0;
        }
    }

    AnmVmDebug::Note("slot %d ran 256 instructions without waiting", (int)vm->anmFileIdx);
    Release(vm);
    return 1;
}

void Release(AnmVm *vm)
{
    auto it = g_running.find(vm);
    if (it == g_running.end())
    {
        return;
    }
    if (g_state.lua && it->second->threadRef != LUA_NOREF)
    {
        luaL_unref(g_state.lua, LUA_REGISTRYINDEX, it->second->threadRef);
    }
    g_running.erase(it);
}

void Shutdown()
{
    g_running.clear();
    CloseState();
}

} // namespace AnmScript
