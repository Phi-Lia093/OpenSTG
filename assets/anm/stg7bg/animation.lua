-- stg7bg: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 1, 256, 254, 32 } },
      { id = 1, rect = { 1, 288, 254, 32 } },
      { id = 2, rect = { 1, 320, 254, 32 } },
      { id = 3, rect = { 1, 352, 254, 32 } },
      { id = 4, rect = { 1, 384, 254, 32 } },
      { id = 5, rect = { 1, 416, 254, 32 } },
      { id = 6, rect = { 1, 448, 254, 32 } },
      { id = 7, rect = { 1, 480, 254, 32 } },
      { id = 8, rect = { 1, 1, 254, 254 } },
      { id = 9, rect = { 257, 256, 254, 128 } },
      { id = 10, rect = { 257, 384, 254, 128 } },
      { id = 11, rect = { 1, 257, 254, 254 } },
      { id = 12, rect = { 1, 257, 32, 254 } },
      { id = 13, rect = { 33, 257, 32, 254 } },
      { id = 14, rect = { 65, 257, 32, 254 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 100 },
      { index = 2, offset = 200 },
      { index = 3, offset = 300 },
      { index = 4, offset = 400 },
      { index = 5, offset = 500 },
      { index = 6, offset = 600 },
      { index = 7, offset = 700 },
      { index = 8, offset = 800 },
      { index = 9, offset = 880 },
      { index = 10, offset = 960 },
      { index = 11, offset = 1040 },
      { index = 12, offset = 1120 },
      { index = 13, offset = 1200 },
      { index = 14, offset = 1280 },
      { index = 15, offset = 1360 },
      { index = 16, offset = 1440 },
      { index = 17, offset = 1508 },
      { index = 18, offset = 1700 },
      { index = 19, offset = 1900 },
      { index = 20, offset = 2092 },
      { index = 21, offset = 2292 },
      { index = 22, offset = 2340 },
      { index = 23, offset = 2400 },
      { index = 24, offset = 2472 },
      { index = 25, offset = 2544 },
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
      [24] = {  },
      [25] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, -1073741824)
        s:SET_ROTATION(-1102508069, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, -1073741824)
        s:SET_ROTATION(-1102508069, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, -1073741824)
        s:SET_ROTATION(-1102508069, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, -1073741824)
        s:SET_ROTATION(-1102508069, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, -1073741824)
        s:SET_ROTATION(-1102508069, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, -1073741824)
        s:SET_ROTATION(-1102508069, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, -1073741824)
        s:SET_ROTATION(-1102508069, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:SET_USE_OFFSET(1)
        s:SET_TRANSLATION(0, 0, -1073741824)
        s:SET_ROTATION(-1102508069, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_COLOR(4210752)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 9
    [9] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_COLOR(4210752)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 10
    [10] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_COLOR(4210752)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 11
    [11] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_COLOR(4210752)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 12
    [12] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_COLOR(4210752)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 13
    [13] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_COLOR(4210752)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 14
    [14] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_COLOR(4210752)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 15
    [15] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:SET_CAMERA_MODE(1)
        s:SET_ROTATION(1070141403, 0, 0)
        s:SET_COLOR(4210752)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 16
    [16] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:SET_ROTATION(-1085730853, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 17
    [17] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:SET_AUTO_ROTATE(2)
        s:SET_USE_OFFSET(1)
        ::J56::
        s:INTERP_POS(150, 1, 0, 0, 0)
        s:at(150)
        s:INTERP_POS(150, 4, -1048576000, 0, 0)
        s:at(300)
        s:INTERP_POS(150, 1, 0, 0, 0)
        s:at(450)
        s:INTERP_POS(150, 4, 1098907648, 0, 0)
        s:at(600)
        s:SET_CLOCK(0, 56)
        goto J56
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 18
    [18] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(10)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:SET_AUTO_ROTATE(2)
        s:at(100)
        s:SET_USE_OFFSET(1)
        ::J56::
        s:INTERP_POS(150, 1, 0, 0, 0)
        s:at(250)
        s:INTERP_POS(150, 4, -1048576000, 0, 0)
        s:at(400)
        s:INTERP_POS(150, 1, 0, 0, 0)
        s:at(550)
        s:INTERP_POS(150, 4, 1098907648, 0, 0)
        s:at(700)
        s:SET_CLOCK(100, 56)
        goto J56
        s:EXIT()
    end,

    -- stream 19
    [19] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:SET_AUTO_ROTATE(2)
        s:at(200)
        s:SET_USE_OFFSET(1)
        ::J56::
        s:INTERP_POS(150, 1, 0, 0, 0)
        s:at(350)
        s:INTERP_POS(150, 4, -1048576000, 0, 0)
        s:at(500)
        s:INTERP_POS(150, 1, 0, 0, 0)
        s:at(650)
        s:INTERP_POS(150, 4, 1098907648, 0, 0)
        s:at(800)
        s:SET_CLOCK(200, 56)
        goto J56
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 20
    [20] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(10)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:SET_AUTO_ROTATE(2)
        s:at(150)
        s:SET_USE_OFFSET(1)
        ::J56::
        s:INTERP_POS(150, 1, 0, 0, 0)
        s:at(300)
        s:INTERP_POS(150, 4, -1048576000, 0, 0)
        s:at(450)
        s:INTERP_POS(150, 1, 0, 0, 0)
        s:at(600)
        s:INTERP_POS(150, 4, 1098907648, 0, 0)
        s:at(750)
        s:SET_CLOCK(150, 56)
        goto J56
        s:EXIT()
    end,

    -- stream 21
    [21] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:SET_CAMERA_MODE(1)
        s:NOP22()
        s:EXIT()
    end,

    -- stream 22
    [22] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(12)
        s:SET_ROTATION(1075235812, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:EXIT()
    end,

    -- stream 23
    [23] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(13)
        s:SET_ROTATION(1070141403, 1075235812, 1070141403)
        s:SET_CAMERA_MODE(1)
        s:SET_COLOR(3158064)
        s:at(3000)
        s:EXIT()
    end,

    -- stream 24
    [24] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(13)
        s:SET_ROTATION(1070141403, 1075235812, 1070141403)
        s:SET_CAMERA_MODE(1)
        s:SET_COLOR(4210752)
        s:EXIT()
    end,

    -- stream 25
    [25] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(14)
        s:SET_ROTATION(1075235812, 0, 0)
        s:SET_CAMERA_MODE(1)
        s:EXIT()
    end,
  },
}
