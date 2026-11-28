-- stg3enm: how the animation is put together and what it does.
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
      { id = 25, rect = { 0, 256, 48, 64 } },
      { id = 26, rect = { 48, 256, 48, 64 } },
      { id = 27, rect = { 96, 256, 48, 64 } },
      { id = 28, rect = { 144, 256, 48, 64 } },
      { id = 29, rect = { 0, 320, 48, 64 } },
      { id = 30, rect = { 48, 320, 48, 64 } },
      { id = 31, rect = { 96, 320, 48, 64 } },
      { id = 32, rect = { 144, 320, 48, 64 } },
      { id = 33, rect = { 192, 320, 48, 64 } },
      { id = 34, rect = { 0, 384, 32, 32 } },
      { id = 35, rect = { 32, 384, 32, 32 } },
      { id = 36, rect = { 64, 384, 32, 32 } },
      { id = 37, rect = { 96, 384, 32, 32 } },
      { id = 38, rect = { 128, 384, 32, 32 } },
      { id = 39, rect = { 160, 384, 32, 32 } },
      { id = 40, rect = { 192, 384, 32, 32 } },
      { id = 41, rect = { 224, 384, 32, 32 } },
      { id = 42, rect = { 0, 416, 32, 32 } },
      { id = 43, rect = { 32, 416, 32, 32 } },
      { id = 44, rect = { 64, 416, 32, 32 } },
      { id = 45, rect = { 96, 416, 32, 32 } },
      { id = 46, rect = { 128, 416, 32, 32 } },
      { id = 47, rect = { 160, 416, 32, 32 } },
      { id = 48, rect = { 192, 416, 32, 32 } },
      { id = 49, rect = { 224, 416, 32, 32 } },
      { id = 50, rect = { 256, 32, 48, 16 } },
      { id = 51, rect = { 0, 96, 64, 64 } },
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
      { index = 13, offset = 1268 },
      { index = 14, offset = 1476 },
      { index = 15, offset = 1696 },
      { index = 16, offset = 1916 },
      { index = 17, offset = 2136 },
      { index = 18, offset = 2356 },
      { index = 19, offset = 2576 },
      { index = 20, offset = 2796 },
      { index = 21, offset = 3004 },
      { index = 22, offset = 3212 },
      { index = 23, offset = 3420 },
      { index = 24, offset = 3628 },
      { index = 128, offset = 3688 },
      { index = 129, offset = 3832 },
      { index = 130, offset = 3960 },
      { index = 131, offset = 4080 },
      { index = 132, offset = 4304 },
      { index = 133, offset = 4520 },
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
      [29] = {  },
      [30] = {  },
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
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(50)
        s:SET_ZWRITE_DISABLE(1)
        s:STOP()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(40)
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
        s:SUB_FLOAT_2({r = 1176260608}, 1063239019, 1048971922)
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
        s:SUB_FLOAT_2({r = 1176260608}, 1061678114, 1048971922)
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
        s:ADD_FLOAT_2({r = 1176260608}, 1051123445, 1036586971)
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
        s:ADD_FLOAT_2({r = 1176260608}, 1056532295, 1036586971)
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
        s:SET_ACTIVE_SPRITE(34)
        s:ADD_FLOAT_2({r = 1176260608}, 1064026455, 1036586971)
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

    -- stream 19
    [19] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(42)
        s:ADD_FLOAT_2({r = 1176260608}, 1032754044, 1036586971)
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
        s:SET_ACTIVE_SPRITE(42)
        s:at(30004)
        s:SET_ACTIVE_SPRITE(43)
        s:at(30008)
        s:SET_ACTIVE_SPRITE(44)
        s:at(30012)
        s:SET_ACTIVE_SPRITE(45)
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

    -- stream 23
    [23] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(42)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 0, 255)
        s:SET_SCALE(0, 1084227584)
        s:INTERP_SCALE_2(30, 0, 1065353216, 1065353216)
        s:at(60)
        ::L1::
        s:LABEL(1, 60)
        ::J96::
        s:SET_ACTIVE_SPRITE(42)
        s:at(64)
        s:SET_ACTIVE_SPRITE(43)
        s:at(68)
        s:SET_ACTIVE_SPRITE(44)
        s:at(72)
        s:SET_ACTIVE_SPRITE(45)
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
        s:SET_ACTIVE_SPRITE(51)
        s:SET_BLEND(1)
        s:SET_ANGLE_VEL(0, 0, 1029076816)
        s:at(10000)
        s:EXIT_HIDE2()
    end,

    -- stream 25
    [25] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_USE_OFFSET(1)
        s:SET_ACTIVE_SPRITE(25)
        ::J64::
        s:INTERP_POS(60, 4, 0, 1090519040, 0)
        s:at(60)
        s:INTERP_POS(60, 4, 0, -1056964608, 0)
        s:at(120)
        s:SET_CLOCK(0, 64)
        goto J64
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 26
    [26] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(29)
        s:at(3)
        s:SET_ACTIVE_SPRITE(30)
        s:at(6)
        s:SET_ACTIVE_SPRITE(31)
        s:at(9)
        s:SET_ACTIVE_SPRITE(32)
        ::J96::
        s:NOP()
        s:at(30009)
        s:SET_CLOCK(9, 96)
        goto J96
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 27
    [27] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ACTIVE_SPRITE(29)
        s:at(3)
        s:SET_ACTIVE_SPRITE(30)
        s:at(6)
        s:SET_ACTIVE_SPRITE(31)
        s:at(9)
        s:SET_ACTIVE_SPRITE(32)
        ::J88::
        s:NOP()
        s:at(30009)
        s:SET_CLOCK(9, 88)
        goto J88
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 28
    [28] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(33)
        s:SET_ANGLE_VEL(0, 0, -1094119461)
        s:at(30)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(25)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        ::J144::
        s:INTERP_POS(60, 4, 0, 1090519040, 0)
        s:at(90)
        s:INTERP_POS(60, 4, 0, -1056964608, 0)
        s:at(150)
        s:SET_CLOCK(30, 144)
        goto J144
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 29
    [29] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(33)
        s:SET_ANGLE_VEL(0, 0, 1053364187)
        s:at(30)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ACTIVE_SPRITE(25)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        ::J136::
        s:INTERP_POS(60, 4, 0, 1090519040, 0)
        s:at(90)
        s:INTERP_POS(60, 4, 0, -1056964608, 0)
        s:at(150)
        s:SET_CLOCK(30, 136)
        goto J136
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 30
    [30] = function(s)
        s:at(0)
        s:SET_ROTATION(0, 0, 0)
        s:SET_ANGLE_VEL(0, 0, 0)
        s:SET_ACTIVE_SPRITE(25)
        s:at(3)
        s:SET_ACTIVE_SPRITE(26)
        s:at(6)
        s:SET_ACTIVE_SPRITE(27)
        s:at(9)
        s:SET_ACTIVE_SPRITE(28)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        ::J120::
        s:INTERP_POS(60, 4, 0, 1090519040, 0)
        s:at(69)
        s:INTERP_POS(60, 4, 0, -1056964608, 0)
        s:at(129)
        s:SET_CLOCK(9, 120)
        goto J120
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
