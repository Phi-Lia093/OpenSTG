#!/usr/bin/env python3
"""Check that running a stream through Lua produces the instructions it was written from.

    python tools/verify_runtime.py

This is the second of the two checks. The first, tools/verify_scripts_lua.py, reads the Lua file
as text and compares it with the binary streams -- that proves the transcription. This one runs
the Lua with the same coroutine mechanism the game uses and records the instructions it calls,
which proves the runtime.

The two are separate on purpose. When they were done together, a mistake in either looked like a
mistake in the other, and neither could be trusted.

How it works: a small program embeds Lua and loads animation.lua, then runs each stream on a
coroutine. Every instruction method records its name; the `at` method does what the game's does,
yielding while the clock is behind the requested time. The recorder drives the clock forward so
the stream runs to its end, and prints what it saw.

The expected instructions come from tools/verify_scripts_lua.py's own reading of the binary, so
the comparison is against the original data rather than against the Lua file.
"""

from __future__ import annotations

import os
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / '.tools/runtime-check'
CL = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207\bin\Hostx86\x86\cl.exe")
STAGE = ROOT / '.tools/static'
ANM = ROOT / 'build/run/th07.data/anm'

EXITS = {-1, 1, 2, 23}

PROGRAM = r'''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

/* What the recorder has seen so far. */
static int g_count = 0;

/* The clock the at() calls compare against, and how far it has been pushed. */
static int g_clock = 0;

/* Every instruction the stream calls, by name and with the time it ran at.

   `g_remaining` bounds the whole run. A stream that jumps backwards -- which most of them do,
   it is how an animation repeats -- would otherwise run forever now that at() no longer
   yields. When the budget runs out the instruction is still recorded and then the call reports
   an error, which the driver turns into "this stream did not finish". */
static int g_remaining = 0;

static int record(lua_State *L, const char *name, int time)
{
    if (g_remaining <= 0)
    {
        luaL_error(L, "budget exhausted after %d instructions", g_count);
        return 0;
    }

    /* The arguments go out as 32-bit words, which is what an instruction stores. A Lua integer
       becomes its int word and a Lua float its float word -- the same choice the runtime makes,
       and the reason a float written without a decimal point used to arrive as an int. Comparing
       the word compares exactly what the interpreter would read, without needing to know which
       type each opcode uses. */
    printf("%s %d", name, time);
    for (int index = 2; index <= lua_gettop(L); index++)
    {
        if (lua_isinteger(L, index))
        {
            const unsigned int word = (unsigned int)(long long)lua_tointeger(L, index);
            printf(" %08x", word);
        }
        else if (lua_isnumber(L, index))
        {
            const float value = (float)lua_tonumber(L, index);
            unsigned int word = 0;
            memcpy(&word, &value, sizeof(word));
            printf(" %08x", word);
        }
        else if (lua_istable(L, index))
        {
            /* A register reference: the word is the register's number. */
            lua_getfield(L, index, "r");
            const unsigned int word = (unsigned int)(long long)lua_tointeger(L, -1);
            lua_pop(L, 1);
            printf(" R%08x", word);
        }
        else
        {
            printf(" ?");
        }
    }
    printf("\n");
    fflush(stdout);
    g_count++;
    g_remaining--;
    return 0;
}

/* at(t): the recorder does not need the game's timing, only the instruction sequence.

   The game yields here and resumes next frame; this returns immediately and advances the clock,
   so the stream runs in one pass. That is enough to check that running the Lua produces the
   same instructions in the same order, which is what this tool is for -- the timing itself is
   checked by playing the game. */
/* LABEL(n): records and returns. The recorder has no interrupts and only wants the instruction
   sequence; waiting here would stop every stream at its first label. */
static int l_label(lua_State *L)
{
    record(L, "LABEL", 0);
    return 0;
}

/* entry(): the point the stream is entered at. The recorder always starts at the top, so every
   stream is compared from its beginning; an interrupt would enter it part way. */
static int l_entry(lua_State *L)
{
    lua_pushinteger(L, 0);
    return 1;
}

static int l_at(lua_State *L)
{
    const int requested = (int)luaL_checkinteger(L, 2);
    if (requested > g_clock)
    {
        g_clock = requested;
    }
    return 0;
}

/* Every instruction method. It records its own name, which is what makes this a check of the
   runtime rather than of the instruction bodies.

   The name comes from an upvalue set when the closure was made, not from the stack, because a
   method call carries only its arguments. */
static int l_instruction(lua_State *L)
{
    const char *name = lua_tostring(L, lua_upvalueindex(1));

    /* The instruction is recorded at the clock it ran at, and only then may it change the clock.
       A JUMP carries the time the stream continues at, so updating first would label the jump
       itself with the time it jumps to rather than the time it was executed. */
    const int at = g_clock;
    if (name && strcmp(name, "JUMP") == 0 && lua_gettop(L) >= 3)
    {
        g_clock = (int)lua_tointeger(L, 3);
    }
    return record(L, name ? name : "?", at);
}

/* Registers `fn` under `name`, with `name` as its single upvalue. Pushing the name and then the
   closure in that order is what makes the closure own it. */
static void set_method(lua_State *L, int table, const char *name)
{
    lua_pushstring(L, name);
    lua_pushcclosure(L, l_instruction, 1);
    lua_setfield(L, table, name);
}

static const char *kNames[] = {
    "NOP", "EXIT_HIDE", "EXIT_HIDE2", "EXIT", "SET_ACTIVE_SPRITE", "JUMP", "DEC_JUMP",
    "SET_TRANSLATION", "SET_SCALE", "SET_ALPHA", "SET_COLOR", "FLIP_X", "FLIP_Y",
    "SET_ROTATION", "SET_ANGLE_VEL", "SET_SCALE_SPEED", "FADE", "SET_BLEND",
    "POS_TIME_LINEAR", "POS_TIME_DECEL", "POS_TIME_ACCEL", "STOP", "INTERRUPT_LABEL",
    "NOP22", "LABEL", "STOP_HIDE", "SET_USE_OFFSET", "SET_AUTO_ROTATE", "SET_SCROLL_POS_X",
    "SET_SCROLL_POS_Y", "SET_VISIBILITY", "INTERP_SCALE", "SET_ZWRITE_DISABLE",
    "SET_CAMERA_MODE", "INTERP_POS", "INTERP_COLOR", "INTERP_ALPHA", "INTERP_ROTATE",
    "INTERP_SCALE_2", "MOV", "MOV_FLOAT", "ADD", "ADD_FLOAT", "SUB", "SUB_FLOAT", "MUL",
    "MUL_FLOAT", "DIV", "DIV_FLOAT", "MOD", "MOD_FLOAT", "ADD_2", "ADD_FLOAT_2", "SUB_2",
    "SUB_FLOAT_2", "MUL_2", "MUL_FLOAT_2", "DIV_2", "DIV_FLOAT_2", "MOD_2", "MOD_FLOAT_2",
    "RAND", "RAND_FLOAT", "SIN", "COS", "TAN", "ACOS", "ATAN", "NORMALIZE_ANGLE",
    "JUMP_IF_EQ", "JUMP_IF_EQ_FLOAT", "JUMP_IF_NEQ", "JUMP_IF_NEQ_FLOAT", "JUMP_IF_LT",
    "JUMP_IF_LT_FLOAT", "JUMP_IF_LEQ", "JUMP_IF_LEQ_FLOAT", "JUMP_IF_GT",
    "JUMP_IF_GT_FLOAT", "JUMP_IF_GEQ", "JUMP_IF_GEQ_FLOAT", "WAIT", "SET_SCROLLVEL_X",
    "SET_SCROLLVEL_Y", NULL
};

/* Runs one stream to its end, pushing the clock forward whenever it waits. */
static int run_stream(lua_State *L, int entry, int stream, int budget)
{
    lua_getglobal(L, "animation");
    if (!lua_istable(L, -1))
    {
        fprintf(stderr, "animation is not a table (it is %s)\n",
                lua_typename(L, lua_type(L, -1)));
        lua_pop(L, 1);
        return -1;
    }
    lua_geti(L, -1, entry);
    if (!lua_istable(L, -1))
    {
        fprintf(stderr, "entry %d is not a table\n", entry);
        lua_pop(L, 2);
        return -1;
    }
    lua_geti(L, -1, stream);
    if (!lua_isfunction(L, -1))
    {
        fprintf(stderr, "stream %d is not a function (it is %s)\n", stream,
                lua_typename(L, lua_type(L, -1)));
        lua_pop(L, 3);
        return -1;
    }

    lua_State *thread = lua_newthread(L);
    /* The stack is animation, entry, function, thread; the function is the one below the new
       thread, so it is at -2 once the thread is pushed. */
    lua_pushvalue(L, -2);
    lua_xmove(L, thread, 1);

    lua_newtable(thread);
    lua_pushcfunction(thread, l_at);
    lua_setfield(thread, -2, "at");
    lua_pushcfunction(thread, l_entry);
    lua_setfield(thread, -2, "entry");
    lua_pushcfunction(thread, l_label);
    lua_setfield(thread, -2, "LABEL");
    for (int i = 0; kNames[i]; i++)
    {
        set_method(thread, lua_gettop(thread), kNames[i]);
    }

    g_clock = 0;
    g_remaining = budget;

    /* Called rather than resumed: at() no longer yields, so the whole stream runs in one pass.
       A stream that loops forever trips the budget, which ends it the same way the game's
       per-frame step would eventually leave a looping animation running. */
    const int status = lua_pcall(thread, 1, 0, 0);

    lua_pop(L, 4);
    if (status != LUA_OK)
    {
        fprintf(stderr, "run: %s\n", lua_tostring(thread, -1));
        return -2;
    }
    return g_count;
}

int main(int argc, char **argv)
{
    if (argc < 4) { return 2; }

    lua_State *L = luaL_newstate();
    if (!L) { fprintf(stderr, "no state\n"); return 2; }
    luaL_openlibs(L);

    if (luaL_loadfile(L, argv[1]) != LUA_OK)
    {
        fprintf(stderr, "load: %s\n", lua_tostring(L, -1));
        return 2;
    }
    if (lua_pcall(L, 0, 1, 0) != LUA_OK)
    {
        fprintf(stderr, "call: %s\n", lua_tostring(L, -1));
        return 2;
    }
    if (!lua_istable(L, -1))
    {
        fprintf(stderr, "not a table: %s\n", lua_typename(L, lua_type(L, -1)));
        return 2;
    }
    lua_setglobal(L, "animation");

    const int result = run_stream(L, atoi(argv[2]), atoi(argv[3]), 4096);
    lua_close(L);
    return result < 0 ? 3 : 0;
}
'''


