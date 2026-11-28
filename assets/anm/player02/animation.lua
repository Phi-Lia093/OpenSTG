-- player02: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 32, 48 } },
      { id = 1, rect = { 32, 0, 32, 48 } },
      { id = 2, rect = { 64, 0, 32, 48 } },
      { id = 3, rect = { 96, 0, 32, 48 } },
      { id = 4, rect = { 0, 48, 32, 48 } },
      { id = 5, rect = { 32, 48, 32, 48 } },
      { id = 6, rect = { 64, 48, 32, 48 } },
      { id = 7, rect = { 96, 48, 32, 48 } },
      { id = 8, rect = { 128, 48, 32, 48 } },
      { id = 9, rect = { 160, 48, 32, 48 } },
      { id = 10, rect = { 192, 48, 32, 48 } },
      { id = 64, rect = { 128, 0, 16, 32 } },
      { id = 65, rect = { 144, 0, 16, 32 } },
      { id = 66, rect = { 1, 129, 126, 126 } },
      { id = 67, rect = { 129, 129, 126, 126 } },
      { id = 128, rect = { 128, 32, 16, 16 } },
      { id = 129, rect = { 144, 32, 16, 16 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 72 },
      { index = 2, offset = 172 },
      { index = 3, offset = 328 },
      { index = 4, offset = 436 },
      { index = 5, offset = 600 },
      { index = 6, offset = 724 },
      { index = 7, offset = 848 },
      { index = 8, offset = 972 },
      { index = 9, offset = 1072 },
      { index = 10, offset = 1216 },
      { index = 11, offset = 1360 },
      { index = 12, offset = 1516 },
      { index = 13, offset = 1660 },
      { index = 14, offset = 1784 },
      { index = 64, offset = 1908 },
      { index = 65, offset = 1988 },
      { index = 96, offset = 2068 },
      { index = 97, offset = 2188 },
      { index = 128, offset = 2308 },
      { index = 129, offset = 2344 },
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
        s:at(2)
        s:SET_ACTIVE_SPRITE(5)
        s:at(4)
        s:SET_ACTIVE_SPRITE(6)
        s:at(6)
        s:SET_ACTIVE_SPRITE(7)
        s:at(8)
        s:SET_ACTIVE_SPRITE(8)
        s:at(10)
        s:SET_ACTIVE_SPRITE(9)
        s:at(12)
        s:SET_ACTIVE_SPRITE(10)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(10)
        s:at(2)
        s:SET_ACTIVE_SPRITE(9)
        s:at(4)
        s:SET_ACTIVE_SPRITE(8)
        s:at(6)
        s:SET_ACTIVE_SPRITE(7)
        s:at(8)
        s:SET_ACTIVE_SPRITE(6)
        s:at(10)
        s:SET_ACTIVE_SPRITE(5)
        s:at(12)
        s:SET_ACTIVE_SPRITE(4)
        ::J84::
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:at(8)
        s:SET_ACTIVE_SPRITE(1)
        s:at(16)
        s:SET_ACTIVE_SPRITE(2)
        s:at(24)
        s:SET_ACTIVE_SPRITE(3)
        s:at(32)
        s:SET_CLOCK(12, 84)
        goto J84
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(4)
        s:at(2)
        s:SET_ACTIVE_SPRITE(5)
        s:at(4)
        s:SET_ACTIVE_SPRITE(6)
        s:at(6)
        s:SET_ACTIVE_SPRITE(7)
        s:at(8)
        s:SET_ACTIVE_SPRITE(8)
        s:at(10)
        s:SET_ACTIVE_SPRITE(9)
        s:at(12)
        s:SET_ACTIVE_SPRITE(10)
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:FLIP_X()
        s:SET_ACTIVE_SPRITE(10)
        s:at(2)
        s:SET_ACTIVE_SPRITE(9)
        s:at(4)
        s:SET_ACTIVE_SPRITE(8)
        s:at(6)
        s:SET_ACTIVE_SPRITE(7)
        s:at(8)
        s:SET_ACTIVE_SPRITE(6)
        s:at(10)
        s:SET_ACTIVE_SPRITE(5)
        s:at(12)
        s:SET_ACTIVE_SPRITE(4)
        ::J92::
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:at(8)
        s:SET_ACTIVE_SPRITE(1)
        s:at(16)
        s:SET_ACTIVE_SPRITE(2)
        s:at(24)
        s:SET_ACTIVE_SPRITE(3)
        s:at(32)
        s:SET_CLOCK(12, 92)
        goto J92
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ALPHA(255)
        s:SET_ACTIVE_SPRITE(64)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1072064102, 1072064102)
        s:INTERP_SCALE_2(16, 0, 1072064102, 1077936128)
        s:SET_AUTO_ROTATE(1)
        s:at(60)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ALPHA(255)
        s:SET_ACTIVE_SPRITE(65)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1072064102, 1072064102)
        s:INTERP_SCALE_2(16, 0, 1072064102, 1077936128)
        s:SET_AUTO_ROTATE(1)
        s:at(60)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ALPHA(255)
        s:SET_ACTIVE_SPRITE(64)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1072064102, 1077936128)
        s:INTERP_SCALE_2(16, 0, 1072064102, 1077936128)
        s:SET_AUTO_ROTATE(1)
        s:at(150)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ALPHA(255)
        s:SET_ACTIVE_SPRITE(65)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1072064102, 1077936128)
        s:SET_AUTO_ROTATE(1)
        s:at(150)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 9
    [9] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(66)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:SET_ZWRITE_DISABLE(1)
        s:INTERP_ALPHA(30, 0, 128)
        s:SET_SCALE(1086324736, 1086324736)
        s:INTERP_SCALE_2(90, 0, 1045220557, 1045220557)
        s:at(70)
        s:INTERP_ALPHA(20, 0, 0)
        s:at(90)
        s:EXIT()
    end,

    -- stream 10
    [10] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(67)
        s:SET_BLEND(0)
        s:SET_ALPHA(0)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20)
        s:INTERP_ALPHA(30, 0, 128)
        s:SET_SCALE(1086324736, 1086324736)
        s:INTERP_SCALE_2(90, 0, 1045220557, 1045220557)
        s:at(90)
        s:INTERP_ALPHA(20, 0, 0)
        s:at(110)
        s:EXIT()
    end,

    -- stream 11
    [11] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(66)
        s:SET_BLEND(0)
        s:SET_BLEND(1)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:at(40)
        s:INTERP_ALPHA(30, 0, 128)
        s:SET_SCALE(0, 0)
        s:INTERP_SCALE_2(90, 0, 1086324736, 1086324736)
        s:at(110)
        s:INTERP_ALPHA(20, 0, 0)
        s:at(130)
        s:EXIT()
    end,

    -- stream 12
    [12] = function(s)
        s:at(0)
        s:SET_ALPHA(0)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(67)
        s:SET_ZWRITE_DISABLE(1)
        s:at(60)
        s:INTERP_ALPHA(30, 0, 128)
        s:SET_SCALE(0, 0)
        s:INTERP_SCALE_2(90, 0, 1086324736, 1086324736)
        s:at(130)
        s:INTERP_ALPHA(20, 0, 0)
        s:at(150)
        s:EXIT()
    end,

    -- stream 13
    [13] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(66)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ANGLE_VEL(0, 0, 1032194706)
        s:INTERP_ALPHA(30, 0, 196)
        s:at(280)
        s:INTERP_ALPHA(20, 0, 0)
        s:at(300)
        s:EXIT()
    end,

    -- stream 14
    [14] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(67)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ANGLE_VEL(0, 0, -1110896677)
        s:INTERP_ALPHA(30, 0, 196)
        s:at(280)
        s:INTERP_ALPHA(20, 0, 0)
        s:at(300)
        s:EXIT()
    end,

    -- stream 15
    [15] = function(s)
        s:at(0)
        s:SET_ALPHA(96)
        s:SET_ACTIVE_SPRITE(64)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_AUTO_ROTATE(1)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 16
    [16] = function(s)
        s:at(0)
        s:SET_ALPHA(96)
        s:SET_ACTIVE_SPRITE(65)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_AUTO_ROTATE(1)
        s:at(20000)
        s:EXIT()
    end,

    -- stream 17
    [17] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(64)
        s:SET_ALPHA(96)
        s:INTERP_ALPHA(20, 3, 0)
        s:INTERP_SCALE_2(20, 4, 1090519040, 1036831949)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_AUTO_ROTATE(1)
        s:at(20)
        s:EXIT_HIDE2()
    end,

    -- stream 18
    [18] = function(s)
        s:at(0)
        s:SET_BLEND(1)
        s:SET_ACTIVE_SPRITE(65)
        s:SET_ALPHA(96)
        s:INTERP_ALPHA(20, 3, 0)
        s:INTERP_SCALE_2(20, 4, 1090519040, 1036831949)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_AUTO_ROTATE(1)
        s:at(20)
        s:EXIT_HIDE2()
    end,

    -- stream 19
    [19] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(128)
        s:at(10000)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 20
    [20] = function(s)
        ::J0::
        s:at(0)
        s:SET_ACTIVE_SPRITE(128)
        s:at(10000)
        s:SET_CLOCK(0, 0)
        goto J0
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
