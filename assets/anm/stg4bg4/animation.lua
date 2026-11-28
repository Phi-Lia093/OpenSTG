-- stg4bg4: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 1, 1, 126, 254 } },
      { id = 1, rect = { 1, 1, 126, 126 } },
      { id = 2, rect = { 129, 1, 382, 510 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 72 },
      { index = 2, offset = 144 },
      { index = 3, offset = 216 },
      { index = 4, offset = 256 },
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
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_COLOR(6842472)
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 1070141403)
        s:SET_COLOR(3158064)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, -1077342245)
        s:SET_COLOR(10526880)
        s:EXIT()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_CAMERA_MODE(1)
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 0)
        s:EXIT()
    end,
  },
}
