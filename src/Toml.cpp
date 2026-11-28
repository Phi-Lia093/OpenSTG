#include "Toml.hpp"

#include <cctype>
#include <cstdlib>

namespace Toml
{
namespace
{
bool IsSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\r';
}

std::string Trim(const std::string &s)
{
    size_t begin = 0;
    while (begin < s.size() && IsSpace(s[begin]))
    {
        begin++;
    }
    size_t end = s.size();
    while (end > begin && IsSpace(s[end - 1]))
    {
        end--;
    }
    return s.substr(begin, end - begin);
}

// Removes a trailing comment, which starts at # outside a string.
void StripComment(std::string &line)
{
    bool inString = false;
    bool escaped = false;
    for (size_t i = 0; i < line.size(); i++)
    {
        const char c = line[i];
        if (escaped)
        {
            escaped = false;
            continue;
        }
        if (c == '\\' && inString)
        {
            escaped = true;
            continue;
        }
        if (c == '"')
        {
            inString = !inString;
            continue;
        }
        if (c == '#' && !inString)
        {
            line.resize(i);
            return;
        }
    }
}

// Reads a basic string starting at the opening quote. Advances `pos` past the closing one.
bool ParseString(const std::string &s, size_t &pos, std::string &out)
{
    if (pos >= s.size() || s[pos] != '"')
    {
        return false;
    }
    pos++;
    out.clear();
    while (pos < s.size())
    {
        const char c = s[pos];
        if (c == '"')
        {
            pos++;
            return true;
        }
        if (c != '\\')
        {
            out.push_back(c);
            pos++;
            continue;
        }
        pos++;
        if (pos >= s.size())
        {
            return false;
        }
        switch (s[pos])
        {
        case 'n':
            out.push_back('\n');
            break;
        case 't':
            out.push_back('\t');
            break;
        case 'r':
            out.push_back('\r');
            break;
        case '"':
            out.push_back('"');
            break;
        case '\\':
            out.push_back('\\');
            break;
        case 'u':
        {
            // \uXXXX: only the BMP is supported, which covers what a settings file needs.
            if (pos + 4 >= s.size())
            {
                return false;
            }
            const std::string hex = s.substr(pos + 1, 4);
            char *end = nullptr;
            const long code = strtol(hex.c_str(), &end, 16);
            if (!end || *end != '\0')
            {
                return false;
            }
            // Encode as UTF-8.
            const unsigned cp = (unsigned)code;
            if (cp < 0x80)
            {
                out.push_back((char)cp);
            }
            else if (cp < 0x800)
            {
                out.push_back((char)(0xC0 | (cp >> 6)));
                out.push_back((char)(0x80 | (cp & 0x3F)));
            }
            else
            {
                out.push_back((char)(0xE0 | (cp >> 12)));
                out.push_back((char)(0x80 | ((cp >> 6) & 0x3F)));
                out.push_back((char)(0x80 | (cp & 0x3F)));
            }
            pos += 4;
            break;
        }
        default:
            return false;
        }
        pos++;
    }
    return false;
}

// Parses an array of numbers or strings. Advances `pos` past the closing bracket.
bool ParseArray(const std::string &s, size_t &pos, Value &value)
{
    if (pos >= s.size() || s[pos] != '[')
    {
        return false;
    }
    pos++;
    bool sawString = false;
    bool sawNumber = false;
    while (true)
    {
        while (pos < s.size() && IsSpace(s[pos]))
        {
            pos++;
        }
        if (pos >= s.size())
        {
            return false;
        }
        if (s[pos] == ']')
        {
            pos++;
            break;
        }
        if (s[pos] == ',')
        {
            pos++;
            continue;
        }
        if (s[pos] == '"')
        {
            std::string item;
            if (!ParseString(s, pos, item))
            {
                return false;
            }
            sawString = true;
            value.strings.push_back(item);
            continue;
        }
        // A number.
        const char *start = s.c_str() + pos;
        char *end = nullptr;
        const long long number = strtoll(start, &end, 10);
        if (end == start)
        {
            return false;
        }
        sawNumber = true;
        value.integers.push_back(number);
        pos += (size_t)(end - start);
    }
    if (sawString && sawNumber)
    {
        return false;
    }
    value.type = ValueType::Array;
    return true;
}

} // namespace