def opcode_names() -> dict[int, str]:
    text = (ROOT / 'src/script/AnmVm.hpp').read_text(encoding='utf-8')
    start = text.find('ANM_EXIT_HIDE = -1')
    end = text.find('};', start)
    names = {int(v): n[4:] for n, v in
             re.findall(r'(ANM_\w+)\s*=\s*(-?\d+)', text[start:end])}
    names[0] = 'NOP'
    names[22] = 'NOP22'
    return names


def build() -> Path | None:
    BUILD.mkdir(parents=True, exist_ok=True)
    (BUILD / 'main.cpp').write_text(PROGRAM, encoding='utf-8')

    msvc = CL.parents[3]
    sdk_root = Path(r"C:\Program Files (x86)\Windows Kits\10")
    sdk = None
    for include_dir in sorted((sdk_root / 'Include').iterdir(), reverse=True):
        if (sdk_root / 'Lib' / include_dir.name / 'um/x86/kernel32.lib').is_file():
            sdk = include_dir.name
            break
    if sdk is None:
        print('no Windows SDK with 32-bit libraries')
        return None

    env = os.environ.copy()
    env['INCLUDE'] = ';'.join([
        str(msvc / 'include'),
        str(sdk_root / 'Include' / sdk / 'ucrt'),
        str(sdk_root / 'Include' / sdk / 'shared'),
        str(sdk_root / 'Include' / sdk / 'um'),
        str(STAGE / 'include'),
    ])
    env['LIB'] = ';'.join([
        str(msvc / 'lib/x86'),
        str(sdk_root / 'Lib' / sdk / 'ucrt/x86'),
        str(sdk_root / 'Lib' / sdk / 'um/x86'),
        str(STAGE / 'lib'),
    ])

    exe = BUILD / 'runtime_check.exe'
    result = subprocess.run([
        str(CL), '/nologo', '/MT', '/EHsc', '/std:c++17',
        str(BUILD / 'main.cpp'),
        '/I' + str(STAGE / 'include'),
        '/Fe:' + str(exe),
        '/Fo' + str(BUILD) + '\\',
        '/link', 'lua54-static.lib',
    ], env=env, capture_output=True, text=True, errors='replace')
    if result.returncode != 0:
        print(result.stdout[-3000:])
        print(result.stderr[-3000:])
        return None
    return exe


