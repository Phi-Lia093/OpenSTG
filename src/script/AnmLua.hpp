#pragma once

#include <string>
#include <vector>

#include "core/inttypes.hpp"

struct AnmRawEntry;

// Loading an animation from Lua instead of the binary format.
//
// The new form is a directory per entry holding a texture, a TOML sprite table and a Lua
// script. The script is a transcription of the original instruction stream: one call per
// instruction, same arguments, same time. Running it once builds exactly the instruction
// array the binary format would have produced.
//
// The point of transcribing rather than reinterpreting is that the result can be checked.
// BuildFromLua returns an entry chain laid out like the original, so the same comparison
// that would catch a mistake in the conversion needs no understanding of what the animation
// means.
namespace AnmLua
{
// One instruction as Lua reported it, before it is turned into bytes.
struct RaisedInstruction
{
    i32 opcode = 0;
    i32 time = 0;
    i32 flags = 0;
    std::vector<f32> floats;
    std::vector<i32> integers;
};

// One entry's worth of data: its description and its scripts.
struct RaisedEntry
{
    i32 version = 2;
    i32 width = 0;
    i32 height = 0;
    std::string texturePath;
    // rects as x, y, width, height in texture pixels, indexed by sprite id.
    std::vector<std::vector<i32>> sprites;
    // Scripts keyed by their index; each is the instruction stream it builds.
    std::vector<std::vector<RaisedInstruction>> scripts;
    // Script indices, which need not be 0..n-1: the original numbers them.
    std::vector<i32> scriptIndices;
};

// Whether a directory holding this animation exists in the data.
bool Exists(const char *anmName);

// How many entries the animation holds; zero when it cannot be read.
u32 CountEntries(const char *anmName);

// Runs the Lua for one entry and returns what it built. Returns false when the entry
// cannot be read or its script fails, in which case `error` says why.
bool RaiseEntry(const char *anmName, u32 entryIndex, RaisedEntry &out, std::string &error);

// Lays the entries out as the AnmRawEntry chain LoadAnms expects. The returned buffer is
// malloc'd and owned by the caller, matching how the binary path returns its file.
u8 *BuildEntries(const char *anmName, u32 *outSize, std::string &error);

// Compares the chain built from Lua against the original file, instruction by instruction.
//
// This is the check that makes the whole approach safe: the Lua is a transcription, so if it
// was transcribed correctly the two produce identical instruction streams. It reports the
// first difference by entry, script, instruction and field. With TH07_ANM_DEBUG set it also
// writes the comparison to anmlua.log.
//
// Returns true when they match. A missing original is not a failure and returns true.
bool VerifyAgainstBinary(const char *anmName, std::string &error);
} // namespace AnmLua
