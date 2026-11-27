#pragma once

#include <cstddef>

#include "inttypes.hpp"

struct SDL_IOStream;

// Reading binaries embedded in the executable's resource section.
//
// The font the game draws text with is 8.8 MB, and shipping it as a loose file next to the
// executable makes the install look unfinished. It goes into the resource section instead,
// which is the ordinary way for a Windows program to carry read-only data: no extraction
// step, no external file to lose, and the OS loads it only when asked for.
//
// The resource is declared in the generated .rc file; see CMakeLists.txt.
namespace EmbeddedAssets
{
// Locates a resource by the name passed to the .rc file. Returns false when it is absent,
// which is the case for a build configured without it.
bool Find(const wchar_t *name, const void **data, u32 *size);

// The font used for all text, or null when it is not embedded. The pointer stays valid for
// the life of the process: it points into the loaded image, not into a copy.
const void *FontData();
u32 FontSize();

// A read stream over the font. Uses the embedded copy when there is one and falls back to
// the file next to the executable. Returns null when neither is available; the caller
// closes the stream.
SDL_IOStream *OpenFontStream();
} // namespace EmbeddedAssets
