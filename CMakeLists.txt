cmake_minimum_required(VERSION 3.20)

project(th07 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Builds 東方妖々夢 on Windows against desktop OpenGL.
#
# The upstream port also targets Android, iOS, macOS and Emscripten. Those require the
# vendored SDL submodules and each platform's own toolchain, and none of them are part of
# this build, so the configuration here covers Windows only: GL 3.3 core through SDL,
# linked against opengl32, with the SDL packages taken from .tools.
#
# Assets (th07.dat, thbgm.dat, msgothic.ttc) sit next to the executable rather than being
# embedded, which is what TH_EXTERNAL_ASSETS selects.

set(SOURCES
    src/AnmManager.cpp
    src/AnmVm.cpp
    src/AsciiManager.cpp
    src/BombData.cpp
    src/BulletManager.cpp
    src/Chain.cpp
    src/Controller.cpp
    src/EclManager.cpp
    src/EffectManager.cpp
    src/Ending.cpp
    src/EnemyEclInstr.cpp
    src/EnemyManager.cpp
    src/FileSystem.cpp
    src/GameErrorContext.cpp
    src/GameManager.cpp
    src/GameWindow.cpp
    src/Gui.cpp
    src/ItemManager.cpp
    src/MainMenu.cpp
    src/MidiOutput.cpp
    src/MusicRoom.cpp
    src/Player.cpp
    src/ReplayManager.cpp
    src/ResultScreen.cpp
    src/Rng.cpp
    src/ScreenEffect.cpp
    src/SoundPlayer.cpp
    src/Stage.cpp
    src/Supervisor.cpp
    src/TextHelper.cpp
    src/Touch.cpp
    src/main.cpp
    src/graphics/Gles.cpp
    src/pbg4/Lzss.cpp
    src/pbg4/Pbg4Archive.cpp
    src/pbg4/Pbg4File.cpp
    src/sj2utf8/sj2utf8.cpp
    src/utils.cpp
)

add_executable(th07 ${SOURCES})

target_include_directories(th07 PRIVATE src src/pbg4)

# Assets are read from the working directory at runtime.
target_compile_definitions(th07 PRIVATE TH_EXTERNAL_ASSETS)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

# The shaders and the context setup switch on USING_GL for a desktop core profile.
target_compile_definitions(th07 PRIVATE USING_GL)

target_link_libraries(th07 PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    OpenGL::GL
)
