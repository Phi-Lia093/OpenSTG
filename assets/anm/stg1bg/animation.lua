-- stg1bg: how the animation is put together and what it does.
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
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 48 },
      { index = 2, offset = 100 },
    },
    labels = {
      [0] = {  },
      [1] = {  },
      [2] = {  },
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
  },
}
