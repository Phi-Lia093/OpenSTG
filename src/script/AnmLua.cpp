#include "script/AnmLua.hpp"

extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SDL3/SDL_log.h>

#include "core/GameErrorContext.hpp"
#include "core/Supervisor.hpp"
#include "data/AssetStore.hpp"
#include "script/AnmManager.hpp"

namespace AnmLua
{
namespace
{
// --- opcode table -------------------------------------------------------------------
//
// One entry per opcode the interpreter handles, describing how it reads its arguments.
// The kinds come from the interpreter itself, not from memory: a float opcode reads
// args[n].f, an integer one args[n].i, and a few read a single byte out of a word. Getting
// this wrong produces a stream that loads and animates incorrectly, so the table is checked
// against the interpreter in the conversion tests.
enum class ArgKind
{
    None,
    Int,
    Float,
    Byte,
};

struct OpcodeInfo
{
    i16 opcode;
    ArgKind args[5];
    int argCount;
};

// The instructions title01 uses, which is the set this first conversion has to support.
// Adding a file means adding the opcodes it needs; the shape of each is taken from
// tools/extract_opcodes.py, which reads the interpreter's switch.
const OpcodeInfo kOpcodes[] = {
    {ANM_EXIT_HIDE, {ArgKind::None}, 0},
    {ANM_EXIT_HIDE2, {ArgKind::None}, 0},
    {ANM_EXIT, {ArgKind::None}, 0},
    {ANM_SET_ACTIVE_SPRITE, {ArgKind::Int}, 1},
    {ANM_SET_ALPHA, {ArgKind::Int}, 1},
    {ANM_SET_TRANSLATION, {ArgKind::Float, ArgKind::Float, ArgKind::Float}, 3},
    {ANM_JUMP, {ArgKind::Int, ArgKind::Int}, 2},
    {ANM_DEC_JUMP, {ArgKind::Int, ArgKind::Int, ArgKind::Int}, 3},
    {ANM_FADE, {ArgKind::Byte, ArgKind::Int}, 2},
    {ANM_POS_TIME_LINEAR, {ArgKind::Float, ArgKind::Float, ArgKind::Float, ArgKind::Int}, 4},
    {ANM_POS_TIME_DECEL, {ArgKind::Float, ArgKind::Float, ArgKind::Float, ArgKind::Int}, 4},
    {ANM_POS_TIME_ACCEL, {ArgKind::Float, ArgKind::Float, ArgKind::Float, ArgKind::Int}, 4},
    // STOP takes no argument: it just marks the script as stopped unless an interrupt is
    // pending. Extracting the signatures from the interpreter misreported this one, because
    // the scan ran past the case into its neighbour's body.
    {ANM_STOP, {ArgKind::None}, 0},
    {ANM_STOP_HIDE, {ArgKind::None}, 0},
    {ANM_INTERRUPT_LABEL, {ArgKind::Int}, 1},
    {ANM_22, {ArgKind::None}, 0},
    {ANM_SET_ZWRITE_DISABLE, {ArgKind::Int}, 1},
    {ANM_INTERP_POS, {ArgKind::Int, ArgKind::Byte, ArgKind::Float, ArgKind::Float,
                      ArgKind::Float}, 5},
    {ANM_INTERP_ALPHA, {ArgKind::Int, ArgKind::Byte, ArgKind::Byte}, 3},
};

const OpcodeInfo *FindOpcode(i16 opcode)
{
    for (const OpcodeInfo &info : kOpcodes)
    {
        if (info.opcode == opcode)
        {
            return &info;
        }
    }
    return nullptr;
}

// --- reading data -------------------------------------------------------------------

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

std::string EntryPath(const char *anmName, u32 entryIndex, const char *file)
{
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "anm/%s/%02u/%s", anmName, (unsigned)entryIndex, file);
    return buffer;
}

// --- a very small TOML reader -------------------------------------------------------
//
// The uvs.toml written by the converter uses one table header and repeated [[sprite]]
// arrays, so this reads exactly that. It is not a general TOML reader and does not pretend
// to be; the configuration reader in data/Toml.cpp is the general one, and this stays out of
// its way because the shapes differ (arrays of tables rather than flat keys).

struct UvFile
{
    i32 version = 2;
    i32 width = 0;
    i32 height = 0;
    std::string texture;
    std::vector<std::vector<i32>> sprites;
};

bool ParseUvs(const std::string &text, UvFile &out, std::string &error)
{
    size_t position = 0;
    int lineNumber = 0;
    std::vector<i32> *currentRect = nullptr;

    while (position <= text.size())
    {
        size_t lineEnd = text.find('\n', position);
        if (lineEnd == std::string::npos)
        {
            lineEnd = text.size();
        }
        std::string line = text.substr(position, lineEnd - position);
        position = lineEnd + 1;
        lineNumber++;

        // Drop a trailing comment, but not a # inside a string. The converter does not emit
        // either, so this is only for hand edits.
        const size_t hash = line.find('#');
        if (hash != std::string::npos)
        {
            line.resize(hash);
        }
        while (!line.empty() && (line.back() == ' ' || line.back() == '\r' ||
                                 line.back() == '\t'))
        {
            line.pop_back();
        }
        size_t begin = 0;
        while (begin < line.size() && (line[begin] == ' ' || line[begin] == '\t'))
        {
            begin++;
        }
        line = line.substr(begin);

        if (line.empty())
        {
            continue;
        }

        if (line == "[[sprite]]")
        {
            out.sprites.push_back({});
            currentRect = &out.sprites.back();
            continue;
        }
        if (line.front() == '[')
        {
            currentRect = nullptr;
            continue;
        }

        const size_t equals = line.find('=');
        if (equals == std::string::npos)
        {
            error = "line " + std::to_string(lineNumber) + ": expected key = value";
            return false;
        }
        std::string key = line.substr(0, equals);
        while (!key.empty() && (key.back() == ' ' || key.back() == '\t'))
        {
            key.pop_back();
        }
        std::string value = line.substr(equals + 1);
        size_t valueBegin = 0;
        while (valueBegin < value.size() &&
               (value[valueBegin] == ' ' || value[valueBegin] == '\t'))
        {
            valueBegin++;
        }
        value = value.substr(valueBegin);

        if (key == "version")
        {
            out.version = (i32)atoi(value.c_str());
        }
        else if (key == "width")
        {
            out.width = (i32)atoi(value.c_str());
        }
        else if (key == "height")
        {
            out.height = (i32)atoi(value.c_str());
        }
        else if (key == "texture")
        {
            if (value.size() >= 2 && value.front() == '"')
            {
                out.texture = value.substr(1, value.size() - 2);
            }
        }
        else if (key == "rect" && currentRect)
        {
            if (value.empty() || value.front() != '[' || value.back() != ']')
            {
                error = "line " + std::to_string(lineNumber) + ": rect is not an array";
                return false;
            }
            std::string body = value.substr(1, value.size() - 2);
            std::string item;
            for (size_t index = 0; index <= body.size(); index++)
            {
                const char c = index < body.size() ? body[index] : ',';
                if (c == ',')
                {
                    if (!item.empty())
                    {
                        currentRect->push_back(atoi(item.c_str()));
                        item.clear();
                    }
                }
                else if (c != ' ')
                {
                    item.push_back(c);
                }
            }
        }
    }

    if (out.sprites.empty() && out.texture.empty())
    {
        error = "no sprites and no texture";
        return false;
    }
    return true;
}

// --- the Lua host -------------------------------------------------------------------

// State shared with the Lua callbacks through the registry.
struct Host
{
    std::map<i32, std::vector<RaisedInstruction>> scripts;
    std::string error;
};

const char *kHostKey = "anmlua.host";

Host *GetHost(lua_State *L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, kHostKey);
    Host *host = (Host *)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return host;
}

