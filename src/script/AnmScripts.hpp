#pragma once

#include <string>
#include <vector>

#include "core/inttypes.hpp"

struct AnmRawEntry;

// The instruction streams of one animation entry, loaded from beside it.
//
// The streams used to live inside the ANM file, reached as entry + offset. They are a binary
// file of their own now, anm/<name>/scripts_<entry>.bin, in the same encoding as before: the
// move was a change of location, not of format, so executing them needs no new code.
//
// AnmScripts::Load returns a buffer holding exactly those streams. The entry's offsets index
// it, which is why they were rebased by the converter: they were measured from the entry
// before, and are measured from this buffer now.
namespace AnmScripts
{
// The name of the file holding one entry's streams.
std::string FileName(const char *anmName, i32 entryIndex);

// Reads that file. The returned buffer is malloc'd and owned by the caller -- the loader stores
// the pointer and releases it with the entry -- and `outSize` receives its length. Returns
// null and fills `error` when the file is missing or empty.
u8 *Load(const char *anmName, i32 entryIndex, u32 *outSize, std::string &error);
} // namespace AnmScripts
