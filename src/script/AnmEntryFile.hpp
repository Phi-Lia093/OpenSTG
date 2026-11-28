#pragma once

#include <string>
#include <vector>

#include "core/inttypes.hpp"

// An animation entry, as described by anm/<name>/<entry>.toml.
//
// This replaces the binary header. What was a 0x40-byte structure full of offsets is now a
// short list of the facts the loader needs: where the texture is, how to read it, how many
// sprite slots the entry takes, and where each of its streams begins in scripts_N.bin.
//
// Everything the old header carried that was layout bookkeeping -- nameOffset, nextOffset,
// dataOffsets, textureOffset -- is gone, because there is no longer a layout to describe.
namespace AnmEntryFile
{
struct Script
{
    i32 index = 0;   // the number the game uses to run this stream
    i32 offset = 0;  // where it starts in the entry's script file
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

// Number of entries the animation has, or -1 when it has none. Counted by the description
// files that exist, since the entries are now a set of files rather than a chain.
int Count(const char *anmName);

// Reads one entry's description. Returns false and fills `error` when it is missing or
// malformed.
bool Load(const char *anmName, i32 entryIndex, Entry &out, std::string &error);
} // namespace AnmEntryFile