// Reads the optional trailing `{ time = n, flags = n }` table every call carries.
void ReadOptions(lua_State *L, int index, RaisedInstruction &instruction)
{
    if (!lua_istable(L, index))
    {
        return;
    }
    lua_getfield(L, index, "time");
    if (lua_isnumber(L, -1))
    {
        instruction.time = (i32)lua_tointeger(L, -1);
    }
    lua_pop(L, 1);
    lua_getfield(L, index, "flags");
    if (lua_isnumber(L, -1))
    {
        instruction.flags = (i32)lua_tointeger(L, -1);
    }
    lua_pop(L, 1);
}

// The generic recorder: one call appends one instruction.
//
// Called as `script:emit(opcode, arg1, arg2, ..., { time = n, flags = n })`. Arguments are
// read according to the opcode's declared kinds, which is what makes the byte layout come
// out identical to the original's.
int l_emit(lua_State *L)
{
    Host *host = GetHost(L);
    if (!host)
    {
        return luaL_error(L, "no host");
    }

    const i32 scriptIndex = (i32)lua_tointeger(L, lua_upvalueindex(1));
    const i32 opcode = (i32)luaL_checkinteger(L, 2);

    const OpcodeInfo *info = FindOpcode((i16)opcode);
    if (!info)
    {
        return luaL_error(L, "unknown opcode %d", (int)opcode);
    }

    RaisedInstruction instruction;
    instruction.opcode = opcode;

    for (int i = 0; i < info->argCount; i++)
    {
        const int stackIndex = 3 + i;
        switch (info->args[i])
        {
        case ArgKind::Float:
            instruction.floats.push_back((f32)luaL_checknumber(L, stackIndex));
            break;
        case ArgKind::Byte:
        case ArgKind::Int:
            instruction.integers.push_back((i32)luaL_checkinteger(L, stackIndex));
            break;
        case ArgKind::None:
            break;
        }
    }

    ReadOptions(L, 3 + info->argCount, instruction);

    host->scripts[scriptIndex].push_back(instruction);
    return 0;
}

