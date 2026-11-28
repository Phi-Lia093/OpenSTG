-- music00: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 64 } },
    },
    scripts = {
      { index = 0, offset = 0 },
    },
    labels = {
      [0] = { -1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_TRANSLATION(1103101952, -1031798784, 0)
        s:POS_TIME_DECEL(24.0, 24.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -64.0, 0.0, 30)
        s:at(60)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
  [1] = {
    texture = "@",
    width = 512, height = 512, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 512, 17 } },
      { id = 1, rect = { 0, 17, 512, 17 } },
      { id = 2, rect = { 0, 34, 512, 17 } },
      { id = 3, rect = { 0, 51, 512, 17 } },
      { id = 4, rect = { 0, 68, 512, 17 } },
      { id = 5, rect = { 0, 85, 512, 17 } },
      { id = 6, rect = { 0, 102, 512, 17 } },
      { id = 7, rect = { 0, 119, 512, 17 } },
      { id = 8, rect = { 0, 136, 512, 17 } },
      { id = 9, rect = { 0, 153, 512, 17 } },
      { id = 10, rect = { 0, 170, 512, 17 } },
      { id = 11, rect = { 0, 187, 512, 17 } },
      { id = 12, rect = { 0, 204, 512, 17 } },
      { id = 13, rect = { 0, 221, 512, 17 } },
      { id = 14, rect = { 0, 238, 512, 17 } },
      { id = 15, rect = { 0, 255, 512, 17 } },
      { id = 16, rect = { 0, 272, 512, 17 } },
      { id = 17, rect = { 0, 289, 512, 17 } },
      { id = 18, rect = { 0, 306, 512, 17 } },
      { id = 19, rect = { 0, 323, 512, 17 } },
      { id = 20, rect = { 0, 340, 512, 17 } },
      { id = 21, rect = { 0, 357, 512, 17 } },
      { id = 22, rect = { 0, 374, 512, 17 } },
      { id = 23, rect = { 0, 391, 512, 17 } },
      { id = 24, rect = { 0, 408, 512, 17 } },
      { id = 25, rect = { 0, 425, 512, 17 } },
      { id = 26, rect = { 0, 442, 512, 17 } },
      { id = 27, rect = { 0, 459, 512, 17 } },
      { id = 28, rect = { 0, 476, 512, 17 } },
      { id = 29, rect = { 0, 493, 512, 17 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 200 },
      { index = 2, offset = 400 },
      { index = 3, offset = 600 },
      { index = 4, offset = 800 },
      { index = 5, offset = 1000 },
      { index = 6, offset = 1200 },
      { index = 7, offset = 1400 },
      { index = 8, offset = 1600 },
      { index = 9, offset = 1800 },
      { index = 10, offset = 2000 },
      { index = 11, offset = 2200 },
      { index = 12, offset = 2400 },
      { index = 13, offset = 2600 },
      { index = 14, offset = 2800 },
      { index = 15, offset = 3000 },
      { index = 16, offset = 3200 },
      { index = 17, offset = 3400 },
      { index = 18, offset = 3600 },
      { index = 19, offset = 3800 },
      { index = 20, offset = 4000 },
      { index = 21, offset = 4200 },
      { index = 22, offset = 4400 },
      { index = 23, offset = 4600 },
      { index = 24, offset = 4800 },
      { index = 25, offset = 5000 },
      { index = 26, offset = 5200 },
      { index = 27, offset = 5400 },
      { index = 28, offset = 5600 },
      { index = 29, offset = 5800 },
    },
    labels = {
      [0] = { 1, 2, -1 },
      [1] = { 1, 2, -1 },
      [2] = { 1, 2, -1 },
      [3] = { 1, 2, -1 },
      [4] = { 1, 2, -1 },
      [5] = { 1, 2, -1 },
      [6] = { 1, 2, -1 },
      [7] = { 1, 2, -1 },
      [8] = { 1, 2, -1 },
      [9] = { 1, 2, -1 },
      [10] = { 1, 2, -1 },
      [11] = { 1, 2, -1 },
      [12] = { 1, 2, -1 },
      [13] = { 1, 2, -1 },
      [14] = { 1, 2, -1 },
      [15] = { 1, 2, -1 },
      [16] = { 1, 2, -1 },
      [17] = { 1, 2, -1 },
      [18] = { 1, 2, -1 },
      [19] = { 1, 2, -1 },
      [20] = { 1, 2, -1 },
      [21] = { 1, 2, -1 },
      [22] = { 1, 2, -1 },
      [23] = { 1, 2, -1 },
      [24] = { 1, 2, -1 },
      [25] = { 1, 2, -1 },
      [26] = { 1, 2, -1 },
      [27] = { 1, 2, -1 },
      [28] = { 1, 2, -1 },
      [29] = { 1, 2, -1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 5
    [5] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 6
    [6] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 7
    [7] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 8
    [8] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 9
    [9] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 10
    [10] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(10)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 11
    [11] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(11)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 12
    [12] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(12)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 13
    [13] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(13)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 14
    [14] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(14)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 15
    [15] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(15)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 16
    [16] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(16)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 17
    [17] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(17)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 18
    [18] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(18)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 19
    [19] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(19)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 20
    [20] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(20)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 21
    [21] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(21)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 22
    [22] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(22)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 23
    [23] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(23)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 24
    [24] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(24)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 25
    [25] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(25)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 26
    [26] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(26)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 27
    [27] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(27)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 28
    [28] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(28)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,

    -- stream 29
    [29] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(29)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(16, 0, 255)
        s:INTERP_COLOR(16, 0, 16777215)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(16, 0, 144)
        s:INTERP_COLOR(16, 0, 8421504)
        s:at(40)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 40)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(70)
        s:EXIT_HIDE2()
    end,
  },
}
