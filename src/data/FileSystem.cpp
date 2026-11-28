#include "data/FileSystem.hpp"

#include <cstdio>

#include "data/AssetStore.hpp"
#include "core/GameErrorContext.hpp"
#include "core/Supervisor.hpp"

u32 g_LastFileSize;

u8 *FileSystem::OpenFile(const char *filepath, i32 isExternalResource)
{
    SDL_IOStream *file;
    u8 *buf;
    i64 fsize;
    const char *filename;

    if (!isExternalResource)
    {
        // The asset container holds its files by bare name, so a path like "data/x.anm"
        // is looked up as "x.anm". That matches how the original archive was laid out.
        filename = filepath;
        for (const char *p = filepath; *p; p++)
        {
            if (*p == '/' || *p == '\\')
            {
                filename = p + 1;
            }
        }

        u32 size = 0;
        buf = AssetStore::Read(filename, &size);
        if (!buf)
        {
            g_GameErrorContext.Fatal("error : %s is not found in arcfile.\n", filename);
            return NULL;
        }
        Supervisor::DebugPrint("%s Decode ... \n", filename);
        g_LastFileSize = size;
        return buf;
    }
    Supervisor::DebugPrint("%s Load ... \n", filepath);
    file = SDL_IOFromFile(filepath, "rb");
    if (!file)
    {
        Supervisor::DebugPrint("error : %s is not found.\n", filepath);
        return NULL;
    }

    SDL_SeekIO(file, 0, SDL_IO_SEEK_END);
    fsize = SDL_TellIO(file);
    buf = (u8 *)malloc(fsize);
    if (!buf)
    {
        SDL_CloseIO(file);
        return NULL;
    }

    SDL_SeekIO(file, 0, SDL_IO_SEEK_SET);
    if (SDL_ReadIO(file, buf, fsize) != fsize)
    {
        SDL_CloseIO(file);
        return NULL;
    }
    g_LastFileSize = fsize;
    SDL_CloseIO(file);
    return buf;
}

i32 FileSystem::CheckFileExists(const char *file)
{
    SDL_IOStream *fp;

    fp = SDL_IOFromFile(FileSystem::GetPrefPath(file).c_str(), "rb");
    if (fp)
    {
        SDL_CloseIO(fp);
        return true;
    }
    return false;
}

i32 FileSystem::WriteDataToFile(const char *filename, const void *out, u32 bytesToWrite)
{
    SDL_IOStream *file;
    u32 bytesWritten;

    file = SDL_IOFromFile(FileSystem::GetPrefPath(filename).c_str(), "wb");
    if (!file)
    {
        Supervisor::DebugPrint("error : %s write error\n", filename);
        return -1;
    }

    bytesWritten = SDL_WriteIO(file, out, bytesToWrite);
    if (bytesToWrite != bytesWritten)
    {
        SDL_CloseIO(file);
        Supervisor::DebugPrint("error : %s write error\n", filename);
        return -2;
    }
    SDL_CloseIO(file);
    Supervisor::DebugPrint("%s write ...\n", filename);
    return 0;
}

std::string FileSystem::GetBasePath(const char *filename)
{
    // Everything the game reads sits next to the executable: the th07.data/ directory and the
    // music stream.
    const char *basePath = SDL_GetBasePath();
    if (basePath)
    {
        return std::string(basePath) + filename;
    }
    return std::string(filename);
}

std::string FileSystem::GetPrefPath(const char *filename)
{
    // Saves and the configuration live beside the executable too, which keeps a build
    // self-contained and lets several copies coexist.
    return GetBasePath(filename);
}
