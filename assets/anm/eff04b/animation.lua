-- eff04b: how the animation is put together and what it does.
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
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_TRANSLATION(1130364928, 1132986368, 1056964608)
        s:SET_ALPHA(0)
        s:at(60)
        s:FADE(255, 60)
        ::J76::
        s:at(10060)
        s:SET_CLOCK(60, 76)
        goto J76
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
