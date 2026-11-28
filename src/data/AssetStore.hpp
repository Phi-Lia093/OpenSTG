#pragma once

#include <string>
#include <vector>

#include "core/inttypes.hpp"

// Access to the game's data files.
//
// The original read them from a PBG4 archive. The archive is gone; assets now live either
// as loose files in a directory next to the executable or, later, in a ZIP. Callers only
// ever ask for a file by name, so which container is behind this does not matter to them.
//
// Lookup is by bare file name. The game asks for paths like "data/ascii.anm", and the
// container holds files without the directory part, which is how the archive behaved.
namespace AssetStore
{
// Opens the asset container for this session.
//
// Tries, in order, a directory and then a ZIP next to the executable. Returns false when
// neither exists, which is fatal: the game has nothing to load.
bool Open();

// Releases whatever Open() acquired.
void Close();

// Reads a file into a freshly allocated buffer. Returns null when the file is missing;
// the caller frees the result with free(). `size` receives the byte count.
u8 *Read(const char *name, u32 *size);

// Whether a file is present, without reading it.
bool Exists(const char *name);

// Names of every file in the container, for diagnostics.
const std::vector<std::string> &List();
} // namespace AssetStore
