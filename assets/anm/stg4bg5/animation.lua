-- stg4bg5: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 1,
    sprites = {
      { id = 0, rect = { 1, 1, 254, 254 } },
    },
    scripts = {
      { index = 0, offset = 0 },
    },
    labels = {
      [0] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_CAMERA_MODE(1)
        s:at(9000)
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_ANGLE_VEL(0, 986571566, 0)
        ::J64::
        s:INTERP_ALPHA(120, 4, 128)
        s:at(9120)
        s:INTERP_ALPHA(120, 4, 255)
        s:at(9240)
        s:SET_CLOCK(9000, 64)
        goto J64
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
