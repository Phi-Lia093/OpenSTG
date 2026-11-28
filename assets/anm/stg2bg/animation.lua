-- stg2bg: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 1, 1, 254, 254 } },
      { id = 1, rect = { 258, 2, 124, 124 } },
      { id = 2, rect = { 386, 2, 124, 124 } },
      { id = 3, rect = { 0, 400, 256, 32 } },
      { id = 4, rect = { 1, 257, 126, 126 } },
      { id = 5, rect = { 129, 257, 126, 126 } },
      { id = 6, rect = { 1, 384, 126, 8 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 48 },
      { index = 2, offset = 100 },
      { index = 3, offset = 152 },
      { index = 4, offset = 204 },
      { index = 5, offset = 264 },
      { index = 6, offset = 324 },
      { index = 7, offset = 384 },
      { index = 8, offset = 444 },
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
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_CAMERA_MODE(1)
        s:SET_AUTO_ROTATE(2)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_CAMERA_MODE(1)
        s:SET_AUTO_ROTATE(2)
        s:EXIT()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_CAMERA_MODE(1)
        s:SET_AUTO_ROTATE(2)
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(0, 1070141403, 1070141403)
        s:EXIT()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(0, 1070141403, -1077342245)
        s:EXIT()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(-1094119461, 0, 0)
        s:EXIT()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1053364187, 0, 0)
        s:EXIT()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:SET_CAMERA_MODE(1)
        s:EXIT()
    end,
  },
}
