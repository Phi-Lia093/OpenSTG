-- capture: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "@",
    width = 512, height = 512, format = 3,
    sprites = {
      { id = 0, rect = { 0, 0, 128, 128 } },
      { id = 1, rect = { 128, 0, 384, 448 } },
      { id = 2, rect = { 128, 0, 32, 32 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 128 },
      { index = 2, offset = 396 },
      { index = 3, offset = 500 },
    },
    labels = {
      [0] = { 1 },
      [1] = { 1 },
      [2] = {  },
      [3] = {  },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_SCALE(1077936128, 1080033280)
        s:NOP22()
        s:SET_COLOR(1097952)
        s:SET_ALPHA(0)
        s:FADE(255, 30)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 30)
        s:FADE(0, 30)
        s:at(60)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:NOP22()
        s:SET_ANGLE_VEL(0, 0, 1037465424)
        s:SET_ALPHA(255)
        s:INTERP_ALPHA(60, 0, 160)
        s:SET_COLOR(16777215)
        s:INTERP_COLOR(60, 0, 8437984)
        s:SET_SCALE(1065353216, 1065353216)
        s:INTERP_SCALE_2(60, 0, 1053609165, 1053609165)
        s:SET_TRANSLATION(1107296256, 1098907648, 0)
        s:INTERP_POS(60, 0, 1111490560, 1124073472, 0)
        s:at(60)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ROTATION(0, 0, 0)
        s:STOP()
        ::L1::
        s:LABEL(1, 60)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:at(1)
        s:WAIT({r = 10008})
        s:INTERP_SCALE_2(60, 1, 0, 0)
        s:INTERP_ALPHA(60, 1, 0)
        s:SET_ANGLE_VEL(1020688208, 1020688208, 1020688208)
        s:at(61)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:at(1)
        s:WAIT({r = 10008})
        s:INTERP_SCALE_2(60, 1, 0, 0)
        s:INTERP_ALPHA(60, 1, 0)
        s:SET_ANGLE_VEL(1020688208, 1020688208, 1020688208)
        s:at(61)
        s:EXIT_HIDE2()
    end,
  },
}
