#pragma once

#include <string>
#include <vector>

#include "core/inttypes.hpp"

// An animation entry, as animation.lua describes it.
//
// This replaced the binary header and then the TOML file that replaced it. What was a 0x40-byte
// structure full of offsets is now a short table of the facts the loader needs: where the
// texture is, how to read it, how many sprite slots the entry takes, and where each of its
// streams begins.
//
// The description is read by running the Lua and walking the table it returns, so the values
// are obtained from a real Lua value rather than parsed from text.
namespace AnmEntryFile
{
struct Script
{
    i32 index = 0;   // the number the game uses to run this stream
};

struct Sprite
{
    i32 id = 0;
    f32 offsetX = 0;  // top-left of the rectangle, in texture pixels
    f32 offsetY = 0;
    f32 sizeX = 0;
    f32 sizeY = 0;
};

struct Entry
{
    std::string texture;  // file name beside the entry, or "@" for an empty texture
    i32 width = 0;
    i32 height = 0;
    i32 format = 0;  // passed to LoadTexture as the colour key
    std::vector<Sprite> sprites;
    std::vector<Script> scripts;
};

// How many entries the animation describes, or 0 when the file is missing or malformed.
int Count(const char *anmName);

// Reads one entry. Returns false and fills `error` when the file is missing, when the Lua fails
// to run, or when a table the entry needs is absent.
bool Load(const char *anmName, i32 entryIndex, Entry &out, std::string &error);
} // namespace AnmEntryFile