// anm.script(index) returns a table whose methods all forward to l_emit with the index
// captured as an upvalue, so the script does not have to pass it on every call.
int l_anm_script(lua_State *L)
{
    const i32 index = (i32)luaL_checkinteger(L, 1);

    lua_newtable(L);
    lua_pushinteger(L, index);
    lua_pushcclosure(L, l_emit, 1);
    lua_setfield(L, -2, "emit");
    return 1;
}

// Runs one entry's script and returns what it built.
bool RunScript(const char *anmName, u32 entryIndex, Host &host, std::string &error)
{
    std::string source;
    if (!ReadAsset(EntryPath(anmName, entryIndex, "animation.lua"), source))
    {
        error = "cannot read animation.lua";
        return false;
    }

    lua_State *L = luaL_newstate();
    if (!L)
    {
        error = "cannot create a Lua state";
        return false;
    }

    // Only the base library: the script is data, not a program that needs io or os.
    luaL_requiref(L, LUA_GNAME, luaopen_base, 1);
    lua_pop(L, 1);

    lua_pushlightuserdata(L, &host);
    lua_setfield(L, LUA_REGISTRYINDEX, kHostKey);

    lua_newtable(L);
    lua_pushcfunction(L, l_anm_script);
    lua_setfield(L, -2, "script");
    lua_setglobal(L, "anm");

    if (luaL_loadbuffer(L, source.c_str(), source.size(), "animation.lua") != LUA_OK)
    {
        error = lua_tostring(L, -1);
        lua_close(L);
        return false;
    }
    if (lua_pcall(L, 0, 0, 0) != LUA_OK)
    {
        error = lua_tostring(L, -1);
        lua_close(L);
        return false;
    }

    lua_close(L);
    return true;
}

} // namespace

bool Exists(const char *anmName)
{
    std::string text;
    return ReadAsset(EntryPath(anmName, 0, "uvs.toml"), text);
}

u32 CountEntries(const char *anmName)
{
    // Entries are numbered from zero with no gaps, so the count is the first index whose
    // description is absent.
    u32 count = 0;
    std::string text;
    while (count < 64 && ReadAsset(EntryPath(anmName, count, "uvs.toml"), text))
    {
        count++;
    }
    return count;
}

bool RaiseEntry(const char *anmName, u32 entryIndex, RaisedEntry &out, std::string &error)
{
    std::string uvText;
    if (!ReadAsset(EntryPath(anmName, entryIndex, "uvs.toml"), uvText))
    {
        error = "cannot read uvs.toml";
        return false;
    }

    UvFile uvs;
    if (!ParseUvs(uvText, uvs, error))
    {
        return false;
    }

    out.version = uvs.version;
    out.width = uvs.width;
    out.height = uvs.height;
    out.sprites = uvs.sprites;
    // LoadTexture opens the file from disk rather than through the asset store, so the path
    // has to be one the executable can open.
    out.texturePath = "th07.data/" + EntryPath(anmName, entryIndex, uvs.texture.c_str());

    Host host;
    if (!RunScript(anmName, entryIndex, host, error))
    {
        return false;
    }

    // Scripts come out of a map, so they are already ordered by index.
    for (const auto &pair : host.scripts)
    {
        out.scriptIndices.push_back(pair.first);
        out.scripts.push_back(pair.second);
    }
    return true;
}

