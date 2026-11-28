#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// A small TOML reader, covering the part of the format the configuration file uses.
//
// TOML is a large specification: arrays of tables, inline tables, datetimes, multi-line
// strings, dotted keys. None of that appears in a settings file, and pulling in a full
// implementation would add a dependency for a fraction of its features.
//
// What this reads:
//
//     # comment                       whole-line and trailing comments
//     [table]                         a table header, one level, opened or reopened
//     key = "string"                  basic strings, with \" \\ \n \t \\uXXXX
//     key = 123                       integers, decimal, optionally signed
//     key = true / false              booleans
//     key = 1.5                       floats
//     key = [1, 2, 3]                 arrays of a single value type
//
// What it does not read: dates, inline tables, arrays of tables, multi-line strings,
// dotted keys, literal (single-quoted) strings. A file using them reports an error rather
// than being silently misread.
//
// Key lookups are by "table.key", so a caller asks for "game.lives".

namespace Toml
{
enum class ValueType
{
    None,
    Boolean,
    Integer,
    Float,
    String,
    Array,
};

struct Value
{
    ValueType type = ValueType::None;
    bool boolean = false;
    long long integer = 0;
    double number = 0.0;
    std::string text;
    std::vector<long long> integers;
    std::vector<std::string> strings;
};

struct Document
{
    std::unordered_map<std::string, Value> values;

    // Whether the key was present at all. Distinguishes "absent" from "present but wrong".
    bool Has(const std::string &key) const;
    const Value *Get(const std::string &key) const;
};

// Parses `text`. On failure returns false and fills `error` with a line number and reason.
bool Parse(const std::string &text, Document &document, std::string &error);
} // namespace Toml
