-- front: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 1,
    sprites = {
      { id = 0, rect = { 128, 0, 128, 256 } },
      { id = 1, rect = { 0, 0, 128, 80 } },
      { id = 2, rect = { 0, 80, 64, 16 } },
      { id = 3, rect = { 0, 96, 64, 16 } },
      { id = 4, rect = { 0, 112, 64, 16 } },
      { id = 5, rect = { 0, 128, 64, 16 } },
      { id = 6, rect = { 0, 144, 64, 16 } },
      { id = 7, rect = { 0, 160, 64, 16 } },
      { id = 8, rect = { 0, 176, 64, 16 } },
      { id = 9, rect = { 0, 192, 128, 16 } },
      { id = 10, rect = { 64, 80, 16, 16 } },
      { id = 11, rect = { 80, 80, 16, 16 } },
      { id = 12, rect = { 0, 224, 32, 32 } },
      { id = 13, rect = { 0, 240, 128, 16 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 120 },
      { index = 2, offset = 240 },
      { index = 3, offset = 364 },
      { index = 4, offset = 488 },
      { index = 5, offset = 612 },
      { index = 6, offset = 736 },
      { index = 7, offset = 860 },
      { index = 8, offset = 984 },
      { index = 9, offset = 1108 },
      { index = 10, offset = 1144 },
      { index = 11, offset = 1180 },
      { index = 12, offset = 1396 },
      { index = 13, offset = 1432 },
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
      [9] = {  },
      [10] = {  },
      [11] = { 1, -1 },
      [12] = {  },
      [13] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:FADE(255, 120)
        s:SET_TRANSLATION(1140850688, 1129316352, 0)
        s:POS_TIME_LINEAR(1139802112, 1129316352, 0, 120)
        s:at(120)
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:FADE(255, 120)
        s:SET_TRANSLATION(1136656384, 1135083520, 0)
        s:POS_TIME_LINEAR(1138753536, 1135083520, 0, 120)
        s:at(120)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137704960, 1111490560, 0)
        s:FADE(255, 16)
        s:INTERP_POS(60, 0, 1138229248, 1111490560, 0)
        s:at(60)
        s:EXIT()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138753536, 1115684864, 0)
        s:at(8)
        s:FADE(255, 16)
        s:INTERP_POS(60, 0, 1138229248, 1115684864, 0)
        s:at(68)
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1121976320, 0)
        s:at(16)
        s:FADE(255, 16)
        s:INTERP_POS(60, 0, 1138229248, 1119879168, 0)
        s:at(76)
        s:EXIT()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1139277824, 1121976320, 0)
        s:at(24)
        s:FADE(255, 16)
        s:INTERP_POS(60, 0, 1138229248, 1121976320, 0)
        s:at(84)
        s:EXIT()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137967104, 1125122048, 0)
        s:at(32)
        s:FADE(255, 16)
        s:INTERP_POS(60, 0, 1138229248, 1125122048, 0)
        s:at(92)
        s:EXIT()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1125122048, 0)
        s:at(40)
        s:FADE(255, 16)
        s:INTERP_POS(60, 0, 1138229248, 1126170624, 0)
        s:at(100)
        s:EXIT()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138753536, 1127219200, 0)
        s:at(48)
        s:FADE(255, 16)
        s:INTERP_POS(60, 0, 1138229248, 1127219200, 0)
        s:at(108)
        s:EXIT()
    end,

    -- stream 9
    [9] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(10)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 10
    [10] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(11)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 11
    [11] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1117782016, 1104150528, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:FADE(255, 16)
        s:INTERP_POS(30, 0, 1107296256, 1104150528, 0)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:FADE(0, 30)
        s:INTERP_POS(30, 0, 1117782016, 1104150528, 0)
        s:at(60)
        s:STOP_HIDE()
        s:EXIT_HIDE2()
    end,

    -- stream 12
    [12] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(12)
        s:NOP22()
        s:at(200)
        s:EXIT()
    end,

    -- stream 13
    [13] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(13)
        s:NOP22()
        s:EXIT()
    end,
  },
  [1] = {
    texture = "texture_1.png",
    width = 128, height = 256, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 128, 16 } },
      { id = 1, rect = { 0, 16, 128, 16 } },
      { id = 2, rect = { 0, 32, 128, 16 } },
      { id = 3, rect = { 0, 48, 128, 16 } },
      { id = 4, rect = { 0, 64, 128, 16 } },
      { id = 5, rect = { 0, 80, 128, 16 } },
      { id = 6, rect = { 0, 96, 128, 16 } },
      { id = 7, rect = { 0, 112, 128, 16 } },
      { id = 8, rect = { 0, 128, 128, 16 } },
      { id = 9, rect = { 0, 144, 128, 16 } },
      { id = 10, rect = { 0, 160, 128, 16 } },
      { id = 11, rect = { 0, 176, 128, 16 } },
      { id = 12, rect = { 0, 192, 128, 16 } },
      { id = 13, rect = { 0, 208, 128, 16 } },
      { id = 14, rect = { 0, 224, 128, 16 } },
      { id = 15, rect = { 0, 240, 128, 16 } },
    },
    scripts = {},
    labels = {
    },
  },
}
