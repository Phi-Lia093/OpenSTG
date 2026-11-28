-- stg4bg3: how the animation is put together and what it does.
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
        s:NOP22()
        s:SET_ALPHA(0)
        s:SET_AUTO_ROTATE(2)
        s:at(7250)
        s:INTERP_ALPHA(400, 0, 255)
        s:at(17250)
        s:EXIT()
    end,
  },
}
