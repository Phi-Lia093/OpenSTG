-- stg8enm: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 32, 32 } },
      { id = 1, rect = { 32, 0, 32, 32 } },
      { id = 2, rect = { 64, 0, 32, 32 } },
      { id = 3, rect = { 96, 0, 32, 32 } },
      { id = 4, rect = { 128, 0, 32, 32 } },
      { id = 5, rect = { 160, 0, 32, 32 } },
      { id = 6, rect = { 192, 0, 32, 32 } },
      { id = 7, rect = { 224, 0, 32, 32 } },
      { id = 8, rect = { 0, 32, 32, 32 } },
      { id = 9, rect = { 32, 32, 32, 32 } },
      { id = 10, rect = { 64, 32, 32, 32 } },
      { id = 11, rect = { 96, 32, 32, 32 } },
      { id = 12, rect = { 128, 32, 32, 32 } },
      { id = 13, rect = { 160, 32, 32, 32 } },
      { id = 14, rect = { 192, 32, 32, 32 } },
      { id = 15, rect = { 224, 32, 32, 32 } },
      { id = 16, rect = { 256, 0, 32, 32 } },
      { id = 17, rect = { 288, 0, 32, 32 } },
      { id = 18, rect = { 320, 0, 32, 32 } },
      { id = 19, rect = { 352, 0, 32, 32 } },
      { id = 20, rect = { 384, 0, 32, 32 } },
      { id = 21, rect = { 416, 0, 32, 32 } },
      { id = 22, rect = { 448, 0, 32, 32 } },
      { id = 23, rect = { 480, 0, 32, 32 } },
      { id = 24, rect = { 256, 48, 32, 32 } },
      { id = 25, rect = { 128, 96, 32, 32 } },
      { id = 26, rect = { 0, 128, 32, 32 } },
      { id = 27, rect = { 32, 128, 32, 32 } },
      { id = 28, rect = { 64, 128, 32, 32 } },
      { id = 29, rect = { 96, 128, 32, 32 } },
      { id = 30, rect = { 128, 128, 32, 32 } },
      { id = 31, rect = { 160, 128, 32, 32 } },
      { id = 32, rect = { 192, 128, 32, 32 } },
      { id = 33, rect = { 224, 128, 32, 32 } },
      { id = 34, rect = { 0, 160, 32, 32 } },
      { id = 35, rect = { 32, 160, 32, 32 } },
      { id = 36, rect = { 64, 160, 32, 32 } },
      { id = 37, rect = { 96, 160, 32, 32 } },
      { id = 38, rect = { 128, 160, 32, 32 } },
      { id = 39, rect = { 160, 160, 32, 32 } },
      { id = 40, rect = { 192, 160, 32, 32 } },
      { id = 41, rect = { 224, 160, 32, 32 } },
      { id = 42, rect = { 256, 32, 48, 16 } },
      { id = 43, rect = { 1, 160, 30, 32 } },
      { id = 44, rect = { 33, 160, 30, 32 } },
      { id = 45, rect = { 65, 160, 30, 32 } },
      { id = 46, rect = { 97, 160, 30, 32 } },
      { id = 47, rect = { 129, 160, 30, 32 } },
      { id = 48, rect = { 161, 160, 30, 32 } },
      { id = 49, rect = { 193, 160, 30, 32 } },
      { id = 50, rect = { 225, 160, 30, 32 } },
      { id = 51, rect = { 129, 160, 30, 32 } },
      { id = 52, rect = { 161, 160, 30, 32 } },
      { id = 53, rect = { 193, 160, 30, 32 } },
      { id = 54, rect = { 225, 160, 30, 32 } },
      { id = 55, rect = { 0, 192, 80, 96 } },
      { id = 56, rect = { 80, 192, 80, 96 } },
      { id = 57, rect = { 160, 192, 80, 96 } },
      { id = 58, rect = { 0, 288, 80, 96 } },
      { id = 59, rect = { 80, 288, 80, 96 } },
      { id = 60, rect = { 160, 288, 80, 96 } },
      { id = 61, rect = { 0, 384, 80, 96 } },
      { id = 62, rect = { 80, 384, 80, 96 } },
      { id = 63, rect = { 160, 384, 80, 96 } },
      { id = 64, rect = { 0, 96, 64, 64 } },
      { id = 65, rect = { 384, 384, 128, 128 } },
      { id = 66, rect = { 256, 384, 128, 128 } },
      { id = 67, rect = { 256, 80, 64, 80 } },
      { id = 68, rect = { 320, 80, 64, 80 } },
      { id = 69, rect = { 384, 80, 64, 80 } },
      { id = 70, rect = { 448, 80, 64, 80 } },
      { id = 71, rect = { 256, 160, 64, 80 } },
      { id = 72, rect = { 256, 240, 64, 80 } },
      { id = 73, rect = { 320, 240, 64, 80 } },
      { id = 74, rect = { 384, 240, 64, 80 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 72 },
      { index = 2, offset = 136 },
      { index = 3, offset = 208 },
      { index = 4, offset = 328 },
      { index = 5, offset = 456 },
      { index = 6, offset = 528 },
      { index = 7, offset = 592 },
      { index = 8, offset = 664 },
      { index = 9, offset = 784 },
      { index = 10, offset = 912 },
      { index = 11, offset = 960 },
      { index = 12, offset = 1156 },
      { index = 13, offset = 1264 },
      { index = 14, offset = 1472 },
      { index = 15, offset = 1692 },
      { index = 16, offset = 1912 },
      { index = 17, offset = 2132 },
      { index = 18, offset = 2352 },
      { index = 19, offset = 2572 },
      { index = 20, offset = 2792 },
      { index = 21, offset = 3000 },
      { index = 22, offset = 3208 },
      { index = 23, offset = 3416 },
      { index = 24, offset = 3624 },
      { index = 25, offset = 3672 },
      { index = 26, offset = 3720 },
      { index = 27, offset = 3792 },
      { index = 28, offset = 3864 },
      { index = 29, offset = 3936 },
      { index = 30, offset = 4088 },
      { index = 31, offset = 4252 },
      { index = 32, offset = 4312 },
      { index = 128, offset = 4404 },
      { index = 129, offset = 4508 },
      { index = 130, offset = 4616 },
      { index = 131, offset = 4724 },
      { index = 132, offset = 4892 },
      { index = 133, offset = 5060 },
      { index = 134, offset = 5224 },
      { index = 135, offset = 5452 },
      { index = 136, offset = 5628 },
      { index = 137, offset = 5784 },
      { index = 138, offset = 6056 },
      { index = 139, offset = 6320 },
      { index = 140, offset = 6452 },
      { index = 141, offset = 6656 },
      { index = 142, offset = 6872 },
    },
    labels = {
      [0] = {  },
      [1] = {  },
      [2] = {  },
      [3] = {  },
      [4] = {  },
      [5] = {  },
      [6] = {  },
      [7] = {  },
      [8] = {  },
      [9] = {  },
      [10] = {  },
      [11] = { 1 },
      [12] = { 1 },
      [13] = { 2, 1 },
      [14] = { 1, 2 },
      [15] = { 1, 2 },
      [16] = { 1, 2 },
      [17] = { 1, 2 },
      [18] = { 1, 2 },
      [19] = { 1, 2 },
      [20] = { 1, 2 },
      [21] = { 1, 2 },
      [22] = { 1, 2 },
      [23] = { 1, 2 },
      [24] = {  },
      [25] = {  },
      [26] = {  },
      [27] = {  },
      [28] = {  },
      [29] = { 1 },
      [30] = { 1 },
      [31] = {  },
      [32] = {  },
      [33] = {  },
      [34] = {  },
      [35] = {  },
      [36] = {  },
      [37] = {  },
      [38] = {  },
      [39] = {  },
      [40] = {  },
      [41] = {  },
      [42] = {  },
      [43] = {  },
      [44] = {  },
      [45] = {  },
      [46] = {  },
      [47] = {  },
    },

    -- stream 0
    [0] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:at(4)
        s:SET_ACTIVE_SPRITE(1)
        s:at(8)
        s:SET_ACTIVE_SPRITE(2)
        s:at(12)
        s:SET_ACTIVE_SPRITE(3)
        s:at(16)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:at(8)
        s:SET_ACTIVE_SPRITE(6)
        s:at(16)
        s:SET_ACTIVE_SPRITE(5)
        s:at(24)
        s:SET_ACTIVE_SPRITE(4)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(7)
        s:at(8)
        s:SET_ACTIVE_SPRITE(6)
        s:at(16)
        s:SET_ACTIVE_SPRITE(5)
        s:at(24)
        s:SET_ACTIVE_SPRITE(4)
        s:EXIT()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:at(8)
        s:SET_ACTIVE_SPRITE(5)
        s:at(16)
        s:SET_ACTIVE_SPRITE(6)
        s:at(24)
        s:SET_ACTIVE_SPRITE(7)
        ::J48::
        s:SET_ACTIVE_SPRITE(0)
        s:at(28)
        s:SET_ACTIVE_SPRITE(1)
        s:at(32)
        s:SET_ACTIVE_SPRITE(2)
        s:at(36)
        s:SET_ACTIVE_SPRITE(3)
        s:at(40)
        s:SET_CLOCK(24, 48)
        goto J48
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(4)
        s:at(8)
        s:SET_ACTIVE_SPRITE(5)
        s:at(16)
        s:SET_ACTIVE_SPRITE(6)
        s:at(24)
        s:SET_ACTIVE_SPRITE(7)
        ::J56::
        s:SET_ACTIVE_SPRITE(0)
        s:at(28)
        s:SET_ACTIVE_SPRITE(1)
        s:at(32)
        s:SET_ACTIVE_SPRITE(2)
        s:at(36)
        s:SET_ACTIVE_SPRITE(3)
        s:at(40)
        s:SET_CLOCK(24, 56)
        goto J56
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:at(4)
        s:SET_ACTIVE_SPRITE(9)
        s:at(8)
        s:SET_ACTIVE_SPRITE(10)
        s:at(12)
        s:SET_ACTIVE_SPRITE(11)
        s:at(16)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(15)
        s:at(8)
        s:SET_ACTIVE_SPRITE(14)
        s:at(16)
        s:SET_ACTIVE_SPRITE(13)
        s:at(24)
        s:SET_ACTIVE_SPRITE(12)
        s:EXIT()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(15)
        s:at(8)
        s:SET_ACTIVE_SPRITE(14)
        s:at(16)
        s:SET_ACTIVE_SPRITE(13)
        s:at(24)
        s:SET_ACTIVE_SPRITE(12)
        s:EXIT()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(12)
        s:at(8)
        s:SET_ACTIVE_SPRITE(13)
        s:at(16)
        s:SET_ACTIVE_SPRITE(14)
        s:at(24)
        s:SET_ACTIVE_SPRITE(15)
        ::J48::
        s:SET_ACTIVE_SPRITE(8)
        s:at(28)
        s:SET_ACTIVE_SPRITE(9)
        s:at(32)
        s:SET_ACTIVE_SPRITE(10)
        s:at(36)
        s:SET_ACTIVE_SPRITE(11)
        s:at(40)
        s:SET_CLOCK(24, 48)
        goto J48
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 9
    [9] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(12)
        s:at(8)
        s:SET_ACTIVE_SPRITE(13)
        s:at(16)
        s:SET_ACTIVE_SPRITE(14)
        s:at(24)
        s:SET_ACTIVE_SPRITE(15)
        ::J56::
        s:SET_ACTIVE_SPRITE(8)
        s:at(28)
        s:SET_ACTIVE_SPRITE(9)
        s:at(32)
        s:SET_ACTIVE_SPRITE(10)
        s:at(36)
        s:SET_ACTIVE_SPRITE(11)
        s:at(40)
        s:SET_CLOCK(24, 56)
        goto J56
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 10
    [10] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, 1044975579)
        s:SET_ACTIVE_SPRITE(16)
        s:at(30000)
        s:EXIT()
    end,

    -- stream 11
    [11] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 0, 255)
        s:RAND_FLOAT({r = 1176260608}, 1086918619)
        s:SUB_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1078530011)
        s:SET_ROTATION(0, 0, {r = 1176260608})
        s:RAND_FLOAT({r = 1176260608}, 1065353216)
        s:ADD_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1065353216)
        s:SET_SCALE({r = 1176260608}, {r = 1176260608})
        s:at(60)
        s:STOP()
        ::L1::
        s:LABEL(1, 60)
        s:INTERP_ALPHA(40, 0, 0)
        s:EXIT_HIDE2()
    end,

    -- stream 12
    [12] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_AUTO_ROTATE(1)
        s:SET_SCALE(1065353216, 1073741824)
        s:SET_ACTIVE_SPRITE(42)
        s:SET_ZWRITE_DISABLE(1)
        s:STOP()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(40)
        s:EXIT_HIDE2()
    end,

    -- stream 13
    [13] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(24)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 0, 255)
        s:INTERP_COLOR(180, 255, 35980)
        s:STOP()
        s:at(1)
        ::L2::
        s:LABEL(2, 1)
        s:INTERP_COLOR(180, 255, 35980)
        s:SET_ALPHA(255)
        s:at(181)
        s:STOP()
        s:at(182)
        ::L1::
        s:LABEL(1, 182)
        s:INTERP_ALPHA(30, 0, 0)
        s:INTERP_SCALE_2(30, 0, 1077936128, 1077936128)
        s:at(212)
        s:EXIT_HIDE2()
    end,

    -- stream 14
    [14] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SUB_FLOAT_2({r = 1176260608}, 1055928808, 1048971922)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        ::J52::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 52)
        goto J52
        ::L1::
        s:LABEL(1, 30000)
        s:SET_ANGLE_VEL(0, 0, 0)
        ::J108::
        s:SET_ACTIVE_SPRITE(8)
        s:at(30004)
        s:SET_ACTIVE_SPRITE(9)
        s:at(30008)
        s:SET_ACTIVE_SPRITE(10)
        s:at(30012)
        s:SET_ACTIVE_SPRITE(11)
        s:at(30016)
        s:SET_CLOCK(30000, 108)
        goto J108
        ::L2::
        s:LABEL(2, 30016)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(30056)
        s:EXIT_HIDE2()
    end,

    -- stream 15
    [15] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:SUB_FLOAT_2({r = 1176260608}, 1064334710, 1048971922)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        ::J52::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 52)
        goto J52
        ::L1::
        s:LABEL(1, 30000)
        s:SET_ANGLE_VEL(0, 0, 0)
        ::J108::
        s:SET_ACTIVE_SPRITE(0)
        s:at(30004)
        s:SET_ACTIVE_SPRITE(1)
        s:at(30008)
        s:SET_ACTIVE_SPRITE(2)
        s:at(30012)
        s:SET_ACTIVE_SPRITE(3)
        s:at(30016)
        s:SET_CLOCK(30000, 108)
        goto J108
        ::L2::
        s:LABEL(2, 30016)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(30056)
        s:EXIT_HIDE2()
    end,

    -- stream 16
    [16] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:ADD_FLOAT_2({r = 1176260608}, 1056679030, 1036586971)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        ::J52::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 52)
        goto J52
        ::L1::
        s:LABEL(1, 30000)
        s:SET_ANGLE_VEL(0, 0, 0)
        ::J108::
        s:SET_ACTIVE_SPRITE(0)
        s:at(30004)
        s:SET_ACTIVE_SPRITE(1)
        s:at(30008)
        s:SET_ACTIVE_SPRITE(2)
        s:at(30012)
        s:SET_ACTIVE_SPRITE(3)
        s:at(30016)
        s:SET_CLOCK(30000, 108)
        goto J108
        ::L2::
        s:LABEL(2, 30016)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(30056)
        s:EXIT_HIDE2()
    end,

    -- stream 17
    [17] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:ADD_FLOAT_2({r = 1176260608}, 1051431628, 1036586971)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        ::J52::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 52)
        goto J52
        ::L1::
        s:LABEL(1, 30000)
        s:SET_ANGLE_VEL(0, 0, 0)
        ::J108::
        s:SET_ACTIVE_SPRITE(8)
        s:at(30004)
        s:SET_ACTIVE_SPRITE(9)
        s:at(30008)
        s:SET_ACTIVE_SPRITE(10)
        s:at(30012)
        s:SET_ACTIVE_SPRITE(11)
        s:at(30016)
        s:SET_CLOCK(30000, 108)
        goto J108
        ::L2::
        s:LABEL(2, 30016)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(30056)
        s:EXIT_HIDE2()
    end,

    -- stream 18
    [18] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(26)
        s:ADD_FLOAT_2({r = 1176260608}, 1059014184, 1036586971)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        ::J52::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 52)
        goto J52
        ::L1::
        s:LABEL(1, 30000)
        s:SET_ANGLE_VEL(0, 0, 0)
        ::J108::
        s:SET_ACTIVE_SPRITE(26)
        s:at(30004)
        s:SET_ACTIVE_SPRITE(27)
        s:at(30008)
        s:SET_ACTIVE_SPRITE(28)
        s:at(30012)
        s:SET_ACTIVE_SPRITE(29)
        s:at(30016)
        s:SET_CLOCK(30000, 108)
        goto J108
        ::L2::
        s:LABEL(2, 30016)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(30056)
        s:EXIT_HIDE2()
    end,

    -- stream 19
    [19] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:ADD_FLOAT_2({r = 1176260608}, 1065146630, 1036586971)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        ::J52::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 52)
        goto J52
        ::L1::
        s:LABEL(1, 30000)
        s:SET_ANGLE_VEL(0, 0, 0)
        ::J108::
        s:SET_ACTIVE_SPRITE(34)
        s:at(30004)
        s:SET_ACTIVE_SPRITE(35)
        s:at(30008)
        s:SET_ACTIVE_SPRITE(36)
        s:at(30012)
        s:SET_ACTIVE_SPRITE(37)
        s:at(30016)
        s:SET_CLOCK(30000, 108)
        goto J108
        ::L2::
        s:LABEL(2, 30016)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(30056)
        s:EXIT_HIDE2()
    end,

    -- stream 20
    [20] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 0, 255)
        s:SET_SCALE(0, 1084227584)
        s:INTERP_SCALE_2(30, 0, 1065353216, 1065353216)
        s:at(60)
        ::L1::
        s:LABEL(1, 60)
        ::J96::
        s:SET_ACTIVE_SPRITE(0)
        s:at(64)
        s:SET_ACTIVE_SPRITE(1)
        s:at(68)
        s:SET_ACTIVE_SPRITE(2)
        s:at(72)
        s:SET_ACTIVE_SPRITE(3)
        s:at(76)
        s:SET_CLOCK(60, 96)
        goto J96
        ::L2::
        s:LABEL(2, 76)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(116)
        s:EXIT_HIDE2()
    end,

    -- stream 21
    [21] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 0, 255)
        s:SET_SCALE(0, 1084227584)
        s:INTERP_SCALE_2(30, 0, 1065353216, 1065353216)
        s:at(60)
        ::L1::
        s:LABEL(1, 60)
        ::J96::
        s:SET_ACTIVE_SPRITE(8)
        s:at(64)
        s:SET_ACTIVE_SPRITE(9)
        s:at(68)
        s:SET_ACTIVE_SPRITE(10)
        s:at(72)
        s:SET_ACTIVE_SPRITE(11)
        s:at(76)
        s:SET_CLOCK(60, 96)
        goto J96
        ::L2::
        s:LABEL(2, 76)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(116)
        s:EXIT_HIDE2()
    end,

    -- stream 22
    [22] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(26)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 0, 255)
        s:SET_SCALE(0, 1084227584)
        s:INTERP_SCALE_2(30, 0, 1065353216, 1065353216)
        s:at(60)
        ::L1::
        s:LABEL(1, 60)
        ::J96::
        s:SET_ACTIVE_SPRITE(26)
        s:at(64)
        s:SET_ACTIVE_SPRITE(27)
        s:at(68)
        s:SET_ACTIVE_SPRITE(28)
        s:at(72)
        s:SET_ACTIVE_SPRITE(29)
        s:at(76)
        s:SET_CLOCK(60, 96)
        goto J96
        ::L2::
        s:LABEL(2, 76)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(116)
        s:EXIT_HIDE2()
    end,

    -- stream 23
    [23] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 0, 255)
        s:SET_SCALE(0, 1084227584)
        s:INTERP_SCALE_2(30, 0, 1065353216, 1065353216)
        s:at(60)
        ::L1::
        s:LABEL(1, 60)
        ::J96::
        s:SET_ACTIVE_SPRITE(34)
        s:at(64)
        s:SET_ACTIVE_SPRITE(35)
        s:at(68)
        s:SET_ACTIVE_SPRITE(36)
        s:at(72)
        s:SET_ACTIVE_SPRITE(37)
        s:at(76)
        s:SET_CLOCK(60, 96)
        goto J96
        ::L2::
        s:LABEL(2, 76)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(116)
        s:EXIT_HIDE2()
    end,

    -- stream 24
    [24] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, 1044975579)
        s:SET_ACTIVE_SPRITE(25)
        s:at(30000)
        s:EXIT()
    end,

    -- stream 25
    [25] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, -1102508069)
        s:SET_ACTIVE_SPRITE(25)
        s:at(30000)
        s:EXIT()
    end,

    -- stream 26
    [26] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(43)
        s:at(3)
        s:SET_ACTIVE_SPRITE(44)
        s:at(6)
        s:SET_ACTIVE_SPRITE(45)
        s:at(9)
        s:SET_ACTIVE_SPRITE(46)
        s:at(12)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 27
    [27] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(47)
        s:at(3)
        s:SET_ACTIVE_SPRITE(48)
        s:at(6)
        s:SET_ACTIVE_SPRITE(49)
        s:at(9)
        s:SET_ACTIVE_SPRITE(50)
        s:at(12)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 28
    [28] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(51)
        s:at(3)
        s:SET_ACTIVE_SPRITE(52)
        s:at(6)
        s:SET_ACTIVE_SPRITE(53)
        s:at(9)
        s:SET_ACTIVE_SPRITE(54)
        s:at(12)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 29
    [29] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_AUTO_ROTATE(1)
        s:SET_SCALE(1065353216, 1073741824)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(42)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(30, 0, 255)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 30)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 30
    [30] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_AUTO_ROTATE(1)
        s:SET_SCALE(1065353216, 1073741824)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(42)
        s:SET_COLOR(16747775)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(30, 0, 140)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 30)
        s:INTERP_ALPHA(40, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 31
    [31] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(64)
        s:SET_BLEND(1)
        s:SET_ANGLE_VEL(0, 0, 1029076816)
        s:at(10000)
        s:EXIT_HIDE2()
    end,

    -- stream 32
    [32] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(64)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(80, 0, 255)
        s:SET_ANGLE_VEL(0, 0, 1029076816)
        s:at(10000)
        s:EXIT_HIDE2()
    end,

    -- stream 33
    [33] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_ACTIVE_SPRITE(55)
        ::J24::
        s:INTERP_POS(24, 4, 0, -1065353216, 0)
        s:at(24)
        s:INTERP_POS(24, 4, 0, 1082130432, 0)
        s:at(48)
        s:SET_CLOCK(0, 24)
        goto J24
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 34
    [34] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:INTERP_POS(24, 4, 0, 0, 0)
        s:SET_ACTIVE_SPRITE(61)
        s:at(3)
        s:SET_ACTIVE_SPRITE(62)
        s:at(6)
        s:SET_ACTIVE_SPRITE(63)
        ::J76::
        s:NOP()
        s:at(30006)
        s:SET_CLOCK(6, 76)
        goto J76
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 35
    [35] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:INTERP_POS(24, 4, 0, 0, 0)
        s:SET_ACTIVE_SPRITE(58)
        s:at(3)
        s:SET_ACTIVE_SPRITE(59)
        s:at(6)
        s:SET_ACTIVE_SPRITE(60)
        ::J76::
        s:NOP()
        s:at(30006)
        s:SET_CLOCK(6, 76)
        goto J76
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 36
    [36] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:INTERP_POS(24, 4, 0, 0, 0)
        s:SET_ACTIVE_SPRITE(61)
        s:at(3)
        s:SET_ACTIVE_SPRITE(62)
        s:at(6)
        s:SET_ACTIVE_SPRITE(63)
        s:SET_ACTIVE_SPRITE(55)
        ::J88::
        s:INTERP_POS(24, 4, 0, -1065353216, 0)
        s:at(30)
        s:INTERP_POS(24, 4, 0, 1082130432, 0)
        s:at(54)
        s:SET_CLOCK(6, 88)
        goto J88
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 37
    [37] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:INTERP_POS(24, 4, 0, 0, 0)
        s:SET_ACTIVE_SPRITE(58)
        s:at(3)
        s:SET_ACTIVE_SPRITE(59)
        s:at(6)
        s:SET_ACTIVE_SPRITE(60)
        s:SET_ACTIVE_SPRITE(55)
        ::J88::
        s:INTERP_POS(24, 4, 0, -1065353216, 0)
        s:at(30)
        s:INTERP_POS(24, 4, 0, 1082130432, 0)
        s:at(54)
        s:SET_CLOCK(6, 88)
        goto J88
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 38
    [38] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:INTERP_POS(24, 4, 0, 0, 0)
        s:SET_ACTIVE_SPRITE(55)
        s:at(5)
        s:SET_ACTIVE_SPRITE(56)
        s:at(10)
        s:SET_ACTIVE_SPRITE(57)
        s:at(24)
        s:NOP()
        ::J84::
        s:INTERP_POS(24, 4, 0, -1065353216, 0)
        s:at(48)
        s:INTERP_POS(24, 4, 0, 1082130432, 0)
        s:at(72)
        s:SET_CLOCK(24, 84)
        goto J84
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 39
    [39] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_USE_OFFSET(1)
        ::J52::
        s:SET_ACTIVE_SPRITE(67)
        s:INTERP_POS(24, 4, 0, -1065353216, 0)
        s:at(6)
        s:SET_ACTIVE_SPRITE(68)
        s:at(12)
        s:SET_ACTIVE_SPRITE(69)
        s:at(18)
        s:SET_ACTIVE_SPRITE(70)
        s:at(24)
        s:SET_ACTIVE_SPRITE(67)
        s:INTERP_POS(24, 4, 0, 1082130432, 0)
        s:at(30)
        s:SET_ACTIVE_SPRITE(68)
        s:at(36)
        s:SET_ACTIVE_SPRITE(69)
        s:at(42)
        s:SET_ACTIVE_SPRITE(70)
        s:at(48)
        s:SET_CLOCK(0, 52)
        goto J52
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 40
    [40] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_USE_OFFSET(1)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(72)
        s:at(3)
        s:SET_ACTIVE_SPRITE(73)
        s:at(6)
        s:SET_ACTIVE_SPRITE(74)
        s:INTERP_ROTATE(12, 1, 0, 0, 1078530011)
        s:at(18)
        s:SET_ANGLE_VEL(0, 0, 1053364187)
        ::J144::
        s:NOP()
        s:at(30018)
        s:SET_CLOCK(18, 144)
        goto J144
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 41
    [41] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ACTIVE_SPRITE(72)
        s:at(3)
        s:SET_ACTIVE_SPRITE(73)
        s:at(6)
        s:SET_ACTIVE_SPRITE(74)
        s:INTERP_ROTATE(12, 1, 0, 0, -1068953637)
        s:at(18)
        s:SET_ANGLE_VEL(0, 0, -1094119461)
        ::J124::
        s:NOP()
        s:at(30018)
        s:SET_CLOCK(18, 124)
        goto J124
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 42
    [42] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(74)
        s:at(3)
        s:SET_ACTIVE_SPRITE(73)
        s:at(6)
        s:SET_ACTIVE_SPRITE(72)
        s:SET_USE_OFFSET(1)
        ::J96::
        s:SET_ACTIVE_SPRITE(67)
        s:INTERP_POS(24, 4, 0, -1065353216, 0)
        s:at(12)
        s:SET_ACTIVE_SPRITE(68)
        s:at(18)
        s:SET_ACTIVE_SPRITE(69)
        s:at(24)
        s:SET_ACTIVE_SPRITE(70)
        s:at(30)
        s:SET_ACTIVE_SPRITE(67)
        s:INTERP_POS(24, 4, 0, 1082130432, 0)
        s:at(36)
        s:SET_ACTIVE_SPRITE(68)
        s:at(42)
        s:SET_ACTIVE_SPRITE(69)
        s:at(48)
        s:SET_ACTIVE_SPRITE(70)
        s:at(54)
        s:SET_CLOCK(6, 96)
        goto J96
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 43
    [43] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ACTIVE_SPRITE(74)
        s:at(3)
        s:SET_ACTIVE_SPRITE(73)
        s:at(6)
        s:SET_ACTIVE_SPRITE(72)
        s:SET_USE_OFFSET(1)
        ::J88::
        s:SET_ACTIVE_SPRITE(67)
        s:INTERP_POS(24, 4, 0, -1065353216, 0)
        s:at(12)
        s:SET_ACTIVE_SPRITE(68)
        s:at(18)
        s:SET_ACTIVE_SPRITE(69)
        s:at(24)
        s:SET_ACTIVE_SPRITE(70)
        s:at(30)
        s:SET_ACTIVE_SPRITE(67)
        s:INTERP_POS(24, 4, 0, 1082130432, 0)
        s:at(36)
        s:SET_ACTIVE_SPRITE(68)
        s:at(42)
        s:SET_ACTIVE_SPRITE(69)
        s:at(48)
        s:SET_ACTIVE_SPRITE(70)
        s:at(54)
        s:SET_CLOCK(6, 88)
        goto J88
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 44
    [44] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ACTIVE_SPRITE(71)
        ::J52::
        s:INTERP_POS(12, 4, 0, -1065353216, 0)
        s:at(12)
        s:INTERP_POS(12, 4, 0, 1082130432, 0)
        s:at(24)
        s:SET_CLOCK(0, 52)
        goto J52
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 45
    [45] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ACTIVE_SPRITE(65)
        s:SET_ALPHA(0)
        s:at(200)
        s:INTERP_ALPHA(80, 0, 128)
        s:at(280)
        s:NOP()
        ::J92::
        s:INTERP_ALPHA(200, 4, 64)
        s:INTERP_SCALE_2(200, 4, 1069547520, 1069547520)
        s:at(480)
        s:INTERP_ALPHA(200, 4, 192)
        s:INTERP_SCALE_2(200, 4, 1065353216, 1065353216)
        s:at(680)
        s:SET_CLOCK(280, 92)
        goto J92
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 46
    [46] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, -1137995098)
        s:SET_ACTIVE_SPRITE(66)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:at(200)
        s:INTERP_ALPHA(80, 0, 128)
        s:at(280)
        s:NOP()
        ::J104::
        s:INTERP_ALPHA(200, 4, 128)
        s:INTERP_SCALE_2(200, 4, 1073741824, 1073741824)
        s:at(480)
        s:INTERP_ALPHA(200, 4, 255)
        s:INTERP_SCALE_2(200, 4, 1069547520, 1069547520)
        s:at(680)
        s:SET_CLOCK(280, 104)
        goto J104
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 47
    [47] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ACTIVE_SPRITE(57)
        s:INTERP_ALPHA(120, 0, 128)
        ::J72::
        s:INTERP_POS(12, 4, 0, -1065353216, 0)
        s:at(12)
        s:INTERP_POS(12, 4, 0, 1082130432, 0)
        s:at(24)
        s:SET_CLOCK(0, 72)
        goto J72
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
