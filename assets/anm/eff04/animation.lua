-- eff04: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 256 } },
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
        s:SET_SCALE(1075020609, 1075020609)
        s:SET_TRANSLATION(1130364928, 1131413504, 1056964608)
        s:SET_ALPHA(0)
        s:FADE(255, 60)
        s:SET_ANGLE_VEL(0, 0, -1140806147)
        s:at(60)
        s:SET_ANGLE_VEL(0, 0, -1140806147)
        ::J116::
        s:SET_COLOR(16777215)
        s:SET_ALPHA(255)
        s:at(61)
        s:SET_CLOCK(60, 116)
        goto J116
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
