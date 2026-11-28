-- player00: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 1, 1, 31, 47 } },
      { id = 1, rect = { 33, 1, 31, 47 } },
      { id = 2, rect = { 65, 1, 31, 47 } },
      { id = 3, rect = { 97, 1, 31, 47 } },
      { id = 4, rect = { 1, 49, 31, 47 } },
      { id = 5, rect = { 33, 49, 31, 47 } },
      { id = 6, rect = { 65, 49, 31, 47 } },
      { id = 7, rect = { 97, 49, 31, 47 } },
      { id = 8, rect = { 129, 49, 31, 47 } },
      { id = 9, rect = { 161, 49, 31, 47 } },
      { id = 10, rect = { 193, 49, 31, 47 } },
      { id = 11, rect = { 1, 49, 31, 47 } },
      { id = 12, rect = { 33, 49, 31, 47 } },
      { id = 13, rect = { 65, 49, 31, 47 } },
      { id = 14, rect = { 97, 49, 31, 47 } },
      { id = 15, rect = { 129, 49, 31, 47 } },
      { id = 16, rect = { 161, 49, 31, 47 } },
      { id = 17, rect = { 193, 49, 31, 47 } },
      { id = 64, rect = { 129, 1, 14, 14 } },
      { id = 65, rect = { 144, 1, 14, 14 } },
      { id = 66, rect = { 208, 0, 32, 32 } },
      { id = 67, rect = { 160, 0, 16, 16 } },
      { id = 68, rect = { 176, 0, 16, 16 } },
      { id = 69, rect = { 193, 1, 14, 46 } },
      { id = 70, rect = { 1, 160, 30, 62 } },
      { id = 128, rect = { 128, 16, 16, 16 } },
      { id = 129, rect = { 144, 16, 16, 16 } },
      { id = 130, rect = { 128, 32, 16, 16 } },
      { id = 131, rect = { 1, 97, 62, 62 } },
      { id = 132, rect = { 65, 97, 62, 62 } },
      { id = 133, rect = { 128, 128, 128, 128 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 72 },
      { index = 2, offset = 180 },
      { index = 3, offset = 300 },
      { index = 4, offset = 416 },
      { index = 64, offset = 544 },
      { index = 65, offset = 632 },
      { index = 66, offset = 720 },
      { index = 67, offset = 772 },
      { index = 68, offset = 824 },
      { index = 69, offset = 916 },
      { index = 70, offset = 1008 },
      { index = 96, offset = 1096 },
      { index = 97, offset = 1208 },
      { index = 98, offset = 1320 },
      { index = 99, offset = 1432 },
      { index = 100, offset = 1544 },
      { index = 101, offset = 1676 },
      { index = 102, offset = 1808 },
      { index = 128, offset = 1944 },
      { index = 129, offset = 2000 },
      { index = 130, offset = 2056 },
      { index = 131, offset = 2112 },
      { index = 132, offset = 2168 },
      { index = 133, offset = 2196 },
      { index = 134, offset = 2588 },
      { index = 135, offset = 2980 },
      { index = 136, offset = 3372 },
      { index = 137, offset = 3764 },
      { index = 138, offset = 3944 },
      { index = 139, offset = 4144 },
      { index = 140, offset = 4344 },
      { index = 141, offset = 4544 },
      { index = 142, offset = 4688 },
      { index = 143, offset = 4864 },
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
      [11] = {  },
      [12] = {  },
      [13] = {  },
      [14] = {  },
      [15] = {  },
      [16] = {  },
      [17] = {  },
      [18] = {  },
      [19] = {  },
      [20] = {  },
      [21] = {  },
      [22] = {  },
      [23] = {  },
      [24] = { 1 },
      [25] = { 1 },
      [26] = { 1 },
      [27] = { 1 },
      [28] = {  },
      [29] = {  },
      [30] = {  },
      [31] = {  },
      [32] = {  },
      [33] = {  },
      [34] = {  },
    },

    -- stream 0
    [0] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:at(8)
        s:SET_ACTIVE_SPRITE(1)
        s:at(16)
        s:SET_ACTIVE_SPRITE(2)
        s:at(24)
        s:SET_ACTIVE_SPRITE(3)
        s:at(32)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:at(3)
        s:SET_ACTIVE_SPRITE(5)
        s:at(6)
        s:SET_ACTIVE_SPRITE(6)
        ::J36::
        s:at(14)
        s:SET_ACTIVE_SPRITE(7)
        s:at(22)
        s:SET_ACTIVE_SPRITE(8)
        s:at(30)
        s:SET_ACTIVE_SPRITE(9)
        s:at(38)
        s:SET_ACTIVE_SPRITE(10)
        s:at(46)
        s:SET_CLOCK(6, 36)
        goto J36
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:at(3)
        s:SET_ACTIVE_SPRITE(6)
        s:at(6)
        s:SET_ACTIVE_SPRITE(5)
        s:at(9)
        s:SET_ACTIVE_SPRITE(4)
        ::J48::
        s:at(17)
        s:SET_ACTIVE_SPRITE(0)
        s:at(25)
        s:SET_ACTIVE_SPRITE(1)
        s:at(33)
        s:SET_ACTIVE_SPRITE(2)
        s:at(41)
        s:SET_ACTIVE_SPRITE(3)
        s:at(49)
        s:SET_CLOCK(9, 48)
        goto J48
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(4)
        s:at(3)
        s:SET_ACTIVE_SPRITE(5)
        s:at(6)
        s:SET_ACTIVE_SPRITE(6)
        ::J44::
        s:at(14)
        s:SET_ACTIVE_SPRITE(7)
        s:at(22)
        s:SET_ACTIVE_SPRITE(8)
        s:at(30)
        s:SET_ACTIVE_SPRITE(9)
        s:at(38)
        s:SET_ACTIVE_SPRITE(10)
        s:at(46)
        s:SET_CLOCK(6, 44)
        goto J44
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(7)
        s:at(3)
        s:SET_ACTIVE_SPRITE(6)
        s:at(6)
        s:SET_ACTIVE_SPRITE(5)
        s:at(9)
        s:SET_ACTIVE_SPRITE(4)
        ::J56::
        s:at(17)
        s:SET_ACTIVE_SPRITE(0)
        s:at(25)
        s:SET_ACTIVE_SPRITE(1)
        s:at(33)
        s:SET_ACTIVE_SPRITE(2)
        s:at(41)
        s:SET_ACTIVE_SPRITE(3)
        s:at(49)
        s:SET_CLOCK(9, 56)
        goto J56
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, 1036831949)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(64)
        s:SET_ZWRITE_DISABLE(1)
        s:at(10000)
        s:EXIT()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, -1105618534)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(65)
        s:SET_ZWRITE_DISABLE(1)
        s:at(10000)
        s:EXIT()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(69)
        s:SET_ZWRITE_DISABLE(1)
        s:at(10000)
        s:EXIT()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_ZWRITE_DISABLE(1)
        s:at(10000)
        s:EXIT()
    end,

    -- stream 9
    [9] = function(s)
        s:at(0)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_SCALE(1067869798, 1065353216)
        s:INTERP_SCALE_2(8, 0, 1067869798, 1067869798)
        s:SET_ZWRITE_DISABLE(1)
        s:at(10000)
        s:EXIT()
    end,

    -- stream 10
    [10] = function(s)
        s:at(0)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_SCALE(1072064102, 1065353216)
        s:INTERP_SCALE_2(8, 0, 1072064102, 1072064102)
        s:SET_ZWRITE_DISABLE(1)
        s:at(10000)
        s:EXIT()
    end,

    -- stream 11
    [11] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, -1105618534)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ALPHA(96)
        s:SET_ACTIVE_SPRITE(66)
        s:SET_ZWRITE_DISABLE(1)
        s:at(10000)
        s:EXIT()
    end,

    -- stream 12
    [12] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ACTIVE_SPRITE(67)
        s:SET_ALPHA(96)
        s:FADE(0, 30)
        s:SET_SCALE_SPEED(1019159948, 1019159948)
        s:SET_ZWRITE_DISABLE(1)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 13
    [13] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ACTIVE_SPRITE(68)
        s:SET_ALPHA(72)
        s:FADE(0, 30)
        s:SET_SCALE_SPEED(1019159948, 1019159948)
        s:SET_ZWRITE_DISABLE(1)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 14
    [14] = function(s)
        s:at(0)
        s:SET_ALPHA(64)
        s:FADE(0, 20)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(69)
        s:SET_AUTO_ROTATE(1)
        s:INTERP_SCALE(1036831949, 1086324736, 20)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20)
        s:EXIT()
    end,

    -- stream 15
    [15] = function(s)
        s:at(0)
        s:SET_ALPHA(64)
        s:FADE(0, 20)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_AUTO_ROTATE(1)
        s:INTERP_SCALE(1036831949, 1086324736, 20)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20)
        s:EXIT()
    end,

    -- stream 16
    [16] = function(s)
        s:at(0)
        s:SET_ALPHA(64)
        s:FADE(0, 20)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_AUTO_ROTATE(1)
        s:SET_SCALE(1067869798, 1067869798)
        s:INTERP_SCALE_2(20, 0, 1036831949, 1088421888)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20)
        s:EXIT()
    end,

    -- stream 17
    [17] = function(s)
        s:at(0)
        s:SET_ALPHA(64)
        s:FADE(0, 20)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_AUTO_ROTATE(1)
        s:SET_SCALE(1072064102, 1072064102)
        s:INTERP_SCALE_2(20, 0, 1036831949, 1091567616)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20)
        s:EXIT()
    end,

    -- stream 18
    [18] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, -1105618534)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ALPHA(96)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(66)
        s:INTERP_SCALE(1056964608, 1056964608, 30)
        s:SET_ZWRITE_DISABLE(1)
        s:FADE(0, 30)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 19
    [19] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, 1036831949)
        ::J20::
        s:SET_ACTIVE_SPRITE(128)
        s:at(10000)
        s:SET_CLOCK(0, 20)
        goto J20
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 20
    [20] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, -1110651699)
        ::J20::
        s:SET_ACTIVE_SPRITE(128)
        s:at(10000)
        s:SET_CLOCK(0, 20)
        goto J20
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 21
    [21] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, 1036831949)
        ::J20::
        s:SET_ACTIVE_SPRITE(129)
        s:at(10000)
        s:SET_CLOCK(0, 20)
        goto J20
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 22
    [22] = function(s)
        s:at(0)
        s:SET_ANGLE_VEL(0, 0, -1110651699)
        ::J20::
        s:SET_ACTIVE_SPRITE(129)
        s:at(10000)
        s:SET_CLOCK(0, 20)
        goto J20
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 23
    [23] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(130)
        s:EXIT()
    end,

    -- stream 24
    [24] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(131)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_COLOR(3158271)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ALPHA(128)
        s:SET_SCALE(0, 0)
        s:SET_SCALE_SPEED(1028443341, 1028443341)
        s:at(30)
        s:POS_TIME_ACCEL(16.0, 16.0, 0.0, 10)
        s:at(60)
        s:SET_SCALE_SPEED(0, 0)
        ::J164::
        s:at(70)
        s:POS_TIME_ACCEL(-16.0, -8.0, 0.0, 10)
        s:at(80)
        s:POS_TIME_ACCEL(0.0, -16.0, 0.0, 10)
        s:at(90)
        s:POS_TIME_ACCEL(8.0, 0.0, 0.0, 10)
        s:at(100)
        s:POS_TIME_ACCEL(-16.0, 16.0, 0.0, 10)
        s:at(110)
        s:POS_TIME_ACCEL(-8.0, 8.0, 0.0, 10)
        s:SET_CLOCK(60, 164)
        goto J164
        ::L1::
        s:LABEL(1, 110)
        s:FADE(0, 30)
        s:at(120)
        s:POS_TIME_ACCEL(-16.0, -8.0, 0.0, 10)
        s:at(130)
        s:POS_TIME_ACCEL(0.0, -16.0, 0.0, 10)
        s:EXIT_HIDE2()
    end,

    -- stream 25
    [25] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(131)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_COLOR(3211263)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ALPHA(128)
        s:SET_SCALE(0, 0)
        s:at(8)
        s:SET_SCALE_SPEED(1028443341, 1028443341)
        s:at(38)
        s:POS_TIME_ACCEL(-16.0, 16.0, 0.0, 10)
        s:at(68)
        s:SET_SCALE_SPEED(0, 0)
        ::J164::
        s:at(78)
        s:POS_TIME_ACCEL(-8.0, 8.0, 0.0, 10)
        s:at(88)
        s:POS_TIME_ACCEL(16.0, -16.0, 0.0, 10)
        s:at(98)
        s:POS_TIME_ACCEL(-8.0, 8.0, 0.0, 10)
        s:at(108)
        s:POS_TIME_ACCEL(-16.0, -16.0, 0.0, 10)
        s:at(118)
        s:POS_TIME_ACCEL(8.0, -8.0, 0.0, 10)
        s:SET_CLOCK(68, 164)
        goto J164
        ::L1::
        s:LABEL(1, 118)
        s:FADE(0, 30)
        s:at(128)
        s:POS_TIME_ACCEL(-8.0, 8.0, 0.0, 10)
        s:at(138)
        s:POS_TIME_ACCEL(16.0, -16.0, 0.0, 10)
        s:EXIT_HIDE2()
    end,

    -- stream 26
    [26] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(131)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_COLOR(3211056)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ALPHA(128)
        s:SET_SCALE(0, 0)
        s:at(8)
        s:SET_SCALE_SPEED(1028443341, 1028443341)
        s:at(38)
        s:POS_TIME_ACCEL(0.0, -8.0, 0.0, 10)
        s:at(68)
        s:SET_SCALE_SPEED(0, 0)
        ::J164::
        s:at(78)
        s:POS_TIME_ACCEL(-8.0, 16.0, 0.0, 10)
        s:at(88)
        s:POS_TIME_ACCEL(8.0, 0.0, 10.0, 0)
        s:at(98)
        s:POS_TIME_ACCEL(16.0, 16.0, 0.0, 10)
        s:at(108)
        s:POS_TIME_ACCEL(16.0, 16.0, 0.0, 10)
        s:at(118)
        s:POS_TIME_ACCEL(0.0, -16.0, 0.0, 10)
        s:SET_CLOCK(68, 164)
        goto J164
        ::L1::
        s:LABEL(1, 118)
        s:FADE(0, 30)
        s:at(128)
        s:POS_TIME_ACCEL(-8.0, 16.0, 0.0, 10)
        s:at(138)
        s:POS_TIME_ACCEL(8.0, 0.0, 10.0, 0)
        s:EXIT_HIDE2()
    end,

    -- stream 27
    [27] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(131)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_COLOR(16724016)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:SET_ALPHA(128)
        s:at(24)
        s:SET_SCALE(0, 0)
        s:SET_SCALE_SPEED(1028443341, 1028443341)
        s:at(54)
        s:POS_TIME_ACCEL(0.0, 0.0, 0.0, 30)
        s:at(84)
        s:SET_SCALE_SPEED(0, 0)
        ::J164::
        s:at(94)
        s:POS_TIME_ACCEL(8.0, -8.0, 0.0, 10)
        s:at(104)
        s:POS_TIME_ACCEL(-16.0, -8.0, 0.0, 10)
        s:at(114)
        s:POS_TIME_ACCEL(-8.0, 16.0, 0.0, 10)
        s:at(124)
        s:POS_TIME_ACCEL(8.0, -16.0, 0.0, 10)
        s:at(134)
        s:POS_TIME_ACCEL(-8.0, 8.0, 0.0, 10)
        s:SET_CLOCK(84, 164)
        goto J164
        ::L1::
        s:LABEL(1, 134)
        s:FADE(0, 30)
        s:at(144)
        s:POS_TIME_ACCEL(8.0, -8.0, 0.0, 10)
        s:at(154)
        s:POS_TIME_ACCEL(-16.0, -8.0, 0.0, 10)
        s:EXIT_HIDE2()
    end,

    -- stream 28
    [28] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(132)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_COLOR(16748688)
        s:SET_SCALE(1077936128, 1088895438)
        s:SET_ALPHA(0)
        s:FADE(255, 60)
        s:at(60)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:POS_TIME_ACCEL(-192.0, 0.0, 0.0, 120)
        s:at(120)
        s:FADE(0, 60)
        s:at(180)
        s:EXIT_HIDE2()
    end,

    -- stream 29
    [29] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(132)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ROTATION(0, 0, 1070141403)
        s:SET_BLEND(1)
        s:SET_COLOR(9474303)
        s:SET_SCALE(1077936128, 1088895438)
        s:SET_ALPHA(0)
        s:FADE(255, 60)
        s:at(60)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:POS_TIME_ACCEL(0.0, -320.0, 0.0, 120)
        s:at(120)
        s:FADE(0, 60)
        s:at(180)
        s:EXIT_HIDE2()
    end,

    -- stream 30
    [30] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(132)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ROTATION(0, 0, 1078530011)
        s:SET_BLEND(1)
        s:SET_COLOR(16748688)
        s:SET_SCALE(1077936128, 1088895438)
        s:SET_ALPHA(0)
        s:FADE(255, 60)
        s:at(60)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:POS_TIME_ACCEL(192.0, 0.0, 0.0, 120)
        s:at(120)
        s:FADE(0, 60)
        s:at(180)
        s:EXIT_HIDE2()
    end,

    -- stream 31
    [31] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(132)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ROTATION(0, 0, -1077342245)
        s:SET_BLEND(1)
        s:SET_COLOR(9474303)
        s:SET_SCALE(1077936128, 1088895438)
        s:SET_ALPHA(0)
        s:FADE(255, 60)
        s:at(60)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, 0)
        s:POS_TIME_ACCEL(0.0, 320.0, 0.0, 120)
        s:at(120)
        s:FADE(0, 60)
        s:at(180)
        s:EXIT_HIDE2()
    end,

    -- stream 32
    [32] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(133)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1077936128, 1077936128)
        s:INTERP_SCALE_2(60, 4, 1073741824, 1073741824)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 4, 255)
        s:at(60)
        s:SET_USE_OFFSET(1)
        s:at(130)
        s:INTERP_ALPHA(20, 4, 0)
        s:at(150)
        s:EXIT_HIDE2()
    end,

    -- stream 33
    [33] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(133)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ANGLE_VEL(0, 0, -1121920644)
        s:SET_BLEND(1)
        s:SET_SCALE(1082130432, 1082130432)
        s:INTERP_SCALE_2(60, 0, 1073741824, 1073741824)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 4, 128)
        s:at(60)
        s:SET_USE_OFFSET(1)
        s:at(110)
        s:INTERP_ALPHA(30, 4, 0)
        s:at(140)
        s:EXIT_HIDE2()
    end,

    -- stream 34
    [34] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(133)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ANGLE_VEL(0, 0, 1040583314)
        s:SET_BLEND(1)
        s:SET_SCALE(0, 0)
        s:INTERP_SCALE_2(60, 4, 1075838976, 1075838976)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 4, 128)
        s:at(60)
        s:SET_USE_OFFSET(1)
        s:at(110)
        s:INTERP_ALPHA(30, 4, 0)
        s:at(140)
        s:EXIT_HIDE2()
    end,
  },
}
