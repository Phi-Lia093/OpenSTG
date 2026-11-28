-- face_sk00: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 1, 1, 126, 510 } },
      { id = 1, rect = { 129, 1, 126, 510 } },
      { id = 2, rect = { 257, 1, 126, 510 } },
      { id = 3, rect = { 385, 1, 126, 510 } },
      { id = 4, rect = { 433, 0, 78, 512 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 380 },
      { index = 2, offset = 516 },
      { index = 3, offset = 896 },
      { index = 4, offset = 1032 },
      { index = 5, offset = 1264 },
      { index = 6, offset = 1496 },
      { index = 7, offset = 1740 },
    },
    labels = {
      [0] = { 1, 2, 3, 4, 5 },
      [1] = {  },
      [2] = { 1, 2, 3, 4, 5 },
      [3] = {  },
      [4] = {  },
      [5] = {  },
      [6] = {  },
      [7] = {  },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == 3 then goto L3 end
        if at == 4 then goto L4 end
        if at == 5 then goto L5 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_TRANSLATION(-1027604480, 1098907648, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1107296256, 1141637120, 0)
        s:INTERP_POS(30, 4, 1109393408, 1121976320, 0)
        s:INTERP_ALPHA(30, 0, 255)
        s:at(30)
        s:STOP()
        ::L2::
        s:LABEL(2, 60)
        s:at(60)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 60)
        s:INTERP_ALPHA(15, 0, 255)
        s:INTERP_POS(15, 4, 1109393408, 1121976320, 0)
        s:at(75)
        s:STOP()
        ::L4::
        s:LABEL(4, 75)
        s:INTERP_ALPHA(15, 0, 128)
        s:INTERP_POS(15, 4, 1103101952, 1124073472, 0)
        s:at(90)
        s:STOP()
        ::L5::
        s:LABEL(5, 90)
        s:INTERP_ALPHA(30, 0, 0)
        s:INTERP_POS(30, 0, -1027604480, 1124073472, 0)
        s:at(120)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:NOP22()
        s:SET_TRANSLATION(1111490560, 1125122048, 0)
        s:INTERP_ALPHA(60, 0, 224)
        s:INTERP_POS(180, 0, 1111490560, -1025507328, 0)
        s:at(90)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(150)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == 3 then goto L3 end
        if at == 4 then goto L4 end
        if at == 5 then goto L5 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_TRANSLATION(1133248512, 1141637120, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1133248512, 1141637120, 0)
        s:INTERP_POS(30, 4, 1133248512, 1121976320, 0)
        s:INTERP_ALPHA(30, 0, 255)
        s:at(30)
        s:STOP()
        ::L2::
        s:LABEL(2, 60)
        s:at(60)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 60)
        s:INTERP_ALPHA(15, 0, 255)
        s:INTERP_POS(15, 4, 1133248512, 1121976320, 0)
        s:at(75)
        s:STOP()
        ::L4::
        s:LABEL(4, 75)
        s:INTERP_ALPHA(15, 0, 128)
        s:INTERP_POS(15, 4, 1133772800, 1124073472, 0)
        s:at(90)
        s:STOP()
        ::L5::
        s:LABEL(5, 90)
        s:INTERP_ALPHA(30, 0, 0)
        s:INTERP_POS(30, 0, 1137704960, 1124073472, 0)
        s:at(120)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:NOP22()
        s:SET_TRANSLATION(1132986368, -1022361600, 0)
        s:INTERP_ALPHA(60, 0, 224)
        s:INTERP_POS(180, 0, 1132986368, 1121976320, 0)
        s:at(90)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(150)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_ALPHA(0)
        s:NOP22()
        s:SET_TRANSLATION(1107296256, 1098907648, 0)
        s:INTERP_POS(150, 0, 1125122048, 1098907648, 0)
        s:INTERP_ALPHA(50, 0, 255)
        s:MOV({r = 10008}, 100)
        ::J116::
        s:SET_SCROLL_POS_Y(-1140291447)
        s:at(1)
        if s:LOOP_BACK({r = 10008}, 0, 116) then
            goto J116
        end
        s:INTERP_ALPHA(50, 0, 0)
        s:MOV({r = 10008}, 50)
        ::J184::
        s:SET_SCROLL_POS_Y(-1140291447)
        s:at(2)
        if s:LOOP_BACK({r = 10008}, 1, 184) then
            goto J184
        end
        s:EXIT_HIDE2()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_ALPHA(0)
        s:NOP22()
        s:SET_TRANSLATION(1135083520, 1098907648, 0)
        s:INTERP_POS(150, 0, 1131413504, 1098907648, 0)
        s:INTERP_ALPHA(50, 0, 255)
        s:MOV({r = 10008}, 100)
        ::J116::
        s:SET_SCROLL_POS_Y(1007192201)
        s:at(1)
        if s:LOOP_BACK({r = 10008}, 0, 116) then
            goto J116
        end
        s:INTERP_ALPHA(50, 0, 0)
        s:MOV({r = 10008}, 50)
        ::J184::
        s:SET_SCROLL_POS_Y(1007192201)
        s:at(2)
        if s:LOOP_BACK({r = 10008}, 1, 184) then
            goto J184
        end
        s:EXIT_HIDE2()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_ALPHA(0)
        s:SET_ROTATION(0, 0, -1077342245)
        s:SET_TRANSLATION(1130364928, 1124597760, 0)
        s:INTERP_POS(150, 0, 1130364928, 1130889216, 0)
        s:at(20)
        s:INTERP_ALPHA(50, 0, 255)
        s:MOV({r = 10008}, 80)
        ::J128::
        s:SET_SCROLL_POS_Y(1007192201)
        s:at(21)
        if s:LOOP_BACK({r = 10008}, 20, 128) then
            goto J128
        end
        s:INTERP_ALPHA(50, 0, 0)
        s:MOV({r = 10008}, 50)
        ::J196::
        s:SET_SCROLL_POS_Y(1007192201)
        s:at(22)
        if s:LOOP_BACK({r = 10008}, 21, 196) then
            goto J196
        end
        s:EXIT_HIDE2()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_ALPHA(0)
        s:SET_ROTATION(0, 0, -1077342245)
        s:SET_TRANSLATION(1130364928, 1137442816, 0)
        s:INTERP_POS(150, 0, 1130364928, 1134297088, 0)
        s:at(20)
        s:INTERP_ALPHA(50, 0, 255)
        s:MOV({r = 10008}, 80)
        ::J128::
        s:SET_SCROLL_POS_Y(-1140291447)
        s:at(21)
        if s:LOOP_BACK({r = 10008}, 20, 128) then
            goto J128
        end
        s:INTERP_ALPHA(50, 0, 0)
        s:MOV({r = 10008}, 50)
        ::J196::
        s:SET_SCROLL_POS_Y(-1140291447)
        s:at(22)
        if s:LOOP_BACK({r = 10008}, 21, 196) then
            goto J196
        end
        s:EXIT_HIDE2()
    end,
  },
  [1] = {
    texture = "texture_1.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 1, 1, 126, 510 } },
      { id = 1, rect = { 129, 1, 126, 510 } },
      { id = 2, rect = { 257, 1, 126, 510 } },
      { id = 3, rect = { 385, 1, 126, 510 } },
      { id = 4, rect = { 433, 0, 78, 512 } },
    },
    scripts = {},
    labels = {
    },
  },
}
