#include "AssetStore.hpp"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <system_error>

namespace AssetStore
{
namespace
{
// Where the assets currently come from. ZIP is not implemented yet; the enumeration exists
// so that adding it is a new case rather than a change to the callers.
enum class Container
{
    None,
    Directory,
    Zip,
};

Container g_Container = Container::None;
std::string g_Root;        // directory holding the files, with a trailing separator
std::string g_ArchivePath; // the ZIP, when that is the container
std::vector<std::string> g_Names;

std::string BasePath()
{
    const char *base = SDL_GetBasePath();
    return base ? std::string(base) : std::string();
}

// Records the name of every file under `directory`, relative to it, with forward slashes.
void CollectNames(const std::filesystem::path &directory)
{
    std::error_code ec;
    for (auto it = std::filesystem::recursive_directory_iterator(directory, ec);
         !ec && it != std::filesystem::recursive_directory_iterator(); it.increment(ec))
    {
        if (!it->is_regular_file(ec))
        {
            continue;
        }
        std::string relative = std::filesystem::relative(it->path(), directory, ec).string();
        if (ec)
        {
            continue;
        }
        for (char &c : relative)
        {
            if (c == '\\')
            {
                c = '/';
            }
        }
        g_Names.push_back(relative);
    }
}

} // namespace

bool Open()
{
    Close();

    const std::string base = BasePath();
    const std::string dataDir = base + "th07.data/";
    const std::string zipPath = base + "th07.zip";

    std::error_code ec;
    if (std::filesystem::is_directory(dataDir, ec))
    {
        g_Container = Container::Directory;
        g_Root = dataDir;
        CollectNames(dataDir);
        SDL_Log("assets: %zu files from %s", g_Names.size(), g_Root.c_str());
        return !g_Names.empty();
    }

    if (std::filesystem::is_regular_file(zipPath, ec))
    {
        // Reserved. The abstraction is in place; the reader is not.
        SDL_Log("assets: %s found, but ZIP support is not implemented", zipPath.c_str());
        return false;
    }

    SDL_Log("assets: neither th07.data/ nor th07.zip found next to the executable");
    return false;
}

void Close()
{
    g_Container = Container::None;
    g_Root.clear();
    g_ArchivePath.clear();
    g_Names.clear();
}

u8 *Read(const char *name, u32 *size)
{
    if (size)
    {
        *size = 0;
    }
    if (!name || g_Container == Container::None)
    {
        return NULL;
    }

    switch (g_Container)
    {
    case Container::Directory:
    {
        const std::string path = g_Root + name;
        SDL_IOStream *file = SDL_IOFromFile(path.c_str(), "rb");
        if (!file)
        {
            return NULL;
        }
        const Sint64 length = SDL_GetIOSize(file);
        if (length <= 0)
        {
            SDL_CloseIO(file);
            return NULL;
        }
        u8 *buffer = (u8 *)malloc((size_t)length);
        if (!buffer)
        {
            SDL_CloseIO(file);
            return NULL;
        }
        const size_t read = SDL_ReadIO(file, buffer, (size_t)length);
        SDL_CloseIO(file);
        if (read != (size_t)length)
        {
            free(buffer);
            return NULL;
        }
        if (size)
        {
            *size = (u32)length;
        }
        return buffer;
    }
    case Container::Zip:
        // Not implemented; see Open().
        return NULL;
    case Container::None:
        return NULL;
    }
    return NULL;
}

bool Exists(const char *name)
{
    if (!name || g_Container == Container::None)
    {
        return false;
    }
    if (g_Container == Container::Directory)
    {
        std::error_code ec;
        return std::filesystem::is_regular_file(g_Root + name, ec);
    }
    return false;
}

const std::vector<std::string> &List()
{
    return g_Names;
}

} // namespace AssetStore