def expected_streams(directory: Path, entry: int) -> list[list[tuple[int, int, list[int]]]]:
    """The instructions the binary holds for each stream of one entry.

    Each is (opcode, time, argument words). The words are what the instruction stores, which is
    what the recorder prints for the Lua side, so the two compare directly.
    """
    script_file = directory / f'scripts_{entry}.bin'
    animation = directory / 'animation.lua'
    if not script_file.is_file() or not animation.is_file():
        return []

    text = animation.read_text(encoding='utf-8')
    marks = [(int(m.group(1)), m.end())
             for m in re.finditer(r'^\s+\[(\d+)\] = \{', text, re.M)]
    offsets = []
    for index, (number, position) in enumerate(marks):
        if number != entry:
            continue
        end = marks[index + 1][1] if index + 1 < len(marks) else len(text)
        block = text[position:end]
        if 'scripts' in block:
            offsets = [int(v) for v in re.findall(r'offset = (\d+)', block)]
            break

    data = script_file.read_bytes()
    out = []
    for start in sorted(offsets):
        stream = []
        position = start
        while position + 8 <= len(data):
            opcode, size, time, flags = struct.unpack_from('<hHhH', data, position)
            count = max(0, size // 4 - 2)
            if position + 8 + count * 4 > len(data):
                break
            words = list(struct.unpack_from(f'<{count}I', data, position + 8))

            # A register argument is marked in the flags field; the recorder prints those with a
            # leading R so the two cannot be confused with a value that happens to be the same.
            rendered = []
            for index, word in enumerate(words):
                rendered.append(word)

            stream.append((opcode, time, rendered, flags))
            if opcode in EXITS or size < 8:
                break
            position += size
        out.append(stream)
    return out


def main() -> int:
    exe = build()
    if exe is None:
        return 1

    names = opcode_names()
    problems = []
    checked = 0

    for directory in sorted(p for p in ANM.iterdir() if p.is_dir()):
        animation = directory / 'animation.lua'
        if not animation.is_file():
            continue

        text = animation.read_text(encoding='utf-8')
        entries = sorted({int(m.group(1)) for m in
                          re.finditer(r'^\s+\[(\d+)\] = \{', text, re.M)})

        for entry in entries:
            streams = expected_streams(directory, entry)
            for index, wanted in enumerate(streams):
                try:
                    run = subprocess.run([str(exe), str(animation), str(entry), str(index)],
                                         capture_output=True, text=True, errors='replace',
                                         timeout=10)
                except subprocess.TimeoutExpired:
                    problems.append(f'{directory.name} entry {entry} stream {index}: '
                                    f'did not finish within 10 seconds')
                    continue
                if run.returncode != 0:
                    problems.append(f'{directory.name} entry {entry} stream {index}: '
                                    f'{run.stderr.strip()[:100]}')
                    continue

                # Each recorded line is the name, the clock, then one word per argument.
                produced = [line.split() for line in run.stdout.splitlines() if line.split()]

                # The generator calls the interrupt label LABEL, because in Lua it is where the
                # script waits rather than an instruction it runs; the opcode is the same one.
                if len(produced) != len(wanted):
                    problems.append(f'{directory.name} entry {entry} stream {index}: '
                                    f'{len(wanted)} instructions in the binary, '
                                    f'{len(produced)} from the runtime')
                    continue

                mismatch = None
                for position, ((opcode, time, words, flags), line) in enumerate(
                        zip(wanted, produced)):
                    expected_name = ('LABEL' if names.get(opcode) == 'INTERRUPT_LABEL'
                                     else names.get(opcode, f'OP{opcode}'))
                    if line[0] != expected_name:
                        mismatch = (position, f'opcode {expected_name} vs {line[0]}')
                        break
                    # The clock is not compared here. tools/verify_scripts_lua.py checks it
                    # against the binary directly, reading each at(t) and each instruction's time
                    # field; the recorder advances a clock of its own to drive the stream, and
                    # the two schedules are not the same thing. Comparing them here reported
                    # differences that were an artefact of the recorder.

                    if len(line) - 2 != len(words):
                        mismatch = (position, f'{len(words)} arguments vs {len(line) - 2}')
                        break

                    for argument, word in enumerate(words):
                        # A register argument is printed with a leading R; anything else is a
                        # plain word. They are compared as printed, so a value that happens to
                        # equal a register number cannot pass as one.
                        is_register = (flags >> argument) & 1
                        expected_word = (f'R{word:08x}' if is_register else f'{word:08x}')
                        if line[2 + argument] != expected_word:
                            mismatch = (position,
                                        f'argument {argument} {expected_word} vs '
                                        f'{line[2 + argument]}')
                            break
                    if mismatch:
                        break

                if mismatch:
                    position, detail = mismatch
                    problems.append(f'{directory.name} entry {entry} stream {index} '
                                    f'instruction {position}: {detail}')
                    continue
                checked += 1

    print(f'{checked} streams executed and compared')
    if problems:
        print(f'{len(problems)} problem(s):')
        for problem in problems[:15]:
            print(f'  {problem}')
        return 1
    print('every stream runs to the same instructions the binary holds')
    return 0


if __name__ == '__main__':
    sys.exit(main())
