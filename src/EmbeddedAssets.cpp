#include "EmbeddedAssets.hpp"

#include <SDL3/SDL_iostream.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "FileSystem.hpp"

namespace EmbeddedAssets
{
namespace
{
#ifdef _WIN32
const void *g_FontData = nullptr;
u32 g_FontSize = 0;
bool g_FontProbed = false;

// Loads and pins a resource so its address stays valid. FindResource only locates it; the
// data is not resident until it is loaded, and the pointer is into the module image.
bool LoadResource(const wchar_t *name, const void **data, u32 *size)
{
    // RT_RCDATA is declared as an ANSI resource type; MAKEINTRESOURCEW converts it to the
    // wide form FindResourceW expects.
    HRSRC resource = FindResourceW(GetModuleHandleW(nullptr), name,
                                  MAKEINTRESOURCEW(10 /* RT_RCDATA */));
    if (!resource)
    {
        return false;
    }
    HGLOBAL loaded = LoadResource(GetModuleHandleW(nullptr), resource);
    if (!loaded)
    {
        return false;
    }
    const void *pointer = LockResource(loaded);
    const DWORD length = SizeofResource(GetModuleHandleW(nullptr), resource);
    if (!pointer || length == 0)
    {
        return false;
    }
    *data = pointer;
    *size = (u32)length;
    return true;
}
#endif
} // namespace

bool Find(const wchar_t *name, const void **data, u32 *size)
{
#ifdef _WIN32
    if (!name || !data || !size)
    {
        return false;
    }
    return LoadResource(name, data, size);
#else
    (void)name;
    (void)data;
    (void)size;
    return false;
#endif
}

const void *FontData()
{
#ifdef _WIN32
    if (!g_FontProbed)
    {
        g_FontProbed = true;
        LoadResource(L"TH07_FONT", &g_FontData, &g_FontSize);
    }
    return g_FontData;
#else
    return nullptr;
#endif
}

u32 FontSize()
{
    FontData();
#ifdef _WIN32
    return g_FontSize;
#else
    return 0;
#endif
}

SDL_IOStream *OpenFontStream()
{
    if (FontData() && FontSize() > 0)
    {
        // The pointer is into the module image, which lives for the whole process, so the
        // stream can read it in place rather than copying 8.8 MB.
        return SDL_IOFromConstMem(FontData(), FontSize());
    }
    return SDL_IOFromFile(FileSystem::GetBasePath("msgothic.ttc").c_str(), "rb");
}

} // namespace EmbeddedAssets
