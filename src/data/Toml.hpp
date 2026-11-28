#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// A small TOML reader.
//
// TOML is a large specification: inline tables, datetimes, multi-line strings, dotted keys.
// None of that appears in the files this reads, and pulling in a full implementation would add
// a dependency for a fraction of its features.
//
// What this reads:
//
//     # comment                       whole-line and trailing comments
//     [table]                         a table header, one level, opened or reopened
//     [[table]]                       an array of tables; repeated, each opens a new element
//     key = "string"                  basic strings, with \" \\ \n \t \\uXXXX
//     key = 123                       integers, decimal, optionally signed
//     key = true / false              booleans
//     key = 1.5                       floats
//     key = [1, 2, 3]                 arrays of a single value type
//
// What it does not read: dates, inline tables, multi-line strings, dotted keys, literal
// (single-quoted) strings. A file using them reports an error rather than being silently
// misread.
//
// Key lookups are by "table.key", so a caller asks for "game.lives". Values inside an array of
// tables are reached through Tables(), which returns one Document per element.

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
    TableArray,
};

struct Document;

struct Value
{
    ValueType type = ValueType::None;
    bool boolean = false;
    long long integer = 0;
    double number = 0.0;
    std::string text;
    std::vector<long long> integers;
    std::vector<std::string> strings;

    // The elements of a [[table]] array, each read like a document of its own. Kept as a
    // pointer so Value stays cheap to copy in the common case, where it is not a table array.
    std::vector<Document> *tables = nullptr;

    ~Value();
    Value() = default;
    Value(const Value &other);
    Value &operator=(const Value &other);
};

struct Document
{
    std::unordered_map<std::string, Value> values;

    // Whether the key was present at all. Distinguishes "absent" from "present but wrong".
    bool Has(const std::string &key) const;
    const Value *Get(const std::string &key) const;

    // The elements of an array of tables, or an empty span when the key is something else.
    const std::vector<Document> *Tables(const std::string &key) const;
};

// Parses `text`. On failure returns false and fills `error` with a line number and reason.
bool Parse(const std::string &text, Document &document, std::string &error);
} // namespace Toml