namespace
{
// A growable byte buffer for one entry.
struct Writer
{
    std::vector<u8> bytes;

    u32 Position() const
    {
        return (u32)bytes.size();
    }

    u32 Align4()
    {
        while (bytes.size() % 4 != 0)
        {
            bytes.push_back(0);
        }
        return (u32)bytes.size();
    }

    void Put(const void *data, size_t size)
    {
        const u8 *from = (const u8 *)data;
        bytes.insert(bytes.end(), from, from + size);
    }

    void PutI32(i32 value)
    {
        Put(&value, sizeof(value));
    }

    void PutF32(f32 value)
    {
        Put(&value, sizeof(value));
    }

    void PatchI32(u32 at, i32 value)
    {
        memcpy(&bytes[at], &value, sizeof(value));
    }
};

// Writes one instruction. LoadAnm and the VM both depend on this exact layout: an 8-byte
// header, then one 4-byte word per argument.
void WriteInstruction(Writer &writer, const RaisedInstruction &instruction,
                      const OpcodeInfo &info)
{
    const u16 size = (u16)(8 + 4 * info.argCount);
    const i16 opcode = (i16)instruction.opcode;
    const i16 time = (i16)instruction.time;
    const u16 flags = (u16)instruction.flags;

    writer.Put(&opcode, sizeof(opcode));
    writer.Put(&size, sizeof(size));
    writer.Put(&time, sizeof(time));
    writer.Put(&flags, sizeof(flags));

    size_t floatCursor = 0;
    size_t intCursor = 0;
    for (int i = 0; i < info.argCount; i++)
    {
        i32 word = 0;
        if (info.args[i] == ArgKind::Float)
        {
            // The argument is a float; the same bytes are what the VM reads through .f.
            f32 value = instruction.floats[floatCursor++];
            memcpy(&word, &value, sizeof(word));
        }
        else
        {
            // Integers and bytes share the path: a byte argument is the low byte of the
            // word, which is what .b[0] sees.
            word = instruction.integers[intCursor++];
            if (info.args[i] == ArgKind::Byte)
            {
                word &= 0xFF;
            }
        }
        writer.Put(&word, sizeof(word));
    }
}
} // namespace

