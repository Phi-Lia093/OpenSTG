# Animations

The animations are Lua. There is no `.anm` file, no instruction buffer, and no interpreter
walking one: a script is a Lua function, and running it is a coroutine that the game resumes once
a frame.

## What is on disk

    assets/anm/<name>/
        animation.lua     the description, the sprites, and every script
        texture_0.png     the textures, one per entry that has one
        texture_1.png
        ...

Everything under `assets/` is source. It is read in preference to `th07.data/`, which holds the
game's own data unchanged; the asset store tries `assets/` first and falls back, so a converted
asset shadows one of the same name in the archive and the rest of the game reads exactly as it
always did.

`animation.lua` returns a table keyed by entry:

```lua
return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 3,
    sprites = {
      { id = 0, rect = { 0, 0, 384, 448 } },
    },
    scripts = {
      { index = 0, offset = 0 },
    },
    labels = {
      [0] = { 2, 5, 3, -1 },
    },

    [0] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        ::L2::
        s:LABEL(2, 0)
        s:FADE(255, 16)
        s:at(60)
        s:STOP()
        ...
    end,
  },
}
```

The entry table carries four things for the loader: which texture, how big the sprite sheet is,
where each sprite is cut from it, and which slots the scripts occupy. `scripts[].index` is the
number the game uses to reach the stream; `scripts[].offset` is where the stream began in the
original encoding and is kept only so the description can be compared against it.

`offset` is not read by the game. It is what makes the description checkable: the same number
locates the stream in the original data, so the Lua and the binary can be compared instruction by
instruction. Nothing here needs it to run.

## How it is produced

`assets/anm/` is built from the original `*.anm` files, and that is the only source it has:

    python tools/build_anm_data.py

It runs eight steps in a fixed order. The order is not a preference:

| step | writes |
| --- | --- |
| `extract_textures` | `th07.data/*.png` |
| `extract_sprites` | `th07.data/*.spr.toml` |
| `organize_anm` | `th07.data/anm/<name>/`, clearing it first |
| `extract_scripts` | `th07.data/anm/<name>/scripts_N.bin` |
| `write_entry_toml` | `th07.data/anm/<name>/N.toml` |
| `merge_entry_toml` | folds the sprite table into `N.toml` |
| `toml_to_lua` | `th07.data/anm/<name>/animation.lua` |
| `scripts_to_lua` | the streams, appended to that file |

`organize_anm` clears `anm/` before it starts, so anything that belongs there must be written
after it. Run `extract_scripts` before `organize_anm`, as the name order suggests, and its files
are deleted by the step that follows and the result is an animation with no scripts -- which looks
like a conversion that worked.

`write_entry_toml` records the offsets it finds, and they are only right after `extract_scripts`
has rebased them to the script file. Those two therefore run once, in that order, and neither is
re-run on a directory that has already been through them.

`extract_textures` appends to each `*.anm` in place. Running it twice appends twice and every
sprite rectangle after the first pass is read from the wrong offset, which is why the pipeline
starts by re-extracting the archive rather than by reusing what is in `th07.data/`.

The `.toml` and `.bin` files the steps pass to each other are removed at the end; they are not
something the game reads.

## How a script runs

A script is a function of `s`, and every instruction is a method call on `s`:

| call | what it does |
|---|---|
| `s:at(t)` | waits until the VM's clock reaches `t` |
| `s:SET_ACTIVE_SPRITE(n)` and the rest | the instruction, by name |
| `s:LABEL(n, t)` | an interrupt label and the clock it starts at |
| `s:SET_CLOCK(t, offset)` | the clock half of a jump |
| `goto J<offset>` | the jump itself |
| `s:entry()` | where this run of the stream begins |

`at(t)` is the one that is not an instruction. The interpreter asked, before each step, whether
the next instruction's time had come; here the coroutine asks the same question and yields until
the answer is yes. It has to be a `lua_yieldk` with a continuation: a C function that yields with
plain `lua_yield` does not run again when it is resumed, so the wait would last one frame and the
stream would carry on past it.

A jump carries two things -- where it goes and what the clock becomes -- so it is written as
`SET_CLOCK` followed by `goto`. The pair is emitted together and must stay together: a jump that
moves without setting the clock leaves the next `at(t)` already satisfied and the stream spins.

An interrupt resumes a stream at a label. The interpreter found the label by scanning the
instruction buffer; here the stream is a coroutine, so it is built again with the label as its
entry point and the dispatch at the top jumps there. `labels` in the entry table says which
numbers a stream has, which is what lets an interrupt no label accepts fall back to `-1` as the
interpreter did.

Three outcomes are not 0-or-1, because the frame differs:

| outcome | what happens |
|---|---|
| the stream ended | the per-frame update still runs, as the interpreter's `stop:` did |
| the frame reached a time it waits for | the clock advances |
| an interrupt arrived | the clock does not advance, because the interpreter left the loop before the increment |

## What the interpreter still does

`AnmOp::Run` holds every instruction body. The runtime calls it with the same `AnmRawInstr` the
interpreter built from the buffer, so there is one implementation of each instruction and the two
forms could not drift apart. `ExecuteScript` no longer interprets anything: it runs the coroutine
and then advances the interpolations, rotations and scrolls that every VM needs whatever ran it.

## The switches

Lua is the path the game takes. Two environment variables go the other way, for comparing the two
forms on the same machine:

    TH07_ANM_LUA_OFF=1        every slot takes the binary path
    TH07_ANM_LUA_SLOT=n       only slot n does
    TH07_ANM_LUA_FIRST/_LAST  only that range does

The binary path survives only as far as the comparison goes: a slot with a script is answered by
the record the loader made, not by a pointer into a buffer, so the binary form has no data of its
own to run. Removing these switches is a small change when they are no longer wanted.

## Debugging

`TH07_ANM_DEBUG=1` makes every VM write a line at the end of each frame to `anmvm.txt`: slot,
whether it is visible, which sprite and texture it resolved, position, the position the renderer
interpolates from, scale, alpha, the script clock, whether an interrupt is pending, whether the
script is stopped, and its instruction pointer. That is the state a VM debugger would show, and it
is how the faults above were found -- none of them were visible from outside, and reading the code
only produced guesses.

The dump is bounded to 20000 lines. With the variable unset, nothing is written.
