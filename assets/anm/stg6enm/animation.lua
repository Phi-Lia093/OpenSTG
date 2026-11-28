-- stg6enm: how the animation is put together and what it does.
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
      { id = 55, rect = { 0, 192, 64, 64 } },
      { id = 56, rect = { 256, 96, 48, 64 } },
      { id = 57, rect = { 304, 96, 48, 64 } },
      { id = 58, rect = { 352, 96, 48, 64 } },
      { id = 59, rect = { 400, 96, 48, 64 } },
      { id = 60, rect = { 256, 160, 48, 64 } },
      { id = 61, rect = { 304, 160, 48, 64 } },
      { id = 62, rect = { 352, 160, 48, 64 } },
      { id = 63, rect = { 400, 160, 48, 64 } },
      { id = 64, rect = { 256, 224, 48, 64 } },
      { id = 65, rect = { 304, 224, 48, 64 } },
      { id = 66, rect = { 352, 224, 48, 64 } },
      { id = 67, rect = { 400, 224, 48, 64 } },
      { id = 68, rect = { 256, 288, 48, 80 } },
      { id = 69, rect = { 304, 288, 48, 80 } },
      { id = 70, rect = { 352, 288, 48, 80 } },
      { id = 71, rect = { 400, 288, 48, 80 } },
      { id = 72, rect = { 256, 368, 72, 80 } },
      { id = 73, rect = { 328, 368, 72, 80 } },
      { id = 74, rect = { 400, 368, 72, 80 } },
      { id = 75, rect = { 0, 256, 48, 64 } },
      { id = 76, rect = { 48, 256, 48, 64 } },
      { id = 77, rect = { 96, 256, 48, 64 } },
      { id = 78, rect = { 0, 240, 256, 16 } },
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
      { index = 30, offset = 4100 },
      { index = 31, offset = 4252 },
      { index = 128, offset = 4416 },
      { index = 129, offset = 4684 },
      { index = 130, offset = 4796 },
      { index = 131, offset = 4908 },
      { index = 132, offset = 5244 },
      { index = 133, offset = 5580 },
      { index = 134, offset = 5692 },
      { index = 135, offset = 5792 },
      { index = 136, offset = 5912 },
      { index = 137, offset = 6020 },
      { index = 138, offset = 6096 },
      { index = 139, offset = 6172 },
      { index = 140, offset = 6256 },
      { index = 141, offset = 6340 },
      { index = 142, offset = 6440 },
      { index = 143, offset = 6548 },
      { index = 144, offset = 6672 },
      { index = 145, offset = 6816 },
      { index = 146, offset = 6916 },
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
      [31] = { 1 },
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
      [48] = {  },
      [49] = {  },
      [50] = {  },
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
        s:SUB_FLOAT_2({r = 1176260608}, 1064869110, 1048971922)
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
        s:SUB_FLOAT_2({r = 1176260608}, 1048094315, 1048971922)
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
        s:ADD_FLOAT_2({r = 1176260608}, 1060661373, 1036586971)
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
        s:ADD_FLOAT_2({r = 1176260608}, 1062404200, 1036586971)
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
        s:ADD_FLOAT_2({r = 1176260608}, 1042510737, 1036586971)
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
        s:ADD_FLOAT_2({r = 1176260608}, 1050455650, 1036586971)
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
        s:SET_ACTIVE_SPRITE(55)
        s:SET_SCALE(1036831949, 1086324736)
        s:SET_ALPHA(64)
        s:INTERP_SCALE_2(16, 4, 1065353216, 1065353216)
        s:INTERP_ALPHA(16, 0, 255)
        s:at(16)
        s:STOP()
        ::L1::
        s:LABEL(1, 16)
        s:INTERP_SCALE_2(16, 4, 1036831949, 1086324736)
        s:INTERP_ALPHA(16, 0, 64)
        s:at(32)
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

    -- stream 31
    [31] = function(s)
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

    -- stream 32
    [32] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        ::J12::
        s:INTERP_POS(64, 4, 0, 1090519040, 0)
        s:DIV_2({r = 10008}, 64, 32)
        ::J60::
        s:SET_ACTIVE_SPRITE(56)
        s:at(8)
        s:SET_ACTIVE_SPRITE(57)
        s:at(16)
        s:SET_ACTIVE_SPRITE(58)
        s:at(24)
        s:SET_ACTIVE_SPRITE(59)
        s:at(32)
        if s:LOOP_BACK({r = 10008}, 0, 60) then
            goto J60
        end
        s:INTERP_POS(64, 4, 0, -1056964608, 0)
        s:DIV_2({r = 10008}, 64, 32)
        ::J176::
        s:SET_ACTIVE_SPRITE(56)
        s:at(40)
        s:SET_ACTIVE_SPRITE(57)
        s:at(48)
        s:SET_ACTIVE_SPRITE(58)
        s:at(56)
        s:SET_ACTIVE_SPRITE(59)
        s:at(64)
        if s:LOOP_BACK({r = 10008}, 32, 176) then
            goto J176
        end
        s:SET_CLOCK(0, 12)
        goto J12
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 33
    [33] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(60)
        s:at(3)
        s:SET_ACTIVE_SPRITE(61)
        s:at(6)
        s:SET_ACTIVE_SPRITE(62)
        s:at(9)
        s:SET_ACTIVE_SPRITE(63)
        ::J80::
        s:NOP()
        s:at(30009)
        s:SET_CLOCK(9, 80)
        goto J80
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 34
    [34] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(64)
        s:at(3)
        s:SET_ACTIVE_SPRITE(65)
        s:at(6)
        s:SET_ACTIVE_SPRITE(66)
        s:at(9)
        s:SET_ACTIVE_SPRITE(67)
        ::J80::
        s:NOP()
        s:at(30009)
        s:SET_CLOCK(9, 80)
        goto J80
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 35
    [35] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(63)
        s:at(3)
        s:SET_ACTIVE_SPRITE(62)
        s:at(6)
        s:SET_ACTIVE_SPRITE(61)
        s:at(9)
        s:SET_ACTIVE_SPRITE(60)
        ::J80::
        s:INTERP_POS(64, 4, 0, 1090519040, 0)
        s:DIV_2({r = 10008}, 64, 32)
        ::J128::
        s:SET_ACTIVE_SPRITE(56)
        s:at(17)
        s:SET_ACTIVE_SPRITE(57)
        s:at(25)
        s:SET_ACTIVE_SPRITE(58)
        s:at(33)
        s:SET_ACTIVE_SPRITE(59)
        s:at(41)
        if s:LOOP_BACK({r = 10008}, 9, 128) then
            goto J128
        end
        s:INTERP_POS(64, 4, 0, -1056964608, 0)
        s:DIV_2({r = 10008}, 64, 32)
        ::J244::
        s:SET_ACTIVE_SPRITE(56)
        s:at(49)
        s:SET_ACTIVE_SPRITE(57)
        s:at(57)
        s:SET_ACTIVE_SPRITE(58)
        s:at(65)
        s:SET_ACTIVE_SPRITE(59)
        s:at(73)
        if s:LOOP_BACK({r = 10008}, 41, 244) then
            goto J244
        end
        s:SET_CLOCK(9, 80)
        goto J80
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 36
    [36] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(67)
        s:at(3)
        s:SET_ACTIVE_SPRITE(66)
        s:at(6)
        s:SET_ACTIVE_SPRITE(65)
        s:at(9)
        s:SET_ACTIVE_SPRITE(64)
        ::J80::
        s:INTERP_POS(64, 4, 0, 1090519040, 0)
        s:DIV_2({r = 10008}, 64, 32)
        ::J128::
        s:SET_ACTIVE_SPRITE(56)
        s:at(17)
        s:SET_ACTIVE_SPRITE(57)
        s:at(25)
        s:SET_ACTIVE_SPRITE(58)
        s:at(33)
        s:SET_ACTIVE_SPRITE(59)
        s:at(41)
        if s:LOOP_BACK({r = 10008}, 9, 128) then
            goto J128
        end
        s:INTERP_POS(64, 4, 0, -1056964608, 0)
        s:DIV_2({r = 10008}, 64, 32)
        ::J244::
        s:SET_ACTIVE_SPRITE(56)
        s:at(49)
        s:SET_ACTIVE_SPRITE(57)
        s:at(57)
        s:SET_ACTIVE_SPRITE(58)
        s:at(65)
        s:SET_ACTIVE_SPRITE(59)
        s:at(73)
        if s:LOOP_BACK({r = 10008}, 41, 244) then
            goto J244
        end
        s:SET_CLOCK(9, 80)
        goto J80
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 37
    [37] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, -1048576000, 0)
        s:SET_ACTIVE_SPRITE(68)
        s:at(4)
        s:SET_ACTIVE_SPRITE(69)
        s:at(8)
        s:SET_ACTIVE_SPRITE(70)
        s:at(12)
        s:SET_ACTIVE_SPRITE(71)
        ::J80::
        s:NOP()
        s:at(30012)
        s:SET_CLOCK(12, 80)
        goto J80
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 38
    [38] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(1103101952, -1048576000, 0)
        s:SET_ACTIVE_SPRITE(72)
        s:at(4)
        s:SET_ACTIVE_SPRITE(73)
        s:at(8)
        s:SET_ACTIVE_SPRITE(74)
        ::J68::
        s:NOP()
        s:at(30008)
        s:SET_CLOCK(8, 68)
        goto J68
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 39
    [39] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:FLIP_X()
        s:SET_TRANSLATION(0, -1048576000, 0)
        s:SET_ACTIVE_SPRITE(68)
        s:at(4)
        s:SET_ACTIVE_SPRITE(69)
        s:at(8)
        s:SET_ACTIVE_SPRITE(70)
        s:at(12)
        s:SET_ACTIVE_SPRITE(71)
        ::J88::
        s:NOP()
        s:at(30012)
        s:SET_CLOCK(12, 88)
        goto J88
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 40
    [40] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:FLIP_X()
        s:SET_TRANSLATION(-1044381696, -1048576000, 0)
        s:SET_ACTIVE_SPRITE(72)
        s:at(4)
        s:SET_ACTIVE_SPRITE(73)
        s:at(8)
        s:SET_ACTIVE_SPRITE(74)
        ::J76::
        s:NOP()
        s:at(30008)
        s:SET_CLOCK(8, 76)
        goto J76
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 41
    [41] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, -1048576000, 0)
        s:SET_ACTIVE_SPRITE(68)
        ::J44::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 44)
        goto J44
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 42
    [42] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(1103101952, -1048576000, 0)
        s:SET_ACTIVE_SPRITE(72)
        ::J44::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 44)
        goto J44
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 43
    [43] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:FLIP_X()
        s:SET_TRANSLATION(0, -1048576000, 0)
        s:SET_ACTIVE_SPRITE(68)
        ::J52::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 52)
        goto J52
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 44
    [44] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:FLIP_X()
        s:SET_TRANSLATION(-1044381696, -1048576000, 0)
        s:SET_ACTIVE_SPRITE(72)
        ::J52::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 52)
        goto J52
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 45
    [45] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(75)
        s:at(3)
        s:SET_ACTIVE_SPRITE(76)
        s:at(6)
        s:SET_ACTIVE_SPRITE(77)
        ::J68::
        s:NOP()
        s:at(30006)
        s:SET_CLOCK(6, 68)
        goto J68
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 46
    [46] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(75)
        s:at(3)
        s:SET_ACTIVE_SPRITE(76)
        s:at(6)
        s:SET_ACTIVE_SPRITE(77)
        ::J76::
        s:NOP()
        s:at(30006)
        s:SET_CLOCK(6, 76)
        goto J76
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 47
    [47] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(78)
        s:SET_ALPHA(0)
        s:SET_SCALE(1069547520, 0)
        s:INTERP_ALPHA(8, 0, 255)
        s:INTERP_SCALE_2(8, 0, 1069547520, 1065353216)
        s:at(40)
        s:INTERP_SCALE_2(16, 0, 1071644672, 0)
        s:at(56)
        s:EXIT_HIDE2()
    end,

    -- stream 48
    [48] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(78)
        s:SET_ALPHA(0)
        s:SET_SCALE(1070071808, 1090519040)
        s:SET_ROTATION(0, 0, 1070141403)
        s:INTERP_ALPHA(30, 0, 255)
        s:INTERP_SCALE_2(30, 0, 1069547520, 1077936128)
        s:at(160)
        s:INTERP_SCALE_2(16, 0, 1069547520, 0)
        s:at(176)
        s:EXIT_HIDE2()
    end,

    -- stream 49
    [49] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(78)
        s:SET_ALPHA(0)
        s:SET_SCALE(1070071808, 1090519040)
        s:INTERP_ALPHA(30, 0, 255)
        s:INTERP_SCALE_2(30, 0, 1069547520, 1077936128)
        s:at(30000)
        s:EXIT_HIDE2()
    end,

    -- stream 50
    [50] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(78)
        s:SET_ALPHA(0)
        s:SET_SCALE(1065353216, 0)
        s:INTERP_ALPHA(8, 0, 255)
        s:INTERP_SCALE_2(8, 0, 1065353216, 1065353216)
        s:at(40)
        s:INTERP_SCALE_2(16, 0, 1067030938, 0)
        s:at(56)
        s:EXIT_HIDE2()
    end,
  },
  [1] = {
    texture = "texture_1.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 48, 80 } },
      { id = 1, rect = { 48, 0, 48, 80 } },
      { id = 2, rect = { 96, 0, 48, 80 } },
      { id = 3, rect = { 144, 0, 48, 96 } },
      { id = 4, rect = { 0, 80, 48, 80 } },
      { id = 5, rect = { 48, 80, 48, 80 } },
      { id = 6, rect = { 96, 80, 48, 80 } },
      { id = 7, rect = { 0, 256, 512, 256 } },
      { id = 8, rect = { 0, 160, 208, 48 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 104 },
      { index = 2, offset = 204 },
      { index = 3, offset = 312 },
      { index = 4, offset = 472 },
      { index = 5, offset = 640 },
      { index = 6, offset = 740 },
      { index = 7, offset = 944 },
      { index = 8, offset = 1156 },
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
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_ACTIVE_SPRITE(0)
        ::J24::
        s:INTERP_POS(128, 4, 0, 1090519040, 0)
        s:at(128)
        s:INTERP_POS(128, 4, 0, -1056964608, 0)
        s:at(256)
        s:SET_CLOCK(0, 24)
        goto J24
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(4)
        s:at(3)
        s:SET_ACTIVE_SPRITE(5)
        s:at(6)
        s:SET_ACTIVE_SPRITE(6)
        ::J68::
        s:NOP()
        s:at(30006)
        s:SET_CLOCK(6, 68)
        goto J68
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(4)
        s:at(3)
        s:SET_ACTIVE_SPRITE(5)
        s:at(6)
        s:SET_ACTIVE_SPRITE(6)
        ::J76::
        s:NOP()
        s:at(30006)
        s:SET_CLOCK(6, 76)
        goto J76
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(6)
        s:at(3)
        s:SET_ACTIVE_SPRITE(5)
        s:at(6)
        s:SET_ACTIVE_SPRITE(4)
        s:at(9)
        s:SET_ACTIVE_SPRITE(0)
        ::J80::
        s:INTERP_POS(128, 4, 0, 1090519040, 0)
        s:at(137)
        s:INTERP_POS(128, 4, 0, -1056964608, 0)
        s:at(265)
        s:SET_CLOCK(9, 80)
        goto J80
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(6)
        s:at(3)
        s:SET_ACTIVE_SPRITE(5)
        s:at(6)
        s:SET_ACTIVE_SPRITE(4)
        s:at(9)
        s:SET_ACTIVE_SPRITE(0)
        ::J88::
        s:INTERP_POS(128, 4, 0, 1090519040, 0)
        s:at(137)
        s:INTERP_POS(128, 4, 0, -1056964608, 0)
        s:at(265)
        s:SET_CLOCK(9, 88)
        goto J88
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(0)
        s:at(3)
        s:SET_ACTIVE_SPRITE(1)
        s:at(6)
        s:SET_ACTIVE_SPRITE(2)
        ::J68::
        s:NOP()
        s:at(30006)
        s:SET_CLOCK(6, 68)
        goto J68
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_ACTIVE_SPRITE(7)
        s:SET_SCALE(0, 0)
        s:INTERP_POS(128, 4, 0, 1090519040, 0)
        s:INTERP_SCALE_2(30, 4, 1065353216, 1045220557)
        s:at(30)
        s:INTERP_SCALE_2(30, 4, 1065353216, 1065353216)
        s:at(128)
        s:NOP()
        ::J124::
        s:INTERP_POS(128, 4, 0, -1056964608, 0)
        s:at(256)
        s:INTERP_POS(128, 4, 0, 1090519040, 0)
        s:at(384)
        s:SET_CLOCK(128, 124)
        goto J124
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_USE_OFFSET(1)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_ALPHA(0)
        ::J36::
        s:INTERP_ALPHA(120, 0, 128)
        s:at(128)
        s:INTERP_POS(128, 4, 0, 1090519040, 0)
        s:at(256)
        s:INTERP_POS(128, 4, 0, -1056964608, 0)
        s:INTERP_ALPHA(120, 0, 64)
        s:at(384)
        s:INTERP_POS(128, 4, 0, 1090519040, 0)
        s:at(512)
        s:INTERP_POS(128, 4, 0, -1056964608, 0)
        s:at(640)
        s:SET_CLOCK(0, 36)
        goto J36
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(1129316352, 1130364928, 0)
        s:SET_ALPHA(0)
        s:FADE(255, 60)
        s:INTERP_POS(180, 0, 1127219200, 1130364928, 0)
        s:at(350)
        s:FADE(0, 60)
        s:at(410)
        s:EXIT_HIDE2()
    end,
  },
}
