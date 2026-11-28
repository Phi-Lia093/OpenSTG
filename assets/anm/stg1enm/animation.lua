-- stg1enm: how the animation is put together and what it does.
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
      { id = 24, rect = { 0, 256, 32, 64 } },
      { id = 25, rect = { 32, 256, 48, 64 } },
      { id = 26, rect = { 80, 256, 48, 64 } },
      { id = 27, rect = { 128, 256, 48, 64 } },
      { id = 28, rect = { 176, 256, 48, 64 } },
      { id = 29, rect = { 224, 256, 48, 64 } },
      { id = 30, rect = { 0, 320, 48, 64 } },
      { id = 31, rect = { 48, 320, 48, 64 } },
      { id = 32, rect = { 96, 320, 48, 64 } },
      { id = 33, rect = { 144, 320, 48, 64 } },
      { id = 34, rect = { 0, 384, 48, 64 } },
      { id = 35, rect = { 48, 384, 48, 64 } },
      { id = 36, rect = { 96, 384, 48, 64 } },
      { id = 37, rect = { 144, 384, 48, 64 } },
      { id = 38, rect = { 0, 448, 48, 64 } },
      { id = 39, rect = { 48, 448, 48, 64 } },
      { id = 40, rect = { 96, 448, 48, 64 } },
      { id = 41, rect = { 144, 448, 48, 64 } },
      { id = 42, rect = { 192, 320, 32, 32 } },
      { id = 43, rect = { 224, 320, 32, 32 } },
      { id = 44, rect = { 256, 320, 32, 32 } },
      { id = 45, rect = { 288, 320, 32, 32 } },
      { id = 46, rect = { 256, 32, 48, 16 } },
      { id = 47, rect = { 0, 96, 64, 64 } },
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
      { index = 11, offset = 1072 },
      { index = 12, offset = 1452 },
      { index = 13, offset = 1572 },
      { index = 128, offset = 1632 },
      { index = 129, offset = 1676 },
      { index = 130, offset = 1720 },
      { index = 131, offset = 1764 },
      { index = 132, offset = 1836 },
      { index = 133, offset = 1908 },
      { index = 134, offset = 1988 },
      { index = 135, offset = 2060 },
      { index = 136, offset = 2188 },
      { index = 137, offset = 2308 },
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
        s:RAND({r = 10000}, 4)
        s:JUMP_IF_NEQ({r = 10000}, 0, 68, 0)
        s:SET_ACTIVE_SPRITE(42)
        s:SET_CLOCK(0, 184)
        goto J184
        s:JUMP_IF_NEQ({r = 10000}, 1, 120, 0)
        s:SET_ACTIVE_SPRITE(43)
        s:SET_CLOCK(0, 184)
        goto J184
        s:JUMP_IF_NEQ({r = 10000}, 2, 156, 0)
        s:SET_ACTIVE_SPRITE(44)
        s:SET_CLOCK(0, 184)
        goto J184
        s:SET_ACTIVE_SPRITE(45)
        ::J184::
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 0, 255)
        s:RAND_FLOAT({r = 1176260608}, 1086918619)
        s:SUB_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1078530011)
        s:SET_ROTATION(0, 0, {r = 1176260608})
        s:RAND_FLOAT({r = 1176260608}, 1077936128)
        s:ADD_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1069547520)
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
        s:SET_SCALE(1067030938, 1067030938)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(46)
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
        s:at(0)
        s:SET_ACTIVE_SPRITE(47)
        s:SET_BLEND(1)
        s:SET_ANGLE_VEL(0, 0, 1029076816)
        s:at(10000)
        s:EXIT_HIDE2()
    end,

    -- stream 14
    [14] = function(s)
        s:at(0)
        s:SET_SCALE(1065353216, 1065353216)
        s:SET_ACTIVE_SPRITE(24)
        s:EXIT()
    end,

    -- stream 15
    [15] = function(s)
        s:at(0)
        s:SET_SCALE(-1082130432, 1065353216)
        s:SET_ACTIVE_SPRITE(25)
        s:EXIT()
    end,

    -- stream 16
    [16] = function(s)
        s:at(0)
        s:SET_SCALE(1065353216, 1065353216)
        s:SET_ACTIVE_SPRITE(25)
        s:EXIT()
    end,

    -- stream 17
    [17] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(26)
        s:at(3)
        s:SET_ACTIVE_SPRITE(26)
        s:at(6)
        s:SET_ACTIVE_SPRITE(26)
        s:at(9)
        s:SET_ACTIVE_SPRITE(26)
        s:at(12)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 18
    [18] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(30)
        s:at(3)
        s:SET_ACTIVE_SPRITE(31)
        s:at(6)
        s:SET_ACTIVE_SPRITE(32)
        s:at(9)
        s:SET_ACTIVE_SPRITE(33)
        s:at(12)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 19
    [19] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(34)
        s:at(3)
        s:SET_ACTIVE_SPRITE(35)
        s:at(6)
        s:SET_ACTIVE_SPRITE(36)
        ::J44::
        s:at(9)
        s:SET_ACTIVE_SPRITE(37)
        s:SET_CLOCK(6, 44)
        goto J44
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 20
    [20] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:at(3)
        s:SET_ACTIVE_SPRITE(35)
        s:at(6)
        s:SET_ACTIVE_SPRITE(36)
        ::J36::
        s:at(9)
        s:SET_ACTIVE_SPRITE(37)
        s:SET_CLOCK(6, 36)
        goto J36
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 21
    [21] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(37)
        s:at(3)
        s:SET_ACTIVE_SPRITE(36)
        s:at(6)
        s:SET_ACTIVE_SPRITE(35)
        s:at(9)
        s:SET_ACTIVE_SPRITE(34)
        ::J56::
        s:at(12)
        s:SET_ACTIVE_SPRITE(30)
        s:at(15)
        s:SET_ACTIVE_SPRITE(31)
        s:at(18)
        s:SET_ACTIVE_SPRITE(32)
        s:at(21)
        s:SET_ACTIVE_SPRITE(33)
        s:SET_CLOCK(9, 56)
        goto J56
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 22
    [22] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(37)
        s:at(3)
        s:SET_ACTIVE_SPRITE(36)
        s:at(6)
        s:SET_ACTIVE_SPRITE(35)
        s:at(9)
        s:SET_ACTIVE_SPRITE(34)
        ::J48::
        s:at(12)
        s:SET_ACTIVE_SPRITE(30)
        s:at(15)
        s:SET_ACTIVE_SPRITE(31)
        s:at(18)
        s:SET_ACTIVE_SPRITE(32)
        s:at(21)
        s:SET_ACTIVE_SPRITE(33)
        s:SET_CLOCK(9, 48)
        goto J48
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 23
    [23] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(38)
        s:at(3)
        s:SET_ACTIVE_SPRITE(39)
        s:at(6)
        s:SET_ACTIVE_SPRITE(40)
        ::J36::
        s:at(9)
        s:SET_ACTIVE_SPRITE(41)
        s:at(30009)
        s:SET_CLOCK(6, 36)
        goto J36
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
