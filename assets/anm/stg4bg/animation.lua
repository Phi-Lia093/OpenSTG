-- stg4bg: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 256, format = 1,
    sprites = {
      { id = 0, rect = { 1, 1, 254, 254 } },
      { id = 1, rect = { 258, 2, 253, 253 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 136 },
    },
    labels = {
      [0] = {  },
      [1] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_TRANSLATION(1130364928, 1131413504, 1065017672)
        s:SET_SCALE(1075096730, 1075096730)
        s:SET_ANGLE_VEL(0, 0, 1001099942)
        s:SET_COLOR(0)
        s:at(600)
        s:INTERP_COLOR(400, 0, 16777215)
        s:at(6200)
        s:INTERP_COLOR(400, 0, 0)
        s:at(8200)
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
  },
}