u8 *BuildEntries(const char *anmName, u32 *outSize, std::string &error)
{
    const u32 entryCount = CountEntries(anmName);
    if (entryCount == 0)
    {
        error = "no entries";
        return nullptr;
    }

    Writer writer;

    for (u32 entryIndex = 0; entryIndex < entryCount; entryIndex++)
    {
        RaisedEntry entry;
        if (!RaiseEntry(anmName, entryIndex, entry, error))
        {
            return nullptr;
        }

        writer.Align4();
        const u32 entryStart = writer.Position();

        const u32 numSprites = (u32)entry.sprites.size();
        const u32 numScripts = (u32)entry.scripts.size();

        AnmRawEntry header;
        memset(&header, 0, sizeof(header));
        header.numSprites = (i32)numSprites;
        header.numScripts = (i32)numScripts;
        header.width = entry.width;
        header.height = entry.height;
        // LoadAnm passes this to LoadTexture as the colour key. A PNG carries its own alpha,
        // so no key is wanted and any non-zero value would punch holes in it.
        header.format = 0;
        header.version = entry.version;
        header.priority = 0;
        header.hasData = 0;
        header.ownsMemory = 0;
        writer.Put(&header, sizeof(header));

        const u32 offsetTable = writer.Position();
        for (u32 i = 0; i < numSprites + numScripts * 2; i++)
        {
            writer.PutI32(0);
        }

        // Sprites: id is the index, and the rectangle is in texture pixels.
        std::vector<u32> spriteOffsets;
        for (u32 i = 0; i < numSprites; i++)
        {
            const std::vector<i32> &rect = entry.sprites[i];
            AnmRawSprite raw;
            memset(&raw, 0, sizeof(raw));
            raw.id = (i32)i;
            raw.offset.x = rect.size() > 0 ? (f32)rect[0] : 0.0f;
            raw.offset.y = rect.size() > 1 ? (f32)rect[1] : 0.0f;
            raw.size.x = rect.size() > 2 ? (f32)rect[2] : 0.0f;
            raw.size.y = rect.size() > 3 ? (f32)rect[3] : 0.0f;
            writer.Align4();
            spriteOffsets.push_back(writer.Position());
            writer.Put(&raw, sizeof(raw));
        }

        // The texture path, which LoadAnm reads to open the PNG.
        writer.Align4();
        // Offsets inside an entry are relative to the entry's start. The first entry begins
        // at zero, so an absolute offset looks right there and wrong everywhere after it.
        const i32 nameOffset = (i32)(writer.Position() - entryStart);
        writer.Put(entry.texturePath.c_str(), entry.texturePath.size() + 1);

        // Scripts.
        //
        // The index a script gets is not its position in the entry. LoadAnm stores it at
        // index + spriteIdxOffset, so the original writes whatever number makes the result
        // land where it wants. Those numbers are recorded by the converter and replayed
        // here, because the placement is not derivable: title01's entries 2 to 6 write
        // negative indices that put their scripts immediately after the first two entries,
        // while entries 7 to 9 write zeros that put theirs at the entry's own slot. Both are
        // valid and they put the scripts in different places, so guessing produces streams
        // that load and behave differently.
        std::vector<i32> scriptIndices;
        std::vector<u32> scriptOffsets;
        for (u32 i = 0; i < numScripts; i++)
        {
            writer.Align4();
            scriptOffsets.push_back(writer.Position());
            scriptIndices.push_back(entry.scriptIndices[i]);

            for (const RaisedInstruction &instruction : entry.scripts[i])
            {
                const OpcodeInfo *info = FindOpcode((i16)instruction.opcode);
                if (!info)
                {
                    error = "opcode " + std::to_string(instruction.opcode) + " is not supported";
                    return nullptr;
                }
                WriteInstruction(writer, instruction, *info);
            }
        }

        // Fill in the header now that the sizes are known.
        const u32 entrySize = writer.Position() - entryStart;
        const i32 sizeField = (i32)entrySize;
        writer.PatchI32(entryStart + offsetof(AnmRawEntry, nameOffset), nameOffset);
        writer.PatchI32(entryStart + offsetof(AnmRawEntry, textureOffset), sizeField);
        writer.PatchI32(entryStart + offsetof(AnmRawEntry, nextOffset), 0);

        // The offset table: sprite offsets first, then (script index, script offset) pairs.
        u32 cursor = offsetTable;
        for (u32 i = 0; i < numSprites; i++)
        {
            writer.PatchI32(cursor, (i32)(spriteOffsets[i] - entryStart));
            cursor += 4;
        }
        for (u32 i = 0; i < numScripts; i++)
        {
            writer.PatchI32(cursor, scriptIndices[i]);
            cursor += 4;
            writer.PatchI32(cursor, (i32)(scriptOffsets[i] - entryStart));
            cursor += 4;
        }

        // Each entry ends with a terminator. ExecuteScript's interrupt scan walks the
        // instruction stream by size and stops only at ANM_EXIT_HIDE or a matching label, so
        // a script that ends without one lets the scan continue into whatever follows. In the
        // binary that is the next entry's data, which happens to contain an exit; here it
        // would be padding, and a zero size stalls the scan and the frame with it.
        writer.Align4();
        {
            const i16 exitOpcode = ANM_EXIT_HIDE;
            const u16 exitSize = 8;
            const i16 exitTime = 0;
            const u16 exitFlags = 0;
            writer.Put(&exitOpcode, sizeof(exitOpcode));
            writer.Put(&exitSize, sizeof(exitSize));
            writer.Put(&exitTime, sizeof(exitTime));
            writer.Put(&exitFlags, sizeof(exitFlags));
        }

        // Chain to the next entry, if there is one.
        if (entryIndex + 1 < entryCount)
        {
            writer.Align4();
            writer.PatchI32(entryStart + offsetof(AnmRawEntry, nextOffset),
                            (i32)(writer.Position() - entryStart));
        }
    }

    // A terminator for interrupt scans.
    //
    // ExecuteScript exits with a scan over the instruction stream looking for an
    // ANM_INTERRUPT_LABEL whose number matches, and it stops only at another such label or at
    // ANM_EXIT_HIDE. It advances by each instruction's size field, so a zero size anywhere
    // makes the scan stand still and the frame never finishes -- which is exactly the stall
    // seen on the title screen.
    //
    // The original never runs into that because the bytes following a script happen to
    // contain an exit; relying on the same luck is not reasonable here, so the buffer ends
    // with an explicit ANM_EXIT_HIDE and any scan that runs past a script stops at it.
    writer.Align4();
    const i16 exitOpcode = ANM_EXIT_HIDE;
    const u16 exitSize = 8;
    const i16 exitTime = 0;
    const u16 exitFlags = 0;
    writer.Put(&exitOpcode, sizeof(exitOpcode));
    writer.Put(&exitSize, sizeof(exitSize));
    writer.Put(&exitTime, sizeof(exitTime));
    writer.Put(&exitFlags, sizeof(exitFlags));

    u8 *result = (u8 *)malloc(writer.bytes.size());
    if (!result)
    {
        error = "out of memory";
        return nullptr;
    }
    memcpy(result, writer.bytes.data(), writer.bytes.size());
    *outSize = (u32)writer.bytes.size();
    return result;
}

