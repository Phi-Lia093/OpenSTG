-- std4txt: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 256, format = 1,
    sprites = {
      { id = 0, rect = { 1, 1, 254, 46 } },
      { id = 1, rect = { 0, 48, 96, 32 } },
      { id = 2, rect = { 256, 0, 256, 48 } },
      { id = 3, rect = { 0, 80, 384, 24 } },
      { id = 4, rect = { 0, 104, 384, 24 } },
      { id = 5, rect = { 0, 128, 216, 128 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 92 },
      { index = 2, offset = 232 },
      { index = 3, offset = 352 },
      { index = 4, offset = 472 },
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
        s:SET_TRANSLATION(1129316352, 1130364928, 0)
        s:SET_ALPHA(0)
        s:at(1700)
        s:FADE(255, 80)
        s:at(1970)
        s:FADE(0, 60)
        s:at(2030)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ROTATION(0, 0, 1070141403)
        s:SET_TRANSLATION(1121910784, 1130332160, 0)
        s:SET_ALPHA(0)
        s:at(1670)
        s:FADE(255, 60)
        s:INTERP_POS(120, 0, 1127186432, 1130332160, 0)
        s:at(1970)
        s:FADE(0, 60)
        s:at(2030)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_TRANSLATION(1124073472, 1111490560, 0)
        s:SET_ALPHA(0)
        s:at(1610)
        s:FADE(255, 60)
        s:INTERP_POS(120, 0, 1124073472, 1126170624, 0)
        s:at(1810)
        s:FADE(0, 60)
        s:at(1870)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_TRANSLATION(1136132096, 1133510656, 0)
        s:SET_ALPHA(0)
        s:at(1640)
        s:FADE(255, 60)
        s:INTERP_POS(120, 0, 1132462080, 1133510656, 0)
        s:at(1840)
        s:FADE(0, 60)
        s:at(1900)
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
        s:at(460)
        s:FADE(0, 60)
        s:at(520)
        s:EXIT_HIDE2()
    end,
  },
}
