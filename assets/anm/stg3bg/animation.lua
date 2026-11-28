-- stg3bg: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 128 } },
      { id = 1, rect = { 0, 128, 256, 128 } },
      { id = 2, rect = { 0, 256, 256, 128 } },
      { id = 3, rect = { 0, 384, 256, 128 } },
      { id = 4, rect = { 257, 1, 126, 126 } },
      { id = 5, rect = { 257, 257, 254, 254 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 100 },
      { index = 2, offset = 200 },
      { index = 3, offset = 300 },
      { index = 4, offset = 400 },
      { index = 5, offset = 492 },
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
        s:SET_ACTIVE_SPRITE(0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:at(400)
        s:SET_ALPHA(255)
        s:INTERP_ALPHA(320, 0, 0)
        s:at(720)
        s:INTERP_ALPHA(60, 0, 255)
        s:at(780)
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:at(400)
        s:SET_ALPHA(255)
        s:INTERP_ALPHA(320, 0, 0)
        s:at(720)
        s:INTERP_ALPHA(60, 0, 255)
        s:at(780)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:at(400)
        s:SET_ALPHA(255)
        s:INTERP_ALPHA(320, 0, 0)
        s:at(720)
        s:INTERP_ALPHA(60, 0, 255)
        s:at(780)
        s:EXIT()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:at(400)
        s:SET_ALPHA(255)
        s:INTERP_ALPHA(320, 0, 0)
        s:at(720)
        s:INTERP_ALPHA(60, 0, 255)
        s:at(780)
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_CAMERA_MODE(1)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(240, 0, 255)
        s:SET_ROTATION(0, 0, -1077342245)
        s:at(30000)
        s:EXIT()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_CAMERA_MODE(1)
        s:SET_ZWRITE_DISABLE(1)
        s:NOP22()
        s:SET_ALPHA(0)
        s:at(700)
        s:INTERP_ALPHA(240, 0, 255)
        s:at(950)
        s:EXIT()
    end,
  },
}
