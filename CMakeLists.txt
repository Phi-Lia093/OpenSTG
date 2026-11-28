cmake_minimum_required(VERSION 3.20)

project(th07 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Builds 東方妖々夢 on Windows against desktop OpenGL.
#
# The upstream port also targeted Android, iOS, macOS and Emscripten through OpenGL ES. Those
# targets are gone, along with the GLES code paths: the configuration here is Windows only,
# with GL 3.3 core through SDL and the static SDL archives from .tools.
#
# The game's data sits next to the executable as th07.data/ and thbgm.dat. The icon, a
# version record and the text font are compiled into the executable as resources.

set(SOURCES
    src/audio/MidiOutput.cpp
    src/audio/SoundPlayer.cpp
    src/core/Chain.cpp
    src/core/Controller.cpp
    src/core/GameErrorContext.cpp
    src/core/GameWindow.cpp
    src/core/Rng.cpp
    src/core/Supervisor.cpp
    src/core/utils.cpp
    src/data/AssetStore.cpp
    src/data/ConfigFile.cpp
    src/data/EmbeddedAssets.cpp
    src/data/FileSystem.cpp
    src/data/ReplayManager.cpp
    src/data/Toml.cpp
    src/game/BombData.cpp
    src/game/BulletManager.cpp
    src/game/EffectManager.cpp
    src/game/EnemyManager.cpp
    src/game/GameManager.cpp
    src/game/ItemManager.cpp
    src/game/Player.cpp
    src/game/Stage.cpp
    src/main.cpp
    src/render/GL.cpp
    src/script/AnmManager.cpp
    src/script/AnmOp.cpp
    src/script/AnmScript.cpp
    src/script/AnmVmDebug.cpp
    src/script/AnmEntryFile.cpp
    src/script/AnmVm.cpp
    src/script/EclManager.cpp
    src/script/EnemyEclInstr.cpp
    src/support/Lzss.cpp
    src/support/sj2utf8/sj2utf8.cpp
    src/ui/AsciiManager.cpp
    src/ui/Ending.cpp
    src/ui/Gui.cpp
    src/ui/MainMenu.cpp
    src/ui/MusicRoom.cpp
    src/ui/ResultScreen.cpp
    src/ui/ScreenEffect.cpp
    src/ui/TextHelper.cpp
)

# The icon and the text font are embedded in the resource section. The font is looked for in
# .tools/ first, since that is where the original data is staged; a copy beside the
# executable still works as a fallback at runtime.
set(TH07_ICON "${CMAKE_SOURCE_DIR}/tools/th07_icon.ico")
if(NOT EXISTS "${TH07_ICON}")
    message(FATAL_ERROR "icon not found: ${TH07_ICON}")
endif()

set(TH07_FONT_CANDIDATES
    "${CMAKE_SOURCE_DIR}/.tools/msgothic.ttc"
    "C:/Windows/Fonts/msgothic.ttc")
set(TH07_FONT "")
foreach(candidate IN LISTS TH07_FONT_CANDIDATES)
    if(EXISTS "${candidate}")
        set(TH07_FONT "${candidate}")
        break()
    endif()
endforeach()
if(TH07_FONT STREQUAL "")
    message(FATAL_ERROR
        "msgothic.ttc not found. Put a copy in .tools/ to embed it in the executable.")
endif()

# The resource script is generated so the paths do not have to be hard-coded, and RC needs
# backslashes escaped for the string literals it receives.
file(TO_NATIVE_PATH "${TH07_ICON}" TH07_ICON_NATIVE)
file(TO_NATIVE_PATH "${TH07_FONT}" TH07_FONT_NATIVE)
string(REPLACE "\\" "\\\\" TH07_ICON_RC "${TH07_ICON_NATIVE}")
string(REPLACE "\\" "\\\\" TH07_FONT_RC "${TH07_FONT_NATIVE}")
configure_file("${CMAKE_SOURCE_DIR}/tools/th07.rc.in"
               "${CMAKE_BINARY_DIR}/th07.rc" @ONLY)

add_executable(th07 ${SOURCES} "${CMAKE_BINARY_DIR}/th07.rc")

# SDL is linked statically, so the executable needs no SDL DLLs beside it. The archives and
# headers come from tools/build_sdl_static.py, which builds them from source; the prebuilt
# MSVC packages ship import libraries only.
set(SDL_STATIC_ROOT "${CMAKE_SOURCE_DIR}/.tools/static")
if(NOT EXISTS "${SDL_STATIC_ROOT}/lib/SDL3-static.lib")
    message(FATAL_ERROR
        "static SDL libraries not found in ${SDL_STATIC_ROOT}.\n"
        "Run: python tools/build_sdl_static.py")
endif()

target_include_directories(th07 PRIVATE src "${SDL_STATIC_ROOT}/include")

# SDL3_STATIC selects the static declarations in SDL's headers.
target_compile_definitions(th07 PRIVATE SDL_STATIC)

# Link the C and C++ runtime statically, matching how the SDL archives were built. Without
# this the executable needs MSVCP140 and the UCRT DLLs beside it, which defeats the point
# of linking SDL statically.
set_property(TARGET th07 PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(OpenGL REQUIRED)

target_link_directories(th07 PRIVATE "${SDL_STATIC_ROOT}/lib")
target_link_libraries(th07 PRIVATE
    SDL3_ttf-static.lib
    SDL3_image-static.lib
    SDL3-static.lib
    # Lua 5.4, built as a static library by tools/build_lua_static.py. The animations are
    # described and scripted in Lua, so it is part of the program rather than a plugin.
    lua54-static.lib
    OpenGL::GL
    # Win32 libraries SDL3 and FreeType reference when built statically.
    winmm.lib
    version.lib
    setupapi.lib
    imm32.lib
    oleaut32.lib
    ole32.lib
    uuid.lib
    user32.lib
    gdi32.lib
    advapi32.lib
    shell32.lib
    kernel32.lib
    # dbghelp walks the stack in the crash handler.
    dbghelp.lib
)
