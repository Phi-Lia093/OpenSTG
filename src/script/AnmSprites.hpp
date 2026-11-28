#pragma once

#include <string>
#include <vector>

#include "core/inttypes.hpp"

struct AnmRawSprite;

// Reading an animation's sprite table from TOML.
//
// The table used to live inside the ANM file, reached through the entry's offset table. It is
// a TOML file beside the animation now, one [[sprite]] element per rectangle, so the numbers
// can be read and edited rather than decoded.
//
// The loader's own logic is untouched: this returns the same AnmRawSprite values the binary
// table held, and everything downstream -- scaling by texture size, sprite indices -- works on
// those exactly as before. Only the source of the numbers changes, which is what makes this
// step testable on its own.
namespace AnmSprites
{
// The name of the sprite file for one entry.
std::string FileName(const char *anmName, i32 entryIndex);

// Whether that file exists.
bool Exists(const char *anmName, i32 entryIndex);

// Reads the table into `out`, in file order. Returns false and fills `error` when the file is
// missing or malformed.
bool Load(const char *anmName, i32 entryIndex, std::vector<AnmRawSprite> &out,
          std::string &error);
} // namespace AnmSprites
