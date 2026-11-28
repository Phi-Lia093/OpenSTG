#include "script/AnmSprites.hpp"

#include <cstdio>
#include <cstring>

#include "core/GameErrorContext.hpp"
#include "data/AssetStore.hpp"
#include "data/Toml.hpp"
#include "script/AnmManager.hpp"

namespace AnmSprites
{
namespace
{
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

std::string FileName(const char *anmName, i32 entryIndex)
{
    // One directory per animation holds its textures and sprite tables together, so the
    // files can be found and compared without sorting through the archive's own.
    char buffer[160];
    snprintf(buffer, sizeof(buffer), "anm/%s/sprites_%d.toml", anmName, (int)entryIndex);
    return buffer;
}

bool Exists(const char *anmName, i32 entryIndex)
{
    std::string text;
    return ReadAsset(FileName(anmName, entryIndex), text);
}

bool Load(const char *anmName, i32 entryIndex, std::vector<AnmRawSprite> &out,
          std::string &error)
{
    const std::string path = FileName(anmName, entryIndex);
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

    const std::vector<Toml::Document> *sprites = document.Tables("sprite");
    if (!sprites)
    {
        error = path + ": no [[sprite]] elements";
        return false;
    }

    out.clear();
    out.reserve(sprites->size());

    for (const Toml::Document &element : *sprites)
    {
        AnmRawSprite sprite;
        memset(&sprite, 0, sizeof(sprite));

        const Toml::Value *id = element.Get("id");
        const Toml::Value *rect = element.Get("rect");
        if (!id || !rect || rect->integers.size() != 4)
        {
            error = path + ": a sprite needs an id and a four-value rect";
            return false;
        }

        sprite.id = (i32)id->integer;
        sprite.offset.x = (f32)rect->integers[0];
        sprite.offset.y = (f32)rect->integers[1];
        sprite.size.x = (f32)rect->integers[2];
        sprite.size.y = (f32)rect->integers[3];
        out.push_back(sprite);
    }

    return true;
}

} // namespace AnmSprites
