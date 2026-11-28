#pragma once

#include "core/Supervisor.hpp"

// Reading and writing th07.toml.
//
// The original stored its settings as a 56-byte binary struct and cast the file straight
// into it, so a single wrong byte silently replaced every setting with defaults. This
// replaces that with a text file: every field is named, ranges are checked one at a time,
// and a bad value resets only that value instead of the whole file.
//
// The in-memory GameConfiguration is untouched -- only how it reaches disk changes.
namespace ConfigFile
{
// Reads `path` into `config`. Fields absent from the file keep their current value, which
// is how the defaults survive: the caller fills `config` with defaults first.
//
// Returns false only when the file exists but could not be parsed. Missing files are not an
// error; the caller decides whether to write one.
bool Load(const char *path, GameConfiguration &config);

// The same, from text already in memory.
bool LoadText(const char *text, GameConfiguration &config);

// Writes `config` to `path`, replacing whatever is there. Returns false on write failure.
bool Save(const char *path, const GameConfiguration &config);

// The text Save() would produce, for tests and for callers that write it themselves.
std::string Serialize(const GameConfiguration &config);
} // namespace ConfigFile
