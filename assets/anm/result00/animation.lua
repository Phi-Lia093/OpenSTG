-- result00: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 32 } },
      { id = 1, rect = { 0, 32, 256, 32 } },
      { id = 2, rect = { 0, 64, 256, 32 } },
      { id = 3, rect = { 0, 96, 256, 32 } },
      { id = 4, rect = { 0, 128, 256, 32 } },
      { id = 5, rect = { 0, 160, 192, 32 } },
      { id = 6, rect = { 192, 160, 64, 32 } },
      { id = 7, rect = { 0, 192, 256, 32 } },
      { id = 8, rect = { 0, 224, 256, 32 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 164 },
      { index = 2, offset = 328 },
      { index = 3, offset = 492 },
      { index = 4, offset = 656 },
      { index = 5, offset = 820 },
      { index = 6, offset = 984 },
      { index = 7, offset = 1148 },
      { index = 8, offset = 1312 },
    },
    labels = {
      [0] = { 1, -1 },
      [1] = { 1, -1 },
      [2] = { 1, -1 },
      [3] = { 1, -1 },
      [4] = { 1, -1 },
      [5] = { 1, -1 },
      [6] = { 1, -1 },
      [7] = { 1, -1 },
      [8] = { 1, -1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_TRANSLATION(-1015021568, 1119879168, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_TRANSLATION(-1015021568, 1119879168, 0)
        s:POS_TIME_DECEL(48.0, 96.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(-256.0, 96.0, 0.0, 60)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:NOP22()
        s:SET_TRANSLATION(-1015021568, 1124073472, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_TRANSLATION(-1015021568, 1124073472, 0)
        s:POS_TIME_DECEL(48.0, 128.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(-256.0, 128.0, 0.0, 60)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:NOP22()
        s:SET_TRANSLATION(-1015021568, 1126170624, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_TRANSLATION(-1015021568, 1126170624, 0)
        s:POS_TIME_DECEL(48.0, 160.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(-256.0, 160.0, 0.0, 60)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:NOP22()
        s:SET_TRANSLATION(-1015021568, 1128267776, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_TRANSLATION(-1015021568, 1128267776, 0)
        s:POS_TIME_DECEL(48.0, 192.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(-256.0, 192.0, 0.0, 60)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:NOP22()
        s:SET_TRANSLATION(-1015021568, 1130364928, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_TRANSLATION(-1015021568, 1130364928, 0)
        s:POS_TIME_DECEL(48.0, 224.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(-256.0, 224.0, 0.0, 60)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:NOP22()
        s:SET_TRANSLATION(-1015021568, 1132462080, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_TRANSLATION(-1015021568, 1132462080, 0)
        s:POS_TIME_DECEL(48.0, 256.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(-256.0, 256.0, 0.0, 60)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 6
    [6] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:NOP22()
        s:SET_TRANSLATION(-1015021568, 1133510656, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_TRANSLATION(-1015021568, 1133510656, 0)
        s:POS_TIME_DECEL(48.0, 288.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(-256.0, 288.0, 0.0, 60)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 7
    [7] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:NOP22()
        s:SET_TRANSLATION(-1015021568, 1134559232, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_TRANSLATION(-1015021568, 1134559232, 0)
        s:POS_TIME_DECEL(48.0, 320.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(-256.0, 320.0, 0.0, 60)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 8
    [8] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:NOP22()
        s:SET_TRANSLATION(-1015021568, 1135607808, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_TRANSLATION(-1015021568, 1135607808, 0)
        s:POS_TIME_DECEL(48.0, 352.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(-256.0, 352.0, 0.0, 60)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
  [1] = {
    texture = "texture_1.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 112, 48 } },
      { id = 1, rect = { 112, 0, 144, 48 } },
      { id = 2, rect = { 0, 48, 112, 48 } },
      { id = 3, rect = { 112, 48, 144, 48 } },
      { id = 4, rect = { 0, 96, 112, 48 } },
      { id = 5, rect = { 112, 96, 144, 48 } },
      { id = 6, rect = { 0, 144, 256, 48 } },
      { id = 7, rect = { 0, 240, 256, 16 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 144 },
      { index = 2, offset = 288 },
      { index = 3, offset = 432 },
      { index = 4, offset = 576 },
      { index = 5, offset = 720 },
      { index = 6, offset = 864 },
      { index = 7, offset = 1008 },
    },
    labels = {
      [0] = { 3, -1 },
      [1] = { 4, -1 },
      [2] = { 5, -1 },
      [3] = { 6, -1 },
      [4] = { 7, -1 },
      [5] = { 8, -1 },
      [6] = { 10, -1 },
      [7] = { 3, 4, 5, 6, 7, 8, 10, -1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_TRANSLATION(1142947840, 1115684864, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 0)
        s:POS_TIME_DECEL(48.0, 64.0, 0.0, 20)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(640.0, 64.0, 0.0, 20)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == 4 then goto L4 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:NOP22()
        s:SET_TRANSLATION(1142947840, 1115684864, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 0)
        s:POS_TIME_DECEL(48.0, 64.0, 0.0, 20)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(640.0, 64.0, 0.0, 20)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == 5 then goto L5 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:NOP22()
        s:SET_TRANSLATION(1142947840, 1115684864, 0)
        s:STOP_HIDE()
        ::L5::
        s:LABEL(5, 0)
        s:POS_TIME_DECEL(48.0, 64.0, 0.0, 20)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(640.0, 64.0, 0.0, 20)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == 6 then goto L6 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:NOP22()
        s:SET_TRANSLATION(1142947840, 1115684864, 0)
        s:STOP_HIDE()
        ::L6::
        s:LABEL(6, 0)
        s:POS_TIME_DECEL(48.0, 64.0, 0.0, 20)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(640.0, 64.0, 0.0, 20)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == 7 then goto L7 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:NOP22()
        s:SET_TRANSLATION(1142947840, 1115684864, 0)
        s:STOP_HIDE()
        ::L7::
        s:LABEL(7, 0)
        s:POS_TIME_DECEL(48.0, 64.0, 0.0, 20)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(640.0, 64.0, 0.0, 20)
        s:at(80)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        local at = s:entry()
        if at == 8 then goto L8 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:NOP22()
        s:SET_TRANSLATION(1142947840, 1115684864, 0)
        s:STOP_HIDE()
        ::L8::
        s:LABEL(8, 0)
        s:POS_TIME_DECEL(48.0, 64.0, 0.0, 20)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(640.0, 64.0, 0.0, 20)
        s:at(80)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 6
    [6] = function(s)
        local at = s:entry()
        if at == 10 then goto L10 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:NOP22()
        s:SET_TRANSLATION(1142947840, 1115684864, 0)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 0)
        s:POS_TIME_DECEL(48.0, 64.0, 0.0, 20)
        s:at(20)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 20)
        s:POS_TIME_DECEL(640.0, 64.0, 0.0, 20)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 7
    [7] = function(s)
        local at = s:entry()
        if at == 3 then goto L3 end
        if at == 4 then goto L4 end
        if at == 5 then goto L5 end
        if at == 6 then goto L6 end
        if at == 7 then goto L7 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:NOP22()
        s:SET_TRANSLATION(1142947840, 1121976320, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 0)
        ::L4::
        s:LABEL(4, 0)
        ::L5::
        s:LABEL(5, 0)
        ::L6::
        s:LABEL(6, 0)
        ::L7::
        s:LABEL(7, 0)
        ::L8::
        s:LABEL(8, 0)
        ::L10::
        s:LABEL(10, 0)
        s:SET_VISIBILITY(0)
        s:POS_TIME_DECEL(640.0, 112.0, 0.0, 20)
        s:at(20)
        s:POS_TIME_DECEL(0.0, 112.0, 0.0, 20)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:SET_VISIBILITY(0)
        s:POS_TIME_DECEL(640.0, 112.0, 0.0, 20)
        s:at(60)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
  [2] = {
    texture = "texture_2.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 32 } },
      { id = 1, rect = { 0, 32, 48, 32 } },
      { id = 2, rect = { 48, 32, 64, 32 } },
      { id = 3, rect = { 0, 64, 256, 48 } },
      { id = 4, rect = { 0, 112, 256, 48 } },
      { id = 5, rect = { 0, 176, 256, 32 } },
      { id = 6, rect = { 0, 208, 256, 48 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 248 },
      { index = 2, offset = 460 },
      { index = 3, offset = 668 },
      { index = 4, offset = 876 },
      { index = 5, offset = 1008 },
      { index = 6, offset = 1140 },
      { index = 7, offset = 1272 },
      { index = 8, offset = 1424 },
      { index = 9, offset = 1620 },
      { index = 10, offset = 1816 },
      { index = 11, offset = 2012 },
      { index = 12, offset = 2208 },
      { index = 13, offset = 2404 },
      { index = 14, offset = 2600 },
      { index = 15, offset = 2796 },
      { index = 16, offset = 2992 },
      { index = 17, offset = 3188 },
      { index = 18, offset = 3384 },
      { index = 19, offset = 3580 },
      { index = 20, offset = 3776 },
      { index = 21, offset = 3972 },
      { index = 22, offset = 4168 },
    },
    labels = {
      [0] = { 1, 3, 4, 5, 6, 7, 10, -1 },
      [1] = { 11, 14, 15, 19, -1 },
      [2] = { 11, 13, -1 },
      [3] = { 11, 13, -1 },
      [4] = { 14, -1 },
      [5] = { 19, -1 },
      [6] = { 13, -1 },
      [7] = { 12, -1 },
      [8] = { 12, 16, -1 },
      [9] = { 12, 16, -1 },
      [10] = { 12, 16, -1 },
      [11] = { 12, 16, -1 },
      [12] = { 12, 16, -1 },
      [13] = { 12, 16, -1 },
      [14] = { 12, 16, -1 },
      [15] = { 12, 16, -1 },
      [16] = { 12, 16, -1 },
      [17] = { 12, 16, -1 },
      [18] = { 12, 16, -1 },
      [19] = { 12, 16, -1 },
      [20] = { 12, 16, -1 },
      [21] = { 12, 16, -1 },
      [22] = { 12, 16, -1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 3 then goto L3 end
        if at == 4 then goto L4 end
        if at == 5 then goto L5 end
        if at == 6 then goto L6 end
        if at == 7 then goto L7 end
        if at == 10 then goto L10 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:NOP22()
        s:SET_TRANSLATION(1111490560, -1035993088, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:POS_TIME_DECEL(48.0, 32.0, 0.0, 60)
        s:at(60)
        s:STOP()
        ::L3::
        s:LABEL(3, 60)
        ::L4::
        s:LABEL(4, 60)
        ::L5::
        s:LABEL(5, 60)
        ::L6::
        s:LABEL(6, 60)
        ::L7::
        s:LABEL(7, 60)
        ::L10::
        s:LABEL(10, 60)
        s:POS_TIME_DECEL(48.0, 12.0, 0.0, 60)
        s:at(120)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 120)
        s:POS_TIME_DECEL(48.0, -48.0, 0.0, 60)
        s:at(180)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == 11 then goto L11 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_TRANSLATION(1134559232, 1128267776, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L11::
        s:LABEL(11, 0)
        s:SET_TRANSLATION(1134559232, 1128267776, 0)
        s:FADE(255, 30)
        s:at(30)
        s:STOP()
        ::L14::
        s:LABEL(14, 30)
        ::L15::
        s:LABEL(15, 30)
        ::L19::
        s:LABEL(19, 30)
        s:FADE(64, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == 11 then goto L11 end
        if at == 13 then goto L13 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_TRANSLATION(1133510656, 1132462080, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L11::
        s:LABEL(11, 0)
        s:SET_TRANSLATION(1133510656, 1132462080, 0)
        s:FADE(255, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        s:SET_TRANSLATION(1133510656, 1138229248, 0)
        s:FADE(255, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == 11 then goto L11 end
        if at == 13 then goto L13 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_TRANSLATION(1135607808, 1132462080, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L11::
        s:LABEL(11, 0)
        s:SET_TRANSLATION(1135607808, 1132462080, 0)
        s:FADE(255, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        s:SET_TRANSLATION(1135607808, 1138229248, 0)
        s:FADE(255, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == 14 then goto L14 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_TRANSLATION(1134559232, 1132462080, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L14::
        s:LABEL(14, 0)
        s:FADE(255, 30)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:FADE(0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        local at = s:entry()
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:SET_TRANSLATION(1134559232, 1132462080, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L19::
        s:LABEL(19, 0)
        s:FADE(255, 30)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:FADE(0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 6
    [6] = function(s)
        local at = s:entry()
        if at == 13 then goto L13 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1134559232, 1137180672, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L13::
        s:LABEL(13, 0)
        s:FADE(255, 30)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:FADE(0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 7
    [7] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 128.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 8
    [8] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 9
    [9] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 177.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 10
    [10] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 194.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 11
    [11] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 211.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 12
    [12] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 228.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 13
    [13] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 245.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 14
    [14] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 262.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 15
    [15] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 279.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 16
    [16] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 296.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 17
    [17] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 313.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 18
    [18] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 330.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 19
    [19] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 347.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 20
    [20] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 364.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 21
    [21] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 381.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 22
    [22] = function(s)
        local at = s:entry()
        if at == 12 then goto L12 end
        if at == 16 then goto L16 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:POS_TIME_DECEL(24.0, 398.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L16::
        s:LABEL(16, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:FADE(0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
  [3] = {
    texture = "texture_3.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 256 } },
    },
    scripts = {
      { index = 0, offset = 0 },
    },
    labels = {
      [0] = { 18, -1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 18 then goto L18 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L18::
        s:LABEL(18, 0)
        s:SET_TRANSLATION(1111490560, 1119879168, 0)
        s:SET_ALPHA(0)
        s:FADE(255, 30)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:FADE(0, 30)
        s:at(60)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
