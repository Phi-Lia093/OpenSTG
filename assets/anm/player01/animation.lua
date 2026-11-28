-- player01: how the animation is put together and what it does.
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
      { id = 18, rect = { 33, 97, 62, 62 } },
      { id = 19, rect = { 0, 161, 30, 94 } },
      { id = 64, rect = { 128, 0, 16, 32 } },
      { id = 65, rect = { 160, 0, 16, 16 } },
      { id = 66, rect = { 160, 96, 16, 64 } },
      { id = 67, rect = { 144, 0, 16, 32 } },
      { id = 68, rect = { 0, 96, 32, 32 } },
      { id = 69, rect = { 96, 96, 64, 64 } },
      { id = 70, rect = { 177, 1, 14, 14 } },
      { id = 128, rect = { 128, 32, 16, 16 } },
      { id = 129, rect = { 144, 32, 16, 16 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 72 },
      { index = 2, offset = 180 },
      { index = 3, offset = 300 },
      { index = 4, offset = 416 },
      { index = 5, offset = 544 },
      { index = 6, offset = 672 },
      { index = 7, offset = 800 },
      { index = 8, offset = 928 },
      { index = 9, offset = 1052 },
      { index = 10, offset = 1176 },
      { index = 11, offset = 1300 },
      { index = 12, offset = 1424 },
      { index = 13, offset = 1548 },
      { index = 14, offset = 1672 },
      { index = 64, offset = 1796 },
      { index = 65, offset = 1860 },
      { index = 66, offset = 1928 },
      { index = 67, offset = 1984 },
      { index = 68, offset = 2052 },
      { index = 69, offset = 2120 },
      { index = 70, offset = 2188 },
      { index = 71, offset = 2244 },
      { index = 72, offset = 2312 },
      { index = 73, offset = 2380 },
      { index = 74, offset = 2512 },
      { index = 75, offset = 2644 },
      { index = 76, offset = 2776 },
      { index = 77, offset = 2904 },
      { index = 78, offset = 3032 },
      { index = 96, offset = 3160 },
      { index = 97, offset = 3256 },
      { index = 98, offset = 3368 },
      { index = 99, offset = 3480 },
      { index = 100, offset = 3592 },
      { index = 101, offset = 3704 },
      { index = 102, offset = 3816 },
      { index = 103, offset = 3928 },
      { index = 104, offset = 4040 },
      { index = 128, offset = 4152 },
      { index = 129, offset = 4208 },
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
      [28] = { 1 },
      [29] = { 1 },
      [30] = {  },
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
        s:SET_ACTIVE_SPRITE(18)
        s:SET_BLEND(1)
        s:SET_COLOR(8421631)
        s:SET_ALPHA(0)
        s:FADE(128, 30)
        s:SET_ANGLE_VEL(0, 0, 1036586971)
        s:SET_ZWRITE_DISABLE(1)
        s:at(260)
        s:FADE(0, 30)
        s:at(290)
        s:EXIT_HIDE2()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(18)
        s:SET_BLEND(1)
        s:SET_COLOR(8454016)
        s:SET_ALPHA(0)
        s:FADE(128, 30)
        s:SET_ANGLE_VEL(0, 0, -1110896677)
        s:SET_ZWRITE_DISABLE(1)
        s:at(260)
        s:FADE(0, 30)
        s:at(290)
        s:EXIT_HIDE2()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(18)
        s:SET_BLEND(1)
        s:SET_COLOR(16744576)
        s:SET_ALPHA(0)
        s:FADE(128, 30)
        s:SET_ANGLE_VEL(0, 0, 1036586971)
        s:SET_ZWRITE_DISABLE(1)
        s:at(260)
        s:FADE(0, 30)
        s:at(290)
        s:EXIT_HIDE2()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(19)
        s:SET_BLEND(1)
        s:SET_COLOR(16744576)
        s:SET_ALPHA(0)
        s:FADE(64, 50)
        s:SET_SCALE(1093664768, 1088421888)
        s:SET_ZWRITE_DISABLE(1)
        s:at(260)
        s:FADE(0, 70)
        s:at(330)
        s:EXIT_HIDE2()
    end,

    -- stream 9
    [9] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(19)
        s:SET_BLEND(1)
        s:SET_COLOR(8454016)
        s:SET_ALPHA(0)
        s:FADE(96, 20)
        s:SET_SCALE(1093664768, 1088421888)
        s:SET_ZWRITE_DISABLE(1)
        s:at(260)
        s:FADE(0, 70)
        s:at(330)
        s:EXIT_HIDE2()
    end,

    -- stream 10
    [10] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(19)
        s:SET_BLEND(1)
        s:SET_COLOR(8421631)
        s:SET_ALPHA(0)
        s:FADE(96, 20)
        s:SET_SCALE(1093664768, 1088421888)
        s:SET_ZWRITE_DISABLE(1)
        s:at(260)
        s:FADE(0, 70)
        s:at(330)
        s:EXIT_HIDE2()
    end,

    -- stream 11
    [11] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(19)
        s:SET_BLEND(1)
        s:SET_COLOR(8454143)
        s:SET_ALPHA(0)
        s:FADE(64, 50)
        s:SET_SCALE(1093664768, 1088421888)
        s:SET_ZWRITE_DISABLE(1)
        s:at(260)
        s:FADE(0, 70)
        s:at(330)
        s:EXIT_HIDE2()
    end,

    -- stream 12
    [12] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(19)
        s:SET_BLEND(1)
        s:SET_COLOR(8421631)
        s:SET_ALPHA(0)
        s:FADE(200, 50)
        s:SET_SCALE(1077936128, 1088421888)
        s:SET_ZWRITE_DISABLE(1)
        s:at(230)
        s:FADE(0, 70)
        s:at(300)
        s:EXIT_HIDE2()
    end,

    -- stream 13
    [13] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(19)
        s:SET_BLEND(1)
        s:SET_COLOR(8446207)
        s:SET_ALPHA(0)
        s:FADE(200, 50)
        s:SET_SCALE(1077936128, 1088421888)
        s:SET_ZWRITE_DISABLE(1)
        s:at(230)
        s:FADE(0, 70)
        s:at(300)
        s:EXIT_HIDE2()
    end,

    -- stream 14
    [14] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(19)
        s:SET_BLEND(1)
        s:SET_COLOR(8454112)
        s:SET_ALPHA(0)
        s:FADE(200, 50)
        s:SET_SCALE(1077936128, 1088421888)
        s:SET_ZWRITE_DISABLE(1)
        s:at(230)
        s:FADE(0, 70)
        s:at(300)
        s:EXIT_HIDE2()
    end,

    -- stream 15
    [15] = function(s)
        s:at(0)
        s:SET_ALPHA(192)
        s:SET_ACTIVE_SPRITE(64)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_AUTO_ROTATE(1)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 16
    [16] = function(s)
        s:at(0)
        s:SET_SCALE(1067869798, 1067869798)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(65)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 17
    [17] = function(s)
        s:at(0)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(65)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 18
    [18] = function(s)
        s:at(0)
        s:SET_SCALE(1073741824, 1073741824)
        s:SET_ALPHA(160)
        s:SET_ACTIVE_SPRITE(65)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 19
    [19] = function(s)
        s:at(0)
        s:SET_SCALE(1075000115, 1075000115)
        s:SET_ALPHA(192)
        s:SET_ACTIVE_SPRITE(65)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 20
    [20] = function(s)
        s:at(0)
        s:SET_SCALE(1067869798, 1067869798)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(66)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 21
    [21] = function(s)
        s:at(0)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ALPHA(128)
        s:SET_ACTIVE_SPRITE(66)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 22
    [22] = function(s)
        s:at(0)
        s:SET_SCALE(1073741824, 1073741824)
        s:SET_ALPHA(160)
        s:SET_ACTIVE_SPRITE(66)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 23
    [23] = function(s)
        s:at(0)
        s:SET_SCALE(1075000115, 1075000115)
        s:SET_ALPHA(192)
        s:SET_ACTIVE_SPRITE(66)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 24
    [24] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_ALPHA(0)
        s:SET_SCALE(1058642330, 1065353216)
        s:FADE(128, 20)
        s:SET_COLOR(16744576)
        s:SET_ZWRITE_DISABLE(1)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 30)
        s:FADE(0, 30)
        s:at(60)
        s:EXIT_HIDE2()
    end,

    -- stream 25
    [25] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_ALPHA(0)
        s:FADE(160, 20)
        s:SET_COLOR(16744576)
        s:SET_SCALE(1061997773, 1065353216)
        s:SET_ZWRITE_DISABLE(1)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 30)
        s:FADE(0, 30)
        s:at(60)
        s:EXIT_HIDE2()
    end,

    -- stream 26
    [26] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_ALPHA(0)
        s:FADE(160, 20)
        s:SET_COLOR(16744576)
        s:SET_SCALE(1065353216, 1065353216)
        s:SET_ZWRITE_DISABLE(1)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 30)
        s:FADE(0, 30)
        s:at(60)
        s:EXIT_HIDE2()
    end,

    -- stream 27
    [27] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_ALPHA(0)
        s:FADE(60, 20)
        s:SET_COLOR(16744576)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1058642330, 1065353216)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 30)
        s:EXIT_HIDE2()
    end,

    -- stream 28
    [28] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_ALPHA(0)
        s:FADE(70, 20)
        s:SET_COLOR(16744576)
        s:SET_SCALE(1061997773, 1065353216)
        s:SET_ZWRITE_DISABLE(1)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 30)
        s:EXIT_HIDE2()
    end,

    -- stream 29
    [29] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(70)
        s:SET_ALPHA(0)
        s:FADE(70, 20)
        s:SET_COLOR(16744576)
        s:SET_SCALE(1065353216, 1065353216)
        s:SET_ZWRITE_DISABLE(1)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 30)
        s:EXIT_HIDE2()
    end,

    -- stream 30
    [30] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(67)
        s:SET_ALPHA(96)
        s:FADE(0, 20)
        s:SET_SCALE_SPEED(1024416809, 1024416809)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20)
        s:EXIT_HIDE2()
    end,

    -- stream 31
    [31] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ALPHA(96)
        s:SET_ACTIVE_SPRITE(68)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1061997773, 1061997773)
        s:SET_SCALE_SPEED(1033476506, 1033476506)
        s:at(6)
        s:FADE(0, 14)
        s:at(20)
        s:EXIT()
    end,

    -- stream 32
    [32] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ALPHA(160)
        s:SET_ACTIVE_SPRITE(68)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1061997773, 1061997773)
        s:SET_SCALE_SPEED(1036831949, 1036831949)
        s:at(6)
        s:FADE(0, 14)
        s:at(20)
        s:EXIT()
    end,

    -- stream 33
    [33] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ALPHA(160)
        s:SET_ACTIVE_SPRITE(68)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1061997773, 1061997773)
        s:SET_SCALE_SPEED(1040187392, 1040187392)
        s:at(6)
        s:FADE(0, 14)
        s:at(20)
        s:EXIT()
    end,

    -- stream 34
    [34] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ALPHA(160)
        s:SET_ACTIVE_SPRITE(68)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1061997773, 1061997773)
        s:SET_SCALE_SPEED(1043542835, 1043542835)
        s:at(6)
        s:FADE(0, 14)
        s:at(20)
        s:EXIT()
    end,

    -- stream 35
    [35] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ALPHA(160)
        s:SET_ACTIVE_SPRITE(69)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1061997773, 1061997773)
        s:SET_SCALE_SPEED(1028443341, 1028443341)
        s:at(6)
        s:FADE(0, 14)
        s:at(30)
        s:EXIT()
    end,

    -- stream 36
    [36] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ALPHA(200)
        s:SET_ACTIVE_SPRITE(69)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1061997773, 1061997773)
        s:SET_SCALE_SPEED(1032358025, 1032358025)
        s:at(6)
        s:FADE(0, 14)
        s:at(30)
        s:EXIT()
    end,

    -- stream 37
    [37] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ALPHA(220)
        s:SET_ACTIVE_SPRITE(69)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1061997773, 1061997773)
        s:SET_SCALE_SPEED(1036831949, 1036831949)
        s:at(6)
        s:FADE(0, 14)
        s:at(35)
        s:EXIT()
    end,

    -- stream 38
    [38] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ALPHA(240)
        s:SET_ACTIVE_SPRITE(69)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1061997773, 1061997773)
        s:SET_SCALE_SPEED(1035340641, 1035340641)
        s:at(6)
        s:FADE(0, 14)
        s:at(45)
        s:EXIT()
    end,

    -- stream 39
    [39] = function(s)
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

    -- stream 40
    [40] = function(s)
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
  },
}
