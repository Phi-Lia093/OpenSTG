-- std6txt: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 48 } },
      { id = 1, rect = { 128, 128, 128, 64 } },
      { id = 2, rect = { 256, 0, 256, 48 } },
      { id = 3, rect = { 0, 80, 384, 24 } },
      { id = 4, rect = { 0, 104, 384, 24 } },
      { id = 5, rect = { 0, 128, 128, 128 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 132 },
      { index = 2, offset = 244 },
      { index = 3, offset = 364 },
      { index = 4, offset = 484 },
      { index = 5, offset = 624 },
    },
    labels = {
      [0] = {  },
      [1] = {  },
      [2] = {  },
      [3] = {  },
      [4] = {  },
      [5] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1130364928, 1131413504, 0)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:at(160)
        s:FADE(255, 140)
        s:INTERP_POS(200, 1, 1130364928, 1131413504, 0)
        s:at(630)
        s:FADE(0, 140)
        s:at(770)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ROTATION(0, 0, 1070141403)
        s:SET_TRANSLATION(1130364928, 1130332160, 0)
        s:SET_ALPHA(0)
        s:at(430)
        s:FADE(255, 120)
        s:at(730)
        s:FADE(0, 100)
        s:at(830)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_TRANSLATION(1124073472, 1126170624, 0)
        s:SET_ALPHA(0)
        s:at(500)
        s:FADE(255, 60)
        s:INTERP_POS(180, 0, 1126170624, 1126170624, 0)
        s:at(750)
        s:FADE(0, 60)
        s:at(810)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_TRANSLATION(1134755840, 1133510656, 0)
        s:SET_ALPHA(0)
        s:at(500)
        s:FADE(255, 60)
        s:INTERP_POS(180, 0, 1132462080, 1133510656, 0)
        s:at(850)
        s:FADE(0, 60)
        s:at(910)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_ROTATION(0, 0, 1070141403)
        s:SET_TRANSLATION(1137295360, 1142677504, 0)
        s:SET_ALPHA(0)
        s:at(500)
        s:FADE(255, 60)
        s:INTERP_POS(120, 4, 1137295360, 1131380736, 0)
        s:at(700)
        s:FADE(0, 60)
        s:at(760)
        s:EXIT_HIDE2()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_ROTATION(0, 0, 1070141403)
        s:SET_TRANSLATION(1137295360, 1142677504, 0)
        s:SET_ALPHA(0)
        s:at(100)
        s:FADE(255, 60)
        s:INTERP_POS(120, 4, 1137295360, 1131380736, 0)
        s:at(300)
        s:FADE(0, 60)
        s:at(360)
        s:EXIT_HIDE2()
    end,
  },
}
