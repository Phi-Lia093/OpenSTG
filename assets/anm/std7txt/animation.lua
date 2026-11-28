-- std7txt: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 256, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 48 } },
      { id = 1, rect = { 0, 48, 128, 32 } },
      { id = 2, rect = { 256, 0, 256, 48 } },
      { id = 3, rect = { 0, 80, 384, 24 } },
      { id = 4, rect = { 0, 104, 384, 24 } },
      { id = 5, rect = { 1, 129, 254, 126 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 152 },
      { index = 2, offset = 264 },
      { index = 3, offset = 384 },
      { index = 4, offset = 504 },
    },
    labels = {
      [0] = {  },
      [1] = {  },
      [2] = {  },
      [3] = {  },
      [4] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1126170624, 1132462080, 0)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ROTATION(0, 0, 1070141403)
        s:SET_ALPHA(0)
        s:at(200)
        s:FADE(200, 140)
        s:INTERP_POS(200, 0, 1126170624, 1130364928, 0)
        s:at(630)
        s:FADE(0, 140)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ROTATION(0, 0, 1070141403)
        s:SET_TRANSLATION(1127186432, 1130332160, 0)
        s:SET_ALPHA(0)
        s:at(260)
        s:FADE(255, 160)
        s:at(630)
        s:FADE(0, 140)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_TRANSLATION(1124073472, 1124073472, 0)
        s:SET_ALPHA(0)
        s:at(110)
        s:FADE(255, 60)
        s:INTERP_POS(120, 0, 1124073472, 1126170624, 0)
        s:at(430)
        s:FADE(0, 60)
        s:at(490)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_TRANSLATION(1136132096, 1133510656, 0)
        s:SET_ALPHA(0)
        s:at(140)
        s:FADE(255, 60)
        s:INTERP_POS(120, 0, 1133510656, 1133510656, 0)
        s:at(460)
        s:FADE(0, 60)
        s:at(520)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_ROTATION(0, 0, 1070141403)
        s:SET_TRANSLATION(1137295360, 1142677504, 0)
        s:SET_ALPHA(0)
        s:at(200)
        s:FADE(255, 60)
        s:INTERP_POS(120, 4, 1137295360, 1131380736, 0)
        s:at(400)
        s:FADE(0, 60)
        s:at(460)
        s:EXIT_HIDE2()
    end,
  },
}
