-- staff01: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 512, 32 } },
      { id = 1, rect = { 0, 32, 512, 32 } },
      { id = 2, rect = { 0, 64, 512, 32 } },
      { id = 3, rect = { 0, 96, 512, 32 } },
      { id = 4, rect = { 0, 128, 512, 32 } },
      { id = 5, rect = { 0, 160, 512, 32 } },
      { id = 6, rect = { 0, 192, 512, 32 } },
      { id = 7, rect = { 0, 224, 512, 48 } },
      { id = 8, rect = { 0, 272, 512, 32 } },
      { id = 9, rect = { 0, 304, 512, 32 } },
      { id = 10, rect = { 0, 336, 512, 32 } },
      { id = 11, rect = { 0, 368, 512, 32 } },
      { id = 12, rect = { 0, 400, 512, 32 } },
      { id = 13, rect = { 0, 432, 512, 32 } },
      { id = 14, rect = { 0, 464, 512, 32 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 140 },
      { index = 2, offset = 280 },
      { index = 3, offset = 420 },
      { index = 4, offset = 560 },
      { index = 5, offset = 700 },
      { index = 6, offset = 840 },
      { index = 7, offset = 984 },
      { index = 8, offset = 1160 },
    },
    labels = {
      [0] = {  },
      [1] = {  },
      [2] = {  },
      [3] = {  },
      [4] = {  },
      [5] = {  },
      [6] = {  },
      [7] = {  },
      [8] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:FADE(255, 120)
        s:SET_TRANSLATION(1127219200, 1136656384, 0)
        s:POS_TIME_LINEAR(1127219200, 1134034944, 0, 220)
        s:at(320)
        s:POS_TIME_LINEAR(1127219200, 1130364928, 0, 220)
        s:at(420)
        s:FADE(0, 120)
        s:at(540)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:FADE(255, 120)
        s:SET_TRANSLATION(1127219200, 1136656384, 0)
        s:POS_TIME_LINEAR(1127219200, 1134034944, 0, 220)
        s:at(520)
        s:POS_TIME_LINEAR(1127219200, 1130364928, 0, 220)
        s:at(620)
        s:FADE(0, 120)
        s:at(740)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:FADE(255, 120)
        s:SET_TRANSLATION(1127219200, 1135083520, 0)
        s:POS_TIME_LINEAR(1127219200, 1135083520, 0, 220)
        s:at(320)
        s:POS_TIME_LINEAR(1127219200, 1132462080, 0, 220)
        s:at(420)
        s:FADE(0, 120)
        s:at(540)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:FADE(255, 120)
        s:SET_TRANSLATION(1127219200, 1136656384, 0)
        s:POS_TIME_LINEAR(1127219200, 1134034944, 0, 220)
        s:at(420)
        s:POS_TIME_LINEAR(1127219200, 1130364928, 0, 220)
        s:at(520)
        s:FADE(0, 120)
        s:at(640)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:FADE(255, 180)
        s:SET_TRANSLATION(1127219200, 1136656384, 0)
        s:POS_TIME_LINEAR(1127219200, 1134034944, 0, 220)
        s:at(660)
        s:POS_TIME_LINEAR(1127219200, 1130364928, 0, 220)
        s:at(760)
        s:FADE(0, 120)
        s:at(880)
        s:EXIT_HIDE2()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:FADE(255, 180)
        s:SET_TRANSLATION(1127219200, 1135083520, 0)
        s:POS_TIME_LINEAR(1127219200, 1135083520, 0, 220)
        s:at(540)
        s:POS_TIME_LINEAR(1127219200, 1132462080, 0, 220)
        s:at(640)
        s:FADE(0, 120)
        s:at(760)
        s:EXIT_HIDE2()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:FADE(255, 180)
        s:SET_ROTATION(0, 0, 1070141403)
        s:SET_TRANSLATION(1132986368, 1129316352, 0)
        s:POS_TIME_LINEAR(1132986368, 1129316352, 0, 220)
        s:at(420)
        s:NOP()
        s:at(620)
        s:FADE(0, 120)
        s:at(740)
        s:EXIT_HIDE2()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:FADE(255, 180)
        s:SET_SCALE(1069547520, 1069547520)
        s:INTERP_SCALE(1065353216, 1065353216, 120)
        s:SET_TRANSLATION(1127219200, 1136656384, 0)
        s:POS_TIME_LINEAR(1127219200, 1134034944, 0, 220)
        s:at(280)
        s:POS_TIME_LINEAR(1127219200, 1130364928, 0, 220)
        s:at(380)
        s:FADE(0, 120)
        s:at(500)
        s:EXIT_HIDE2()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:FADE(255, 180)
        s:SET_SCALE(1065353216, 1065353216)
        s:SET_TRANSLATION(1137180672, 1138753536, 0)
        s:POS_TIME_LINEAR(1137180672, 1138753536, 0, 220)
        s:at(550)
        s:EXIT()
    end,
  },
}
