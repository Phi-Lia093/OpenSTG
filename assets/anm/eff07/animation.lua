-- eff07: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 3,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 256 } },
    },
    scripts = {
      { index = 0, offset = 0 },
    },
    labels = {
      [0] = { 1, 2 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_TRANSLATION(1107296256, 1098907648, 1056964608)
        s:SET_ALPHA(0)
        s:FADE(255, 60)
        s:FADE(255, 60)
        s:SET_SCALE(1069547520, 1071644672)
        s:at(60)
        s:NOP()
        ::L1::
        s:LABEL(1, 60)
        s:SET_ALPHA(255)
        s:SET_COLOR(16777215)
        ::J144::
        s:NOP22()
        s:at(10060)
        s:SET_CLOCK(60, 144)
        goto J144
        ::L2::
        s:LABEL(2, 10060)
        s:SET_ALPHA(64)
        s:SET_COLOR(16728128)
        s:at(20060)
        s:SET_CLOCK(60, 144)
        goto J144
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
        s:SET_SCALE(1075020609, 1075020609)
        s:SET_TRANSLATION(1130364928, 1131413504, 1056964608)
        s:SET_ALPHA(0)
        s:FADE(255, 60)
        s:SET_ANGLE_VEL(0, 0, -1140806147)
        ::J96::
        s:at(10000)
        s:SET_CLOCK(0, 96)
        goto J96
        s:INTERP_SCALE_2(150, 0, 1077537191, 1077537191)
        s:at(10150)
        s:INTERP_SCALE_2(150, 0, 1075020609, 1075020609)
        s:at(10300)
        s:SET_CLOCK(0, 96)
        goto J96
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
