#include "script/AnmScripts.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "data/AssetStore.hpp"

namespace AnmScripts
{
std::string FileName(const char *anmName, i32 entryIndex)
{
    char buffer[160];
    snprintf(buffer, sizeof(buffer), "anm/%s/scripts_%d.bin", anmName, (int)entryIndex);
    return buffer;
}

u8 *Load(const char *anmName, i32 entryIndex, u32 *outSize, std::string &error)
{
    const std::string path = FileName(anmName, entryIndex);

    u32 size = 0;
    u8 *data = AssetStore::Read(path.c_str(), &size);
    if (!data)
    {
        error = "cannot read " + path;
        return nullptr;
    }
    if (size == 0)
    {
        free(data);
        error = path + " is empty";
        return nullptr;
    }

    if (outSize)
    {
        *outSize = size;
    }
    return data;
}
} // namespace AnmScripts
