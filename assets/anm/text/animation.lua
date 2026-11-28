-- text: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "@",
    width = 512, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 18, 320, 17 } },
      { id = 1, rect = { 0, 36, 320, 17 } },
      { id = 2, rect = { 0, 54, 256, 17 } },
      { id = 3, rect = { 0, 72, 256, 17 } },
      { id = 4, rect = { 0, 90, 320, 17 } },
      { id = 5, rect = { 0, 108, 320, 17 } },
      { id = 6, rect = { 0, 0, 320, 17 } },
      { id = 7, rect = { 0, 18, 320, 17 } },
      { id = 8, rect = { 0, 36, 320, 17 } },
      { id = 9, rect = { 0, 54, 320, 17 } },
      { id = 10, rect = { 0, 72, 320, 17 } },
      { id = 11, rect = { 0, 90, 320, 17 } },
      { id = 12, rect = { 0, 108, 320, 17 } },
      { id = 13, rect = { 0, 126, 320, 17 } },
      { id = 14, rect = { 0, 144, 320, 17 } },
      { id = 15, rect = { 0, 162, 320, 17 } },
      { id = 16, rect = { 0, 180, 320, 17 } },
      { id = 17, rect = { 0, 198, 320, 17 } },
      { id = 18, rect = { 0, 216, 320, 17 } },
      { id = 19, rect = { 0, 234, 320, 17 } },
      { id = 20, rect = { 0, 252, 320, 17 } },
      { id = 21, rect = { 0, 0, 512, 17 } },
      { id = 22, rect = { 0, 18, 512, 17 } },
      { id = 23, rect = { 0, 36, 512, 17 } },
      { id = 24, rect = { 0, 54, 512, 17 } },
      { id = 25, rect = { 0, 72, 512, 17 } },
      { id = 26, rect = { 0, 90, 512, 17 } },
      { id = 27, rect = { 0, 108, 512, 17 } },
      { id = 28, rect = { 0, 126, 512, 17 } },
      { id = 29, rect = { 0, 144, 512, 17 } },
      { id = 30, rect = { 0, 162, 512, 17 } },
      { id = 31, rect = { 0, 180, 512, 17 } },
      { id = 32, rect = { 0, 198, 512, 17 } },
      { id = 33, rect = { 0, 216, 512, 17 } },
      { id = 34, rect = { 0, 234, 512, 17 } },
      { id = 35, rect = { 0, 252, 512, 17 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 84 },
      { index = 2, offset = 168 },
      { index = 3, offset = 260 },
      { index = 4, offset = 352 },
      { index = 5, offset = 556 },
      { index = 6, offset = 764 },
      { index = 7, offset = 896 },
      { index = 8, offset = 1108 },
      { index = 9, offset = 1320 },
      { index = 10, offset = 1532 },
      { index = 11, offset = 1744 },
      { index = 12, offset = 1956 },
      { index = 13, offset = 2168 },
      { index = 14, offset = 2380 },
      { index = 15, offset = 2592 },
      { index = 16, offset = 2724 },
      { index = 17, offset = 2856 },
      { index = 18, offset = 2988 },
      { index = 19, offset = 3120 },
      { index = 20, offset = 3252 },
      { index = 21, offset = 3384 },
      { index = 22, offset = 3516 },
      { index = 23, offset = 3648 },
      { index = 24, offset = 3780 },
      { index = 25, offset = 3912 },
      { index = 26, offset = 4044 },
      { index = 27, offset = 4176 },
      { index = 28, offset = 4308 },
      { index = 29, offset = 4440 },
    },
    labels = {
      [0] = {  },
      [1] = {  },
      [2] = {  },
      [3] = {  },
      [4] = { 1 },
      [5] = { 1 },
      [6] = { 1, 2 },
      [7] = { 1, 2, -1 },
      [8] = { 1, 2, -1 },
      [9] = { 1, 2, -1 },
      [10] = { 1, 2, -1 },
      [11] = { 1, 2, -1 },
      [12] = { 1, 2, -1 },
      [13] = { 1, 2, -1 },
      [14] = { 1, 2, -1 },
      [15] = { 2, 1 },
      [16] = { 2, 1 },
      [17] = { 2, 1 },
      [18] = { 2, 1 },
      [19] = { 2, 1 },
      [20] = { 2, 1 },
      [21] = { 2, 1 },
      [22] = { 2, 1 },
      [23] = { 2, 1 },
      [24] = { 2, 1 },
      [25] = { 2, 1 },
      [26] = { 2, 1 },
      [27] = { 2, 1 },
      [28] = { 2, 1 },
      [29] = { 2, 1 },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_TRANSLATION(1116733440, 1136852992, 0)
        s:SET_ALPHA(0)
        s:FADE(255, 12)
        s:at(12)
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:NOP22()
        s:SET_TRANSLATION(1116733440, 1137508352, 0)
        s:SET_ALPHA(0)
        s:FADE(255, 12)
        s:at(12)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_TRANSLATION(1132986368, 1135607808, 0)
        s:SET_ALPHA(0)
        s:FADE(240, 60)
        s:at(360)
        s:FADE(0, 60)
        s:at(420)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_TRANSLATION(1132986368, 1136132096, 0)
        s:SET_ALPHA(0)
        s:FADE(240, 60)
        s:at(360)
        s:FADE(0, 60)
        s:at(420)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_TRANSLATION(1129316352, 1135345664, 0)
        s:SET_ALPHA(0)
        s:FADE(255, 30)
        s:SET_SCALE(1077936128, 1077936128)
        s:INTERP_SCALE_2(30, 0, 1065353216, 1065353216)
        s:at(100)
        s:INTERP_POS(30, 4, 1129316352, 1138491392, 0)
        s:at(130)
        s:STOP()
        ::L1::
        s:LABEL(1, 130)
        s:INTERP_POS(30, 4, -1023410176, 1138491392, 0)
        s:at(160)
        s:EXIT_HIDE2()
    end,

    -- stream 5
    [5] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1132462080, 1134297088, 0)
        s:SET_ALPHA(0)
        s:FADE(255, 30)
        s:SET_SCALE(1077936128, 1077936128)
        s:SET_SCALE_SPEED(-1115125623, -1115125623)
        s:at(30)
        s:SET_SCALE_SPEED(0, 0)
        s:SET_SCALE(1065353216, 1065353216)
        s:at(100)
        s:POS_TIME_DECEL(256.0, 40.0, 0.0, 30)
        s:at(130)
        s:STOP()
        ::L1::
        s:LABEL(1, 130)
        s:POS_TIME_DECEL(576.0, 40.0, 0.0, 30)
        s:at(160)
        s:EXIT_HIDE2()
    end,

    -- stream 6
    [6] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(21)
        s:SET_TRANSLATION(1134559232, 1138753536, 0)
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(80)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 7
    [7] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(21)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1134559232, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:WAIT(0)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 128)
        s:at(80)
        s:STOP_HIDE()
        ::Lm1::
        s:LABEL(-1, 80)
        s:WAIT(0)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(110)
        s:EXIT_HIDE2()
    end,

    -- stream 8
    [8] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(22)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1135083520, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:WAIT(4)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 128)
        s:at(80)
        s:STOP_HIDE()
        ::Lm1::
        s:LABEL(-1, 80)
        s:WAIT(5)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(110)
        s:EXIT_HIDE2()
    end,

    -- stream 9
    [9] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(23)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1135607808, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:WAIT(8)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 128)
        s:at(80)
        s:STOP_HIDE()
        ::Lm1::
        s:LABEL(-1, 80)
        s:WAIT(10)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(110)
        s:EXIT_HIDE2()
    end,

    -- stream 10
    [10] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(24)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136132096, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:WAIT(12)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 128)
        s:at(80)
        s:STOP_HIDE()
        ::Lm1::
        s:LABEL(-1, 80)
        s:WAIT(15)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(110)
        s:EXIT_HIDE2()
    end,

    -- stream 11
    [11] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(25)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:WAIT(16)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 128)
        s:at(80)
        s:STOP_HIDE()
        ::Lm1::
        s:LABEL(-1, 80)
        s:WAIT(20)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(110)
        s:EXIT_HIDE2()
    end,

    -- stream 12
    [12] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(26)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1137180672, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:WAIT(20)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 128)
        s:at(80)
        s:STOP_HIDE()
        ::Lm1::
        s:LABEL(-1, 80)
        s:WAIT(25)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(110)
        s:EXIT_HIDE2()
    end,

    -- stream 13
    [13] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(27)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1137704960, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:WAIT(24)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 128)
        s:at(80)
        s:STOP_HIDE()
        ::Lm1::
        s:LABEL(-1, 80)
        s:WAIT(30)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(110)
        s:EXIT_HIDE2()
    end,

    -- stream 14
    [14] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(28)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1138229248, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:WAIT(28)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 128)
        s:at(80)
        s:STOP_HIDE()
        ::Lm1::
        s:LABEL(-1, 80)
        s:WAIT(35)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(110)
        s:EXIT_HIDE2()
    end,

    -- stream 15
    [15] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(21)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 16
    [16] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(22)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 17
    [17] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(23)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 18
    [18] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(24)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 19
    [19] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(25)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 20
    [20] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(26)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 21
    [21] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(27)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 22
    [22] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(28)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 23
    [23] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(29)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 24
    [24] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(30)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 25
    [25] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(31)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 26
    [26] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(32)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 27
    [27] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(33)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 28
    [28] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 29
    [29] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(35)
        s:NOP22()
        s:SET_TRANSLATION(1115684864, 1136656384, 0)
        s:SET_ALPHA(0)
        ::L2::
        s:LABEL(2, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(10, 0, 255)
        s:at(11)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
