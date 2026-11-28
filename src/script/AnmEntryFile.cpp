#include "script/AnmEntryFile.hpp"

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

#include "core/GameErrorContext.hpp"
#include "data/AssetStore.hpp"

namespace AnmEntryFile
{
namespace
{
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

// A loaded description, kept between calls.
//
// The file holds every entry, and the loader asks for them one at a time as it walks an
// animation. Re-running the Lua for each entry would mean parsing the same file up to ten
// times per animation, so the state is kept and released when a different animation is asked
// for. One state at a time is enough because animations are loaded one after another.
struct Loaded
{
    std::string name;
    lua_State *state = nullptr;
    int entryCount = 0;
};

Loaded g_loaded;

void Close()
{
    if (g_loaded.state)
    {
        lua_close(g_loaded.state);
        g_loaded.state = nullptr;
    }
    g_loaded.name.clear();
    g_loaded.entryCount = 0;
}

bool Open(const char *anmName)
{
    if (g_loaded.state && g_loaded.name == anmName)
    {
        return true;
    }
    Close();

    std::string text;
    if (!ReadAsset(PathFor(anmName), text))
    {
        return false;
    }

    lua_State *state = luaL_newstate();
    if (!state)
    {
        return false;
    }
    luaL_openlibs(state);

    if (luaL_loadbuffer(state, text.c_str(), text.size(), PathFor(anmName).c_str()) != LUA_OK ||
        lua_pcall(state, 0, 1, 0) != LUA_OK)
    {
        g_GameErrorContext.Log("アニメ %s: %s\n", anmName, lua_tostring(state, -1));
        lua_close(state);
        return false;
    }

    if (!lua_istable(state, -1))
    {
        g_GameErrorContext.Log("アニメ %s: 記述がテーブルではありません\n", anmName);
        lua_close(state);
        return false;
    }

    // The entries are keyed by number, so the count is the number of integer keys.
    int count = 0;
    lua_pushnil(state);
    while (lua_next(state, -2) != 0)
    {
        lua_pop(state, 1);
        count++;
    }

    g_loaded.name = anmName;
    g_loaded.state = state;
    g_loaded.entryCount = count;
    return true;
}

bool ReadNumber(lua_State *state, int table, const char *key, double &out)
{
    lua_getfield(state, table, key);
    const bool ok = lua_isnumber(state, -1);
    if (ok)
    {
        out = lua_tonumber(state, -1);
    }
    lua_pop(state, 1);
    return ok;
}
} // namespace

int Count(const char *anmName)
{
    return Open(anmName) ? g_loaded.entryCount : 0;
}

bool Load(const char *anmName, i32 entryIndex, Entry &out, std::string &error)
{
    if (!Open(anmName))
    {
        error = "cannot read " + PathFor(anmName);
        return false;
    }

    lua_State *state = g_loaded.state;
    const int root = lua_gettop(state);

    lua_geti(state, root, entryIndex);
    if (!lua_istable(state, -1))
    {
        lua_settop(state, root);
        error = PathFor(anmName) + ": no entry " + std::to_string(entryIndex);
        return false;
    }
    const int entry = lua_gettop(state);

    lua_getfield(state, entry, "texture");
    if (!lua_isstring(state, -1))
    {
        lua_settop(state, root);
        error = PathFor(anmName) + ": entry " + std::to_string(entryIndex) +
                " has no texture";
        return false;
    }
    out.texture = lua_tostring(state, -1);
    lua_pop(state, 1);

    double width = 0, height = 0, format = 0;
    if (!ReadNumber(state, entry, "width", width) ||
        !ReadNumber(state, entry, "height", height) ||
        !ReadNumber(state, entry, "format", format))
    {
        lua_settop(state, root);
        error = PathFor(anmName) + ": entry " + std::to_string(entryIndex) +
                " needs width, height and format";
        return false;
    }
    out.width = (i32)width;
    out.height = (i32)height;
    out.format = (i32)format;

    out.sprites.clear();
    lua_getfield(state, entry, "sprites");
    if (lua_istable(state, -1))
    {
        const int sprites = lua_gettop(state);
        const int count = (int)lua_rawlen(state, sprites);
        for (int index = 1; index <= count; index++)
        {
            lua_geti(state, sprites, index);
            if (lua_istable(state, -1))
            {
                Sprite sprite;
                double id = 0;
                ReadNumber(state, lua_gettop(state), "id", id);
                sprite.id = (i32)id;

                lua_getfield(state, -1, "rect");
                if (lua_istable(state, -1) && lua_rawlen(state, -1) == 4)
                {
                    for (int part = 0; part < 4; part++)
                    {
                        lua_geti(state, -1, part + 1);
                        const f32 value = (f32)lua_tonumber(state, -1);
                        lua_pop(state, 1);
                        switch (part)
                        {
                        case 0: sprite.offsetX = value; break;
                        case 1: sprite.offsetY = value; break;
                        case 2: sprite.sizeX = value; break;
                        default: sprite.sizeY = value; break;
                        }
                    }
                }
                lua_pop(state, 1);
                out.sprites.push_back(sprite);
            }
            lua_pop(state, 1);
        }
    }
    lua_pop(state, 1);

    out.scripts.clear();
    lua_getfield(state, entry, "scripts");
    if (lua_istable(state, -1))
    {
        const int scripts = lua_gettop(state);
        const int count = (int)lua_rawlen(state, scripts);
        for (int index = 1; index <= count; index++)
        {
            lua_geti(state, scripts, index);
            if (lua_istable(state, -1))
            {
                Script script;
                double id = 0;
                ReadNumber(state, lua_gettop(state), "index", id);
                script.index = (i32)id;
                out.scripts.push_back(script);
            }
            lua_pop(state, 1);
        }
    }
    lua_pop(state, 1);

    lua_settop(state, root);
    return true;
}

} // namespace AnmEntryFile