bool Document::Has(const std::string &key) const
{
    return values.find(key) != values.end();
}

const Value *Document::Get(const std::string &key) const
{
    auto it = values.find(key);
    return it == values.end() ? nullptr : &it->second;
}

bool Parse(const std::string &text, Document &document, std::string &error)
{
    document.values.clear();
    std::string table;

    size_t lineNumber = 0;
    size_t pos = 0;
    while (pos <= text.size())
    {
        size_t lineEnd = text.find('\n', pos);
        if (lineEnd == std::string::npos)
        {
            lineEnd = text.size();
        }
        std::string line = text.substr(pos, lineEnd - pos);
        pos = lineEnd + 1;
        lineNumber++;

        line = Trim(line);
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        // A table header is the whole line, comments already stripped.
        if (line[0] == '[')
        {
            const size_t close = line.find(']');
            if (close == std::string::npos)
            {
                error = "line " + std::to_string(lineNumber) + ": unterminated table header";
                return false;
            }
            std::string rest = Trim(line.substr(close + 1));
            StripComment(rest);
            if (!Trim(rest).empty())
            {
                error = "line " + std::to_string(lineNumber) + ": trailing text after table";
                return false;
            }
            table = Trim(line.substr(1, close - 1));
            if (table.empty())
            {
                error = "line " + std::to_string(lineNumber) + ": empty table name";
                return false;
            }
            continue;
        }

        const size_t equals = line.find('=');
        if (equals == std::string::npos)
        {
            error = "line " + std::to_string(lineNumber) + ": expected key = value";
            return false;
        }

        std::string key = Trim(line.substr(0, equals));
        std::string raw = line.substr(equals + 1);
        StripComment(raw);
        raw = Trim(raw);

        if (key.empty())
        {
            error = "line " + std::to_string(lineNumber) + ": empty key";
            return false;
        }
        if (raw.empty())
        {
            error = "line " + std::to_string(lineNumber) + ": missing value for " + key;
            return false;
        }

        const std::string fullKey = table.empty() ? key : table + "." + key;

        Value value;
        if (raw[0] == '"')
        {
            size_t valuePos = 0;
            if (!ParseString(raw, valuePos, value.text) ||
                !Trim(raw.substr(valuePos)).empty())
            {
                error = "line " + std::to_string(lineNumber) + ": malformed string";
                return false;
            }
            value.type = ValueType::String;
        }
        else if (raw[0] == '[')
        {
            size_t valuePos = 0;
            if (!ParseArray(raw, valuePos, value) || !Trim(raw.substr(valuePos)).empty())
            {
                error = "line " + std::to_string(lineNumber) + ": malformed array";
                return false;
            }
        }
        else if (raw == "true" || raw == "false")
        {
            value.type = ValueType::Boolean;
            value.boolean = raw == "true";
        }
        else
        {
            // Integer or float. A '.' or an exponent makes it a float.
            const bool isFloat = raw.find('.') != std::string::npos ||
                                 raw.find('e') != std::string::npos ||
                                 raw.find('E') != std::string::npos;
            char *end = nullptr;
            if (isFloat)
            {
                value.number = strtod(raw.c_str(), &end);
                value.type = ValueType::Float;
            }
            else
            {
                value.integer = strtoll(raw.c_str(), &end, 10);
                value.type = ValueType::Integer;
            }
            if (end == raw.c_str() || !Trim(end).empty())
            {
                error = "line " + std::to_string(lineNumber) + ": malformed number '" + raw +
                        "'";
                return false;
            }
        }

        document.values[fullKey] = value;
    }

    return true;
}

} // namespace Toml
