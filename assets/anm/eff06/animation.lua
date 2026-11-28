-- eff06: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 3,
    sprites = {
      { id = 0, rect = { 0, 0, 384, 448 } },
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
        s:NOP22()
        s:SET_TRANSLATION(1107296256, 1098907648, 1056964608)
        s:SET_ALPHA(0)
        s:FADE(255, 60)
        ::J68::
        s:SET_SCROLL_POS_Y(1007192201)
        s:at(1)
        s:SET_CLOCK(0, 68)
        goto J68
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
  [1] = {
    texture = "texture_1.png",
    width = 256, height = 256, format = 5,
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
        s:NOP22()
        s:SET_TRANSLATION(1107296256, 1098907648, 1056964608)
        s:SET_ALPHA(0)
        s:FADE(224, 60)
        s:SET_SCALE(1069547520, 1071644672)
        ::J84::
        s:at(10000)
        s:SET_CLOCK(0, 84)
        goto J84
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
