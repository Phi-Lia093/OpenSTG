-- stg6bg: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 0, 1, 256, 254 } },
      { id = 1, rect = { 256, 1, 256, 254 } },
      { id = 2, rect = { 0, 256, 256, 256 } },
      { id = 3, rect = { 257, 257, 254, 254 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 80 },
      { index = 2, offset = 160 },
      { index = 3, offset = 272 },
      { index = 4, offset = 384 },
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
        s:SET_ACTIVE_SPRITE(0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:SET_ALPHA(0)
        s:at(350)
        s:INTERP_ALPHA(40, 0, 255)
        s:at(390)
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:SET_ALPHA(0)
        s:at(350)
        s:INTERP_ALPHA(40, 0, 255)
        s:at(390)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_ROTATION(0, 0, 1078530011)
        s:SET_COLOR(12632256)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:SET_ALPHA(0)
        s:at(350)
        s:INTERP_ALPHA(40, 0, 160)
        s:at(390)
        s:EXIT()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ROTATION(0, 0, 1078530011)
        s:SET_COLOR(12632256)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:SET_ALPHA(0)
        s:at(350)
        s:INTERP_ALPHA(40, 0, 160)
        s:at(390)
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_ANGLE_VEL(0, 0, 1015066109)
        s:SET_TRANSLATION(1130364928, 1131413504, 0)
        s:SET_SCALE(1073741824, 1073741824)
        s:INTERP_SCALE_2(400, 4, 1065353216, 1065353216)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(400, 0, 255)
        s:INTERP_POS(400, 0, 1130364928, 1126694912, 0)
        s:at(400)
        s:NOP()
        ::J160::
        s:INTERP_ALPHA(400, 0, 32)
        s:INTERP_SCALE_2(400, 0, 1069547520, 1069547520)
        s:at(800)
        s:INTERP_ALPHA(400, 0, 255)
        s:INTERP_SCALE_2(400, 0, 1065353216, 1065353216)
        s:at(1200)
        s:SET_CLOCK(400, 160)
        goto J160
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
  [1] = {
    texture = "texture_1.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = -1, rect = { 0, 0, 384, 448 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 88 },
      { index = 2, offset = 176 },
      { index = 3, offset = 264 },
      { index = 4, offset = 352 },
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
        s:SET_ACTIVE_SPRITE(-1)
        s:SET_TRANSLATION(1107296256, 1098907648, 0)
        s:NOP22()
        s:SET_COLOR(0)
        s:at(350)
        s:INTERP_COLOR(40, 0, 16777215)
        s:at(390)
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(-1)
        s:SET_TRANSLATION(1107296256, 1098907648, 0)
        s:NOP22()
        s:SET_COLOR(16777215)
        s:INTERP_COLOR(60, 0, 3932415)
        s:at(60)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(-1)
        s:SET_TRANSLATION(1107296256, 1098907648, 0)
        s:NOP22()
        s:SET_COLOR(3932415)
        s:INTERP_COLOR(60, 0, 16711700)
        s:at(60)
        s:EXIT()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(-1)
        s:SET_TRANSLATION(1107296256, 1098907648, 0)
        s:NOP22()
        s:SET_COLOR(16711700)
        s:INTERP_COLOR(60, 0, 16711935)
        s:at(60)
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(-1)
        s:SET_TRANSLATION(1107296256, 1098907648, 0)
        s:NOP22()
        s:SET_ALPHA(255)
        s:SET_COLOR(16711935)
        s:INTERP_ALPHA(60, 0, 128)
        s:INTERP_COLOR(60, 0, 255)
        s:at(60)
        s:EXIT()
    end,
  },
}
