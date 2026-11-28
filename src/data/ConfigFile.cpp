#include "data/ConfigFile.hpp"

#include <cstdio>
#include <cstring>
#include <string>

#include "data/Toml.hpp"

#ifdef TH_CONFIG_TEST
// The configuration tests build this file without SDL so they can run standalone. Logging
// and file access fall back to the C library, which is all this file uses them for.
#include <cstdarg>
namespace
{
void LogMessage(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}
} // namespace
#define SDL_Log LogMessage
#else
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>
#endif

namespace ConfigFile
{
namespace
{
// --- reading --------------------------------------------------------------------

// Reads an integer field, clamping into [minimum, maximum]. A missing key leaves `value`
// alone; a present but out-of-range one is reported and clamped, which keeps a typo from
// discarding the rest of the file.
void ReadInt(const Toml::Document &doc, const char *key, long long &value, long long minimum,
             long long maximum)
{
    const Toml::Value *entry = doc.Get(key);
    if (!entry)
    {
        return;
    }
    if (entry->type != Toml::ValueType::Integer)
    {
        SDL_Log("config: %s must be an integer, using %lld", key, value);
        return;
    }
    long long read = entry->integer;
    if (read < minimum || read > maximum)
    {
        SDL_Log("config: %s = %lld is outside %lld..%lld, clamping", key, read, minimum,
                maximum);
        read = read < minimum ? minimum : maximum;
    }
    value = read;
}

// Reads a boolean, leaving `value` alone when the key is absent or malformed. Returned by
// value because the option flags are bitfields, which cannot be bound to a reference.
u32 ReadBool(const Toml::Document &doc, const char *key, u32 value)
{
    const Toml::Value *entry = doc.Get(key);
    if (!entry)
    {
        return value;
    }
    if (entry->type != Toml::ValueType::Boolean)
    {
        SDL_Log("config: %s must be true or false, keeping %s", key, value ? "true" : "false");
        return value;
    }
    return entry->boolean ? 1u : 0u;
}

void ReadBool(const Toml::Document &doc, const char *key, u8 &value)
{
    value = (u8)ReadBool(doc, key, value ? 1u : 0u);
}

void ReadI16(const Toml::Document &doc, const char *key, i16 &value)
{
    long long read = value;
    ReadInt(doc, key, read, -32768, 32767);
    value = (i16)read;
}

// Reads a named choice. `names` is null-terminated. An unknown name is reported and the
// current value kept.
void ReadChoice(const Toml::Document &doc, const char *key, u8 &value,
                const char *const *names)
{
    const Toml::Value *entry = doc.Get(key);
    if (!entry)
    {
        return;
    }
    if (entry->type != Toml::ValueType::String)
    {
        SDL_Log("config: %s must be a string, keeping %d", key, (int)value);
        return;
    }
    for (int i = 0; names[i]; i++)
    {
        if (entry->text == names[i])
        {
            value = (u8)i;
            return;
        }
    }
    SDL_Log("config: %s = \"%s\" is not a known value, keeping %d", key,
            entry->text.c_str(), (int)value);
}

const char *const kDifficultyNames[] = {"easy", "normal", "hard", "lunatic", "extra",
                                        "phantasm", NULL};
const char *const kQualityNames[] = {"worst", "medium", "beautiful", NULL};
const char *const kMusicNames[] = {"off", "wav", "midi", NULL};

// --- writing --------------------------------------------------------------------

std::string Quote(const char *text)
{
    std::string out = "\"";
    for (const char *p = text; *p; p++)
    {
        if (*p == '"' || *p == '\\')
        {
            out.push_back('\\');
        }
        out.push_back(*p);
    }
    out.push_back('"');
    return out;
}

// Writes `key = value`, padded so that comments line up and there is never a trailing space
// when there is no comment.
void WriteField(std::string &out, const char *key, const std::string &value,
                const char *comment)
{
    char head[128];
    snprintf(head, sizeof(head), "%-22s = ", key);
    std::string line = head;
    line += value;
    if (comment && *comment)
    {
        // Align at a fixed column where the value is short enough, and always leave at
        // least one space so a long value cannot run into its comment.
        while (line.size() < 32)
        {
            line.push_back(' ');
        }
        if (line.back() != ' ')
        {
            line.push_back(' ');
        }
        line += "# ";
        line += comment;
    }
    line += "\n";
    out += line;
}

void WriteInt(std::string &out, const char *key, long long value, const char *comment)
{
    WriteField(out, key, std::to_string(value), comment);
}

void WriteBool(std::string &out, const char *key, u8 value, const char *comment)
{
    WriteField(out, key, value ? "true" : "false", comment);
}

void WriteBool(std::string &out, const char *key, u32 value, const char *comment)
{
    WriteField(out, key, value ? "true" : "false", comment);
}

void WriteChoice(std::string &out, const char *key, u8 value, const char *const *names,
                 const char *comment)
{
    int count = 0;
    while (names[count])
    {
        count++;
    }
    const char *chosen = value < count ? names[value] : names[0];
    WriteField(out, key, std::string("\"") + chosen + "\"", comment);
}

} // namespace

bool Load(const char *path, GameConfiguration &config)
{
#ifdef TH_CONFIG_TEST
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        return true;
    }
    std::string text;
    char buffer[4096];
    size_t read;
    while ((read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        text.append(buffer, read);
    }
    fclose(file);
#else
    SDL_IOStream *file = SDL_IOFromFile(path, "rb");
    if (!file)
    {
        return true; // Absent is not an error; the caller's defaults stand.
    }

    const Sint64 length = SDL_GetIOSize(file);
    std::string text;
    if (length > 0)
    {
        text.resize((size_t)length);
        const size_t read = SDL_ReadIO(file, &text[0], (size_t)length);
        text.resize(read);
    }
    SDL_CloseIO(file);
#endif

    const bool ok = LoadText(text.c_str(), config);
    if (!ok)
    {
        SDL_Log("config: %s could not be parsed; keeping defaults", path);
    }
    return ok;
}

bool LoadText(const char *text, GameConfiguration &config)
{
    Toml::Document doc;
    std::string error;
    if (!Toml::Parse(text ? text : "", doc, error))
    {
        SDL_Log("config: %s", error.c_str());
        return false;
    }

    const Toml::Value *controller = doc.Get("input.shot");
    if (controller)
    {
        ReadI16(doc, "input.shot", config.controllerMapping.shootButton);
        ReadI16(doc, "input.bomb", config.controllerMapping.bombButton);
        ReadI16(doc, "input.focus", config.controllerMapping.focusButton);
        ReadI16(doc, "input.menu", config.controllerMapping.menuButton);
        ReadI16(doc, "input.up", config.controllerMapping.upButton);
        ReadI16(doc, "input.down", config.controllerMapping.downButton);
        ReadI16(doc, "input.left", config.controllerMapping.leftButton);
        ReadI16(doc, "input.right", config.controllerMapping.rightButton);
        ReadI16(doc, "input.skip", config.controllerMapping.skipButton);
    }

    long long lives = config.lifeCount;
    ReadInt(doc, "game.lives", lives, 0, 4);
    config.lifeCount = (u8)lives;

    long long bombs = config.bombCount;
    ReadInt(doc, "game.bombs", bombs, 0, 3);
    config.bombCount = (u8)bombs;

    ReadChoice(doc, "game.default_difficulty", config.defaultDifficulty, kDifficultyNames);
    config.playSounds = ReadBool(doc, "game.play_sounds", config.playSounds ? 1u : 0u);
    config.windowed = ReadBool(doc, "game.windowed", config.windowed ? 1u : 0u);

    long long frameskip = config.frameskipConfig;
    ReadInt(doc, "game.frameskip", frameskip, 0, 2);
    config.frameskipConfig = (u8)frameskip;

    ReadChoice(doc, "game.effect_quality", config.effectQuality, kQualityNames);
    config.slowMode = ReadBool(doc, "game.slow_mode", config.slowMode ? 1u : 0u);
    config.shotSlow = ReadBool(doc, "game.shot_slow", config.shotSlow ? 1u : 0u);
    ReadChoice(doc, "game.music_mode", config.musicMode, kMusicNames);
    ReadI16(doc, "game.pad_axis_x", config.padAxisX);
    ReadI16(doc, "game.pad_axis_y", config.padAxisY);

    config.colorMode16bit = ReadBool(doc, "graphics.color_mode_16bit", config.colorMode16bit ? 1u : 0u);
    config.disableFog = ReadBool(doc, "graphics.disable_fog", config.disableFog ? 1u : 0u);
    config.disableZBuffer = ReadBool(doc, "graphics.disable_z_buffer", config.disableZBuffer ? 1u : 0u);
    config.disableGouraud = ReadBool(doc, "graphics.disable_gouraud", config.disableGouraud ? 1u : 0u);
    config.disableTextureBlend = ReadBool(doc, "graphics.disable_texture_blend", config.disableTextureBlend ? 1u : 0u);
    config.colorAddEmulation = ReadBool(doc, "graphics.color_add_emulation", config.colorAddEmulation ? 1u : 0u);
    config.use16BitTextures = ReadBool(doc, "graphics.use_16bit_textures", config.use16BitTextures ? 1u : 0u);
    config.noVertexBuffers = ReadBool(doc, "graphics.no_vertex_buffers", config.noVertexBuffers ? 1u : 0u);
    config.forceBackBufferClear = ReadBool(doc, "graphics.force_back_buffer_clear", config.forceBackBufferClear ? 1u : 0u);
    ReadBool(doc, "graphics.disable_item_draw_around_playfield",
             config.disableItemDrawAroundPlayfield);
    config.forceReferenceRender = ReadBool(doc, "graphics.force_reference_render", config.forceReferenceRender ? 1u : 0u);
    config.redrawEveryFrame = ReadBool(doc, "graphics.redraw_every_frame", config.redrawEveryFrame ? 1u : 0u);

    config.preloadBgm = ReadBool(doc, "audio.preload_bgm", config.preloadBgm ? 1u : 0u);

    config.disableDinput = ReadBool(doc, "input.disable_directinput", config.disableDinput ? 1u : 0u);

    config.disableVsync = ReadBool(doc, "display.disable_vsync", config.disableVsync ? 1u : 0u);

    // The version field only existed to detect a struct written by another build. The file
    // is now self-describing, so it is pinned to the value the rest of the code expects.
    config.version = 0x70002;
    return true;
}

std::string Serialize(const GameConfiguration &config)
{
    std::string out;
    out += "# Touhou Youyoumu configuration.\n";
    out += "#\n";
    out += "# Delete this file to reset every setting. Fields that are absent fall back to\n";
    out += "# their default, so a partial file is valid.\n";
    out += "\n";
    out += "[input]\n";
    out += "# Controller button indices. -1 means unmapped.\n";
    WriteInt(out, "shot", config.controllerMapping.shootButton, "");
    WriteInt(out, "bomb", config.controllerMapping.bombButton, "");
    WriteInt(out, "focus", config.controllerMapping.focusButton, "");
    WriteInt(out, "menu", config.controllerMapping.menuButton, "");
    WriteInt(out, "up", config.controllerMapping.upButton, "");
    WriteInt(out, "down", config.controllerMapping.downButton, "");
    WriteInt(out, "left", config.controllerMapping.leftButton, "");
    WriteInt(out, "right", config.controllerMapping.rightButton, "");
    WriteInt(out, "skip", config.controllerMapping.skipButton, "");
    WriteBool(out, "disable_directinput", config.disableDinput, "");

    out += "\n[game]\n";
    WriteInt(out, "lives", config.lifeCount, "0..4");
    WriteInt(out, "bombs", config.bombCount, "0..3");
    WriteChoice(out, "default_difficulty", config.defaultDifficulty, kDifficultyNames,
                "easy normal hard lunatic extra phantasm");
    WriteBool(out, "play_sounds", config.playSounds, "");
    WriteBool(out, "windowed", config.windowed, "");
    WriteInt(out, "frameskip", config.frameskipConfig, "0..2");
    WriteChoice(out, "effect_quality", config.effectQuality, kQualityNames,
                "worst medium beautiful");
    WriteBool(out, "slow_mode", config.slowMode, "");
    WriteBool(out, "shot_slow", config.shotSlow, "");
    WriteChoice(out, "music_mode", config.musicMode, kMusicNames, "off wav midi");
    WriteInt(out, "pad_axis_x", config.padAxisX, "");
    WriteInt(out, "pad_axis_y", config.padAxisY, "");

    out += "\n[graphics]\n";
    WriteBool(out, "color_mode_16bit", config.colorMode16bit, "");
    WriteBool(out, "disable_fog", config.disableFog, "");
    WriteBool(out, "disable_z_buffer", config.disableZBuffer, "");
    WriteBool(out, "disable_gouraud", config.disableGouraud, "");
    WriteBool(out, "disable_texture_blend", config.disableTextureBlend, "");
    WriteBool(out, "color_add_emulation", config.colorAddEmulation, "");
    WriteBool(out, "use_16bit_textures", config.use16BitTextures, "");
    WriteBool(out, "no_vertex_buffers", config.noVertexBuffers, "");
    WriteBool(out, "force_back_buffer_clear", config.forceBackBufferClear, "");
    WriteBool(out, "disable_item_draw_around_playfield",
              config.disableItemDrawAroundPlayfield, "");
    WriteBool(out, "force_reference_render", config.forceReferenceRender, "");
    WriteBool(out, "redraw_every_frame", config.redrawEveryFrame, "");

    out += "\n[audio]\n";
    WriteBool(out, "preload_bgm", config.preloadBgm, "");

    out += "\n[display]\n";
    WriteBool(out, "disable_vsync", config.disableVsync, "");

    return out;
}

bool Save(const char *path, const GameConfiguration &config)
{
    const std::string text = Serialize(config);

#ifdef TH_CONFIG_TEST
    FILE *file = fopen(path, "wb");
    if (!file)
    {
        SDL_Log("config: cannot write %s", path);
        return false;
    }
    const size_t written = fwrite(text.data(), 1, text.size(), file);
    fclose(file);
#else
    SDL_IOStream *file = SDL_IOFromFile(path, "wb");
    if (!file)
    {
        SDL_Log("config: cannot write %s: %s", path, SDL_GetError());
        return false;
    }
    const size_t written = SDL_WriteIO(file, text.data(), text.size());
    SDL_CloseIO(file);
#endif
    if (written != text.size())
    {
        SDL_Log("config: short write to %s", path);
        return false;
    }
    return true;
}

} // namespace ConfigFile