bool VerifyAgainstBinary(const char *anmName, std::string &error)
{
    // The original, read the same way LoadAnms reads it.
    std::string fileName = std::string(anmName) + ".anm";
    u32 binarySize = 0;
    u8 *binary = AssetStore::Read(fileName.c_str(), &binarySize);
    if (!binary)
    {
        // Nothing to compare against; an animation that only exists in the new form is fine.
        return true;
    }

    u32 builtSize = 0;
    u8 *built = BuildEntries(anmName, &builtSize, error);
    if (!built)
    {
        free(binary);
        return false;
    }

    FILE *log = SDL_getenv("TH07_ANM_DEBUG") ? fopen("anmlua.log", "a") : nullptr;
    if (log)
    {
        fprintf(log, "=== %s: binary %u bytes, built %u bytes\n", anmName,
                (unsigned)binarySize, (unsigned)builtSize);
    }

    // Walk both entry chains together.
    const u8 *binaryEntry = binary;
    const u8 *builtEntry = built;
    u32 entryIndex = 0;
    bool matched = true;

    while (true)
    {
        const AnmRawEntry *want = (const AnmRawEntry *)binaryEntry;
        const AnmRawEntry *have = (const AnmRawEntry *)builtEntry;
        ++entryIndex;

        const char *textures[] = {
            (const char *)binaryEntry + want->nameOffset,
            (const char *)builtEntry + have->nameOffset,
        };
        FILE *sink = log;
        if (sink)
        {
            fprintf(sink, "entry %u: sprites %d/%d scripts %d/%d texture %s | %s\n",
                    entryIndex - 1, want->numSprites, have->numSprites, want->numScripts,
                    have->numScripts, textures[0], textures[1]);
        }

        if (want->numSprites != have->numSprites || want->numScripts != have->numScripts)
        {
            error = std::string(anmName) + " entry " + std::to_string(entryIndex - 1) +
                    ": sprite or script count differs";
            matched = false;
            break;
        }

        // Sprites: compare every rectangle.
        {
            const i32 *wantTable = want->dataOffsets;
            const i32 *haveTable = have->dataOffsets;
            for (i32 i = 0; i < want->numSprites; i++)
            {
                const AnmRawSprite *a = (const AnmRawSprite *)(binaryEntry + wantTable[i]);
                const AnmRawSprite *b = (const AnmRawSprite *)(builtEntry + haveTable[i]);
                if (a->id != b->id || a->offset.x != b->offset.x ||
                    a->offset.y != b->offset.y || a->size.x != b->size.x ||
                    a->size.y != b->size.y)
                {
                    error = std::string(anmName) + " entry " +
                            std::to_string(entryIndex - 1) + " sprite " + std::to_string(i) +
                            " differs";
                    matched = false;
                    break;
                }
            }
            if (!matched)
            {
                break;
            }
        }

        // Scripts: compare the instruction streams instruction by instruction, and the bytes
        // of each instruction. The sizes come from the header, so a difference in argument
        // count shows up as a size difference.
        RaisedEntry raised;
        std::string raiseError;
        if (!RaiseEntry(anmName, entryIndex - 1, raised, raiseError))
        {
            error = std::string(anmName) + " entry " + std::to_string(entryIndex - 1) +
                    ": " + raiseError;
            matched = false;
            break;
        }

        for (i32 i = 0; i < want->numScripts && matched; i++)
        {
            const i32 *wantTable = want->dataOffsets;
            const i32 *haveTable = have->dataOffsets;
            const i32 *wantPair = wantTable + want->numSprites + i * 2;
            const i32 *havePair = haveTable + have->numSprites + i * 2;

            if (wantPair[0] != havePair[0])
            {
                error = std::string(anmName) + " entry " +
                        std::to_string(entryIndex - 1) + " script " + std::to_string(i) +
                        ": index " + std::to_string(wantPair[0]) + " vs " +
                        std::to_string(havePair[0]);
                matched = false;
                break;
            }

            const AnmRawInstr *a = (const AnmRawInstr *)(binaryEntry + wantPair[1]);
            const AnmRawInstr *b = (const AnmRawInstr *)(builtEntry + havePair[1]);

            // Count from the Lua side, which knows exactly how many instructions it emitted.
            // Walking until an exit opcode appears is unreliable here: an entry's scripts are
            // laid out back to back in the buffer, and the exit the original carries is not
            // always the last instruction before the next script begins.
            const std::vector<RaisedInstruction> *raisedScript = nullptr;
            for (size_t s = 0; s < raised.scripts.size(); s++)
            {
                if (raised.scriptIndices[s] == wantPair[0])
                {
                    raisedScript = &raised.scripts[s];
                    break;
                }
            }
            const i32 expected = raisedScript ? (i32)raisedScript->size() : 0;

            i32 position = 0;
            while (position < expected)
            {
                if (a->size == 0)
                {
                    error = std::string(anmName) + " entry " +
                            std::to_string(entryIndex - 1) + " script " + std::to_string(i) +
                            ": the original stream ended after " + std::to_string(position) +
                            " instructions, expected " + std::to_string(expected);
                    matched = false;
                    break;
                }
                if (a->opcode != b->opcode || a->time != b->time || a->size != b->size ||
                    a->flags != b->flags)
                {
                    error = std::string(anmName) + " entry " +
                            std::to_string(entryIndex - 1) + " script " + std::to_string(i) +
                            " instruction " + std::to_string(position) + ": opcode " +
                            std::to_string(a->opcode) + "/" + std::to_string(b->opcode) +
                            " time " + std::to_string(a->time) + "/" +
                            std::to_string(b->time) + " size " +
                            std::to_string(a->size) + "/" + std::to_string(b->size);
                    matched = false;
                    break;
                }

                // Arguments, byte for byte.
                const u32 words = (u32)((a->size - 8) / 4);
                for (u32 w = 0; w < words; w++)
                {
                    if (a->args[w].u != b->args[w].u)
                    {
                        error = std::string(anmName) + " entry " +
                                std::to_string(entryIndex - 1) + " script " +
                                std::to_string(i) + " instruction " +
                                std::to_string(position) + " argument " +
                                std::to_string(w) + " differs";
                        matched = false;
                        break;
                    }
                }
                if (!matched)
                {
                    break;
                }

                a = (const AnmRawInstr *)((const u8 *)a + a->size);
                b = (const AnmRawInstr *)((const u8 *)b + b->size);
                position++;
            }
        }

        if (!matched)
        {
            break;
        }

        // Follow the chains.
        const bool wantMore = want->nextOffset != 0;
        const bool haveMore = have->nextOffset != 0;
        if (wantMore != haveMore)
        {
            error = std::string(anmName) + ": entry count differs";
            matched = false;
            break;
        }
        if (!wantMore)
        {
            break;
        }
        binaryEntry += want->nextOffset;
        builtEntry += have->nextOffset;
    }

    if (log)
    {
        fprintf(log, "result: %s\n", matched ? "MATCH" : error.c_str());
        fclose(log);
    }

    free(binary);
    free(built);
    return matched;
}

} // namespace AnmLua
