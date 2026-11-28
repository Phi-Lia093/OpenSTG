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
#include <vector>

#include "core/GameErrorContext.hpp"
#include "data/AssetStore.hpp"
#include "script/AnmVm.hpp"

namespace AnmScript
{
namespace
{
// The Lua state for the animation being run, shared by every VM using it.
//
// One state per animation is enough: the streams are data, and a VM's position lives in its own
// coroutine. The state is rebuilt when a different animation is started, which happens between
// scenes rather than between frames.
struct State
{
    std::string name;
    lua_State *lua = nullptr;
    int animationRef = LUA_NOREF;  // the table returned by animation.lua
};

State g_state;

// What a VM holds while it runs.
struct Running
{
    lua_State *thread = nullptr;
    int threadRef = LUA_NOREF;
    std::string animation;
    // Set by at() when the requested time has not arrived, and read by Run to decide whether
    // the VM should advance or stay put.
    bool waiting = false;
    i32 waitUntil = 0;
};

// One entry per VM, since AnmVm is memset by its constructor and cannot hold this itself.
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

// The script object passed as `s` to a stream function. Its methods set the current time and
// forward each instruction to the VM the coroutine belongs to.
struct ScriptObject
{
    AnmVm *vm;
    AnmVm *Get() const {
        return vm;
    }
};

AnmVm *AsVm(lua_State *L)
{
    auto *self = (ScriptObject *)luaL_checkudata(L, 1, "anm.script");
    return self->vm;
}

// at(t): wait until the VM's clock reaches t.
//
// This is where the original's `while (instr->time <= currentTime)` decision lives now. If the
// time has come, nothing happens and the function keeps running, which matches the original
// running several instructions in one frame. If it has not, the coroutine yields and Run will
// be called again next frame.
int Script_at(lua_State *L)
{
    auto *self = (ScriptObject *)luaL_checkudata(L, 1, "anm.script");
    const i32 requested = (i32)luaL_checkinteger(L, 2);
    const i32 now = self->vm->currentTimeInScript.GetCurrent();

    if (requested > now)
    {
        lua_pushinteger(L, requested);
        return lua_yield(L, 1);
    }
    lua_settop(L, 1);
    return 1;
}

// Every instruction method: applies the opcode to the VM.
//
// The methods are generated from one table so that adding an opcode is one line rather than a
// new C function. Each takes the script object first, then the instruction's arguments.
int Script_Instruction(lua_State *L)
{
    auto *self = (ScriptObject *)luaL_checkudata(L, 1, "anm.script");
    const char *name = luaL_checkstring(L, lua_upvalueindex(1));
    (void)name;
    (void)self;
    return 0;
}

const luaL_Reg kScriptMethods[] = {
    {"at", Script_at},
    {NULL, NULL},
};

void PushScriptObject(lua_State *L, AnmVm *vm)
{
    auto *self = (ScriptObject *)lua_newuserdata(L, sizeof(ScriptObject));
    self->vm = vm;
    luaL_getmetatable(L, "anm.script");
    lua_setmetatable(L, -2);
}

AnmVm *g_startingVm = nullptr;
} // namespace

bool Exists(const char *anmName)
{
    std::string text;
    return ReadAsset(PathFor(anmName), text);
}
} // namespace AnmScript
