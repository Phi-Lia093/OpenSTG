#include "script/AnmEntryFile.hpp"

#include <cstdio>
#include <cstdlib>

#include "data/AssetStore.hpp"
#include "data/Toml.hpp"

namespace AnmEntryFile
{
namespace
{
std::string PathFor(const char *anmName, i32 entryIndex)
{
    char buffer[160];
    snprintf(buffer, sizeof(buffer), "anm/%s/%d.toml", anmName, (int)entryIndex);
    return buffer;
}

bool ReadAsset(const std::string &path, std::string &out)
{
    u32 size = 0;
    u8 *data = AssetStore::Read(path.c_str(), &size);
    if (!data)
    {
        return false;
    }
    out.assign((const char *)data, size);
    free(data);
    return true;
}
} // namespace

int Count(const char *anmName)
{
    // The entries are numbered without gaps, so counting stops at the first missing one.
    int count = 0;
    std::string text;
    while (count < 64 && ReadAsset(PathFor(anmName, count), text))
    {
        count++;
    }
    return count;
}

bool Load(const char *anmName, i32 entryIndex, Entry &out, std::string &error)
{
    const std::string path = PathFor(anmName, entryIndex);

    std::string text;
    if (!ReadAsset(path, text))
    {
        error = "cannot read " + path;
        return false;
    }

    Toml::Document document;
    if (!Toml::Parse(text, document, error))
    {
        error = path + ": " + error;
        return false;
    }

    const Toml::Value *texture = document.Get("texture");
    const Toml::Value *width = document.Get("width");
    const Toml::Value *height = document.Get("height");
    const Toml::Value *format = document.Get("format");
    const Toml::Value *sprites = document.Get("sprites");

    if (!texture || !width || !height || !format || !sprites)
    {
        error = path + ": texture, width, height, format and sprites are all required";
        return false;
    }

    out.texture = texture->text;
    out.width = (i32)width->integer;
    out.height = (i32)height->integer;
    out.format = (i32)format->integer;
    out.sprites.clear();
    out.scripts.clear();

    // The rectangles are part of this document: they describe the same entry and are always
    // needed with it, so reading them here avoids a second file per entry.
    if (const std::vector<Toml::Document> *elements = document.Tables("sprite"))
    {
        for (const Toml::Document &element : *elements)
        {
            const Toml::Value *id = element.Get("id");
            const Toml::Value *rect = element.Get("rect");
            if (!id || !rect || rect->integers.size() != 4)
            {
                error = path + ": a [[sprite]] needs an id and a four-value rect";
                return false;
            }
            Sprite sprite;
            sprite.id = (i32)id->integer;
            sprite.offsetX = (f32)rect->integers[0];
            sprite.offsetY = (f32)rect->integers[1];
            sprite.sizeX = (f32)rect->integers[2];
            sprite.sizeY = (f32)rect->integers[3];
            out.sprites.push_back(sprite);
        }
    }

    if (out.sprites.size() != (size_t)sprites->integer)
    {
        error = path + ": the description says " + std::to_string(sprites->integer) +
                " sprites but " + std::to_string(out.sprites.size()) + " are listed";
        return false;
    }

    if (const std::vector<Toml::Document> *scripts = document.Tables("script"))
    {
        for (const Toml::Document &element : *scripts)
        {
            const Toml::Value *index = element.Get("index");
            const Toml::Value *offset = element.Get("offset");
            if (!index || !offset)
            {
                error = path + ": a [[script]] needs an index and an offset";
                return false;
            }
            Script script;
            script.index = (i32)index->integer;
            script.offset = (i32)offset->integer;
            out.scripts.push_back(script);
        }
    }

    return true;
}

} // namespace AnmEntryFile
