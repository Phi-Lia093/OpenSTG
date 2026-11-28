-- ascii: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 8, 8 } },
      { id = 1, rect = { 8, 0, 8, 8 } },
      { id = 2, rect = { 16, 0, 8, 8 } },
      { id = 3, rect = { 24, 0, 8, 8 } },
      { id = 4, rect = { 32, 0, 8, 8 } },
      { id = 5, rect = { 40, 0, 8, 8 } },
      { id = 6, rect = { 48, 0, 8, 8 } },
      { id = 7, rect = { 56, 0, 8, 8 } },
      { id = 8, rect = { 64, 0, 8, 8 } },
      { id = 9, rect = { 72, 0, 8, 8 } },
      { id = 10, rect = { 80, 0, 48, 8 } },
      { id = 11, rect = { 128, 0, 8, 8 } },
      { id = 12, rect = { 136, 0, 8, 8 } },
      { id = 13, rect = { 144, 0, 8, 8 } },
      { id = 14, rect = { 152, 0, 8, 8 } },
      { id = 15, rect = { 160, 0, 8, 8 } },
      { id = 16, rect = { 168, 0, 8, 8 } },
      { id = 17, rect = { 176, 0, 8, 8 } },
      { id = 18, rect = { 184, 0, 8, 8 } },
      { id = 19, rect = { 192, 0, 8, 8 } },
      { id = 20, rect = { 200, 0, 8, 8 } },
      { id = 21, rect = { 128, 8, 8, 8 } },
      { id = 22, rect = { 136, 8, 8, 8 } },
      { id = 23, rect = { 144, 8, 8, 8 } },
      { id = 24, rect = { 152, 8, 8, 8 } },
      { id = 25, rect = { 160, 8, 8, 8 } },
      { id = 26, rect = { 168, 8, 8, 8 } },
      { id = 27, rect = { 176, 8, 8, 8 } },
      { id = 28, rect = { 184, 8, 8, 8 } },
      { id = 29, rect = { 192, 8, 8, 8 } },
      { id = 30, rect = { 200, 8, 8, 8 } },
      { id = 31, rect = { 0, 32, 16, 16 } },
      { id = 32, rect = { 16, 32, 16, 16 } },
      { id = 33, rect = { 32, 32, 16, 16 } },
      { id = 34, rect = { 48, 32, 16, 16 } },
      { id = 35, rect = { 64, 32, 16, 16 } },
      { id = 36, rect = { 80, 32, 16, 16 } },
      { id = 37, rect = { 96, 32, 16, 16 } },
      { id = 38, rect = { 112, 32, 16, 16 } },
      { id = 39, rect = { 128, 32, 16, 16 } },
      { id = 40, rect = { 144, 32, 16, 16 } },
      { id = 41, rect = { 160, 32, 16, 16 } },
      { id = 42, rect = { 176, 32, 16, 16 } },
      { id = 43, rect = { 192, 32, 16, 16 } },
      { id = 44, rect = { 208, 32, 16, 16 } },
      { id = 45, rect = { 224, 32, 16, 16 } },
      { id = 46, rect = { 240, 32, 16, 16 } },
      { id = 47, rect = { 0, 48, 16, 16 } },
      { id = 48, rect = { 16, 48, 16, 16 } },
      { id = 49, rect = { 32, 48, 16, 16 } },
      { id = 50, rect = { 48, 48, 16, 16 } },
      { id = 51, rect = { 64, 48, 16, 16 } },
      { id = 52, rect = { 80, 48, 16, 16 } },
      { id = 53, rect = { 96, 48, 16, 16 } },
      { id = 54, rect = { 112, 48, 16, 16 } },
      { id = 55, rect = { 128, 48, 16, 16 } },
      { id = 56, rect = { 144, 48, 16, 16 } },
      { id = 57, rect = { 160, 48, 16, 16 } },
      { id = 58, rect = { 176, 48, 16, 16 } },
      { id = 59, rect = { 192, 48, 16, 16 } },
      { id = 60, rect = { 208, 48, 16, 16 } },
      { id = 61, rect = { 224, 48, 16, 16 } },
      { id = 62, rect = { 240, 48, 16, 16 } },
      { id = 63, rect = { 0, 64, 16, 16 } },
      { id = 64, rect = { 16, 64, 16, 16 } },
      { id = 65, rect = { 32, 64, 16, 16 } },
      { id = 66, rect = { 48, 64, 16, 16 } },
      { id = 67, rect = { 64, 64, 16, 16 } },
      { id = 68, rect = { 80, 64, 16, 16 } },
      { id = 69, rect = { 96, 64, 16, 16 } },
      { id = 70, rect = { 112, 64, 16, 16 } },
      { id = 71, rect = { 128, 64, 16, 16 } },
      { id = 72, rect = { 144, 64, 16, 16 } },
      { id = 73, rect = { 160, 64, 16, 16 } },
      { id = 74, rect = { 176, 64, 16, 16 } },
      { id = 75, rect = { 192, 64, 16, 16 } },
      { id = 76, rect = { 208, 64, 16, 16 } },
      { id = 77, rect = { 224, 64, 16, 16 } },
      { id = 78, rect = { 240, 64, 16, 16 } },
      { id = 79, rect = { 0, 80, 16, 16 } },
      { id = 80, rect = { 16, 80, 16, 16 } },
      { id = 81, rect = { 32, 80, 16, 16 } },
      { id = 82, rect = { 48, 80, 16, 16 } },
      { id = 83, rect = { 64, 80, 16, 16 } },
      { id = 84, rect = { 80, 80, 16, 16 } },
      { id = 85, rect = { 96, 80, 16, 16 } },
      { id = 86, rect = { 112, 80, 16, 16 } },
      { id = 87, rect = { 128, 80, 16, 16 } },
      { id = 88, rect = { 144, 80, 16, 16 } },
      { id = 89, rect = { 160, 80, 16, 16 } },
      { id = 90, rect = { 176, 80, 16, 16 } },
      { id = 91, rect = { 192, 80, 16, 16 } },
      { id = 92, rect = { 208, 80, 16, 16 } },
      { id = 93, rect = { 224, 80, 16, 16 } },
      { id = 94, rect = { 240, 80, 16, 16 } },
      { id = 95, rect = { 0, 96, 16, 16 } },
      { id = 96, rect = { 16, 96, 16, 16 } },
      { id = 97, rect = { 32, 96, 16, 16 } },
      { id = 98, rect = { 48, 96, 16, 16 } },
      { id = 99, rect = { 64, 96, 16, 16 } },
      { id = 100, rect = { 80, 96, 16, 16 } },
      { id = 101, rect = { 96, 96, 16, 16 } },
      { id = 102, rect = { 112, 96, 16, 16 } },
      { id = 103, rect = { 128, 96, 16, 16 } },
      { id = 104, rect = { 144, 96, 16, 16 } },
      { id = 105, rect = { 160, 96, 16, 16 } },
      { id = 106, rect = { 176, 96, 16, 16 } },
      { id = 107, rect = { 192, 96, 16, 16 } },
      { id = 108, rect = { 208, 96, 16, 16 } },
      { id = 109, rect = { 224, 96, 16, 16 } },
      { id = 110, rect = { 240, 96, 16, 16 } },
      { id = 111, rect = { 0, 112, 16, 16 } },
      { id = 112, rect = { 16, 112, 16, 16 } },
      { id = 113, rect = { 32, 112, 16, 16 } },
      { id = 114, rect = { 48, 112, 16, 16 } },
      { id = 115, rect = { 64, 112, 16, 16 } },
      { id = 116, rect = { 80, 112, 16, 16 } },
      { id = 117, rect = { 96, 112, 16, 16 } },
      { id = 118, rect = { 112, 112, 16, 16 } },
      { id = 119, rect = { 128, 112, 16, 16 } },
      { id = 120, rect = { 144, 112, 16, 16 } },
      { id = 121, rect = { 160, 112, 16, 16 } },
      { id = 122, rect = { 176, 112, 16, 16 } },
      { id = 123, rect = { 192, 112, 16, 16 } },
      { id = 124, rect = { 208, 112, 16, 16 } },
      { id = 125, rect = { 224, 112, 16, 16 } },
      { id = 126, rect = { 240, 112, 16, 16 } },
      { id = 127, rect = { 224, 16, 16, 16 } },
      { id = 128, rect = { 240, 16, 16, 16 } },
      { id = 129, rect = { 0, 128, 256, 32 } },
      { id = 130, rect = { 0, 160, 256, 32 } },
      { id = 131, rect = { 0, 192, 144, 16 } },
      { id = 132, rect = { 0, 208, 8, 12 } },
      { id = 133, rect = { 8, 208, 8, 12 } },
      { id = 134, rect = { 16, 208, 8, 12 } },
      { id = 135, rect = { 24, 208, 8, 12 } },
      { id = 136, rect = { 32, 208, 8, 12 } },
      { id = 137, rect = { 40, 208, 8, 12 } },
      { id = 138, rect = { 48, 208, 8, 12 } },
      { id = 139, rect = { 56, 208, 8, 12 } },
      { id = 140, rect = { 64, 208, 8, 12 } },
      { id = 141, rect = { 72, 208, 8, 12 } },
      { id = 142, rect = { 0, 224, 96, 16 } },
      { id = 143, rect = { 96, 224, 48, 16 } },
      { id = 144, rect = { 0, 240, 48, 16 } },
      { id = 145, rect = { 144, 224, 112, 32 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 196 },
      { index = 2, offset = 392 },
      { index = 3, offset = 608 },
      { index = 4, offset = 648 },
      { index = 5, offset = 876 },
      { index = 6, offset = 972 },
      { index = 7, offset = 1116 },
    },
    labels = {
      [0] = { 1, 2 },
      [1] = { 1, 2 },
      [2] = { 1, 2 },
      [3] = {  },
      [4] = { -1, 1, 2, 3 },
      [5] = {  },
      [6] = { -1, 1, 2 },
      [7] = {  },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(129)
        s:SET_ZWRITE_DISABLE(1)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(30, 0, 255)
        s:SET_TRANSLATION(1134297088, 1134297088, 0)
        s:at(100)
        s:INTERP_POS(30, 4, 1134297088, 1109393408, 0)
        s:at(130)
        s:STOP()
        ::L2::
        s:LABEL(2, 130)
        s:INTERP_POS(30, 4, 1141374976, 1109393408, 0)
        s:at(160)
        s:STOP_HIDE()
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(130)
        s:SET_ZWRITE_DISABLE(1)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(30, 0, 255)
        s:SET_TRANSLATION(1125646336, 1135345664, 0)
        s:at(100)
        s:INTERP_POS(30, 4, 1125646336, 1138491392, 0)
        s:at(130)
        s:STOP()
        ::L2::
        s:LABEL(2, 130)
        s:INTERP_POS(30, 4, -1021313024, 1138491392, 0)
        s:at(160)
        s:STOP_HIDE()
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(131)
        s:SET_ZWRITE_DISABLE(1)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(30, 0, 255)
        s:SET_TRANSLATION(1134559232, 1135345664, 0)
        s:at(30)
        s:INTERP_POS(30, 0, 1134559232, 1134690304, 0)
        s:at(100)
        s:INTERP_POS(30, 4, 1134559232, 1112539136, 0)
        s:at(130)
        s:STOP()
        ::L2::
        s:LABEL(2, 130)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(160)
        s:STOP_HIDE()
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(132)
        s:SET_ZWRITE_DISABLE(1)
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        if at == 3 then goto L3 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(142)
        s:SET_ZWRITE_DISABLE(1)
        s:NOP22()
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(15, 0, 255)
        s:SET_TRANSLATION(-1027604480, 1138786304, 0)
        s:INTERP_POS(15, 0, 1107296256, 1138786304, 0)
        s:at(15)
        s:STOP()
        ::L2::
        s:LABEL(2, 15)
        s:INTERP_ALPHA(15, 0, 64)
        s:at(30)
        s:STOP()
        ::L3::
        s:LABEL(3, 30)
        s:INTERP_ALPHA(15, 0, 255)
        s:at(45)
        s:STOP()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(143)
        s:SET_ZWRITE_DISABLE(1)
        ::J24::
        s:INTERP_SCALE_2(30, 4, 1067030938, 1067030938)
        s:at(30)
        s:INTERP_SCALE_2(30, 1, 1061997773, 1061997773)
        s:at(60)
        s:SET_CLOCK(0, 24)
        goto J24
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 6
    [6] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(144)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(30, 0, 255)
        s:at(30)
        s:STOP()
        ::L2::
        s:LABEL(2, 30)
        s:INTERP_ALPHA(30, 0, 0)
        s:at(60)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(145)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1130364928, 1131413504, 0)
        ::J56::
        s:INTERP_ALPHA(120, 0, 255)
        s:at(120)
        s:INTERP_ALPHA(60, 0, 64)
        s:at(180)
        s:SET_CLOCK(0, 56)
        goto J56
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
  [1] = {
    texture = "texture_1.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 8, 8 } },
      { id = 1, rect = { 8, 0, 8, 8 } },
      { id = 2, rect = { 16, 0, 8, 8 } },
      { id = 3, rect = { 24, 0, 8, 8 } },
      { id = 4, rect = { 32, 0, 8, 8 } },
      { id = 5, rect = { 40, 0, 8, 8 } },
      { id = 6, rect = { 48, 0, 8, 8 } },
      { id = 7, rect = { 56, 0, 8, 8 } },
      { id = 8, rect = { 64, 0, 8, 8 } },
      { id = 9, rect = { 72, 0, 8, 8 } },
      { id = 10, rect = { 0, 32, 16, 16 } },
      { id = 11, rect = { 16, 32, 16, 16 } },
      { id = 12, rect = { 32, 32, 16, 16 } },
      { id = 13, rect = { 48, 32, 16, 16 } },
      { id = 14, rect = { 64, 32, 16, 16 } },
      { id = 15, rect = { 80, 32, 16, 16 } },
      { id = 16, rect = { 96, 32, 16, 16 } },
      { id = 17, rect = { 112, 32, 16, 16 } },
      { id = 18, rect = { 128, 32, 16, 16 } },
      { id = 19, rect = { 144, 32, 16, 16 } },
      { id = 20, rect = { 160, 32, 16, 16 } },
      { id = 21, rect = { 176, 32, 16, 16 } },
      { id = 22, rect = { 192, 32, 16, 16 } },
      { id = 23, rect = { 208, 32, 16, 16 } },
      { id = 24, rect = { 224, 32, 16, 16 } },
      { id = 25, rect = { 240, 32, 16, 16 } },
      { id = 26, rect = { 0, 48, 16, 16 } },
      { id = 27, rect = { 16, 48, 16, 16 } },
      { id = 28, rect = { 32, 48, 16, 16 } },
      { id = 29, rect = { 48, 48, 16, 16 } },
      { id = 30, rect = { 64, 48, 16, 16 } },
      { id = 31, rect = { 80, 48, 16, 16 } },
      { id = 32, rect = { 96, 48, 16, 16 } },
      { id = 33, rect = { 112, 48, 16, 16 } },
      { id = 34, rect = { 128, 48, 16, 16 } },
      { id = 35, rect = { 144, 48, 16, 16 } },
      { id = 36, rect = { 160, 48, 16, 16 } },
      { id = 37, rect = { 176, 48, 16, 16 } },
      { id = 38, rect = { 192, 48, 16, 16 } },
      { id = 39, rect = { 208, 48, 16, 16 } },
      { id = 40, rect = { 224, 48, 16, 16 } },
      { id = 41, rect = { 240, 48, 16, 16 } },
      { id = 42, rect = { 0, 64, 16, 16 } },
      { id = 43, rect = { 16, 64, 16, 16 } },
      { id = 44, rect = { 32, 64, 16, 16 } },
      { id = 45, rect = { 48, 64, 16, 16 } },
      { id = 46, rect = { 64, 64, 16, 16 } },
      { id = 47, rect = { 80, 64, 16, 16 } },
      { id = 48, rect = { 96, 64, 16, 16 } },
      { id = 49, rect = { 112, 64, 16, 16 } },
      { id = 50, rect = { 128, 64, 16, 16 } },
      { id = 51, rect = { 144, 64, 16, 16 } },
      { id = 52, rect = { 160, 64, 16, 16 } },
      { id = 53, rect = { 176, 64, 16, 16 } },
      { id = 54, rect = { 192, 64, 16, 16 } },
      { id = 55, rect = { 208, 64, 16, 16 } },
      { id = 56, rect = { 224, 64, 16, 16 } },
      { id = 57, rect = { 240, 64, 16, 16 } },
      { id = 58, rect = { 0, 80, 16, 16 } },
      { id = 59, rect = { 16, 80, 16, 16 } },
      { id = 60, rect = { 32, 80, 16, 16 } },
      { id = 61, rect = { 48, 80, 16, 16 } },
      { id = 62, rect = { 64, 80, 16, 16 } },
      { id = 63, rect = { 80, 80, 16, 16 } },
      { id = 64, rect = { 96, 80, 16, 16 } },
      { id = 65, rect = { 112, 80, 16, 16 } },
      { id = 66, rect = { 128, 80, 16, 16 } },
      { id = 67, rect = { 144, 80, 16, 16 } },
      { id = 68, rect = { 160, 80, 16, 16 } },
      { id = 69, rect = { 176, 80, 16, 16 } },
      { id = 70, rect = { 192, 80, 16, 16 } },
      { id = 71, rect = { 208, 80, 16, 16 } },
      { id = 72, rect = { 224, 80, 16, 16 } },
      { id = 73, rect = { 240, 80, 16, 16 } },
      { id = 74, rect = { 0, 96, 16, 16 } },
      { id = 75, rect = { 16, 96, 16, 16 } },
      { id = 76, rect = { 32, 96, 16, 16 } },
      { id = 77, rect = { 48, 96, 16, 16 } },
      { id = 78, rect = { 64, 96, 16, 16 } },
      { id = 79, rect = { 80, 96, 16, 16 } },
      { id = 80, rect = { 96, 96, 16, 16 } },
      { id = 81, rect = { 112, 96, 16, 16 } },
      { id = 82, rect = { 128, 96, 16, 16 } },
      { id = 83, rect = { 144, 96, 16, 16 } },
      { id = 84, rect = { 160, 96, 16, 16 } },
      { id = 85, rect = { 176, 96, 16, 16 } },
      { id = 86, rect = { 192, 96, 16, 16 } },
      { id = 87, rect = { 208, 96, 16, 16 } },
      { id = 88, rect = { 224, 96, 16, 16 } },
      { id = 89, rect = { 240, 96, 16, 16 } },
      { id = 90, rect = { 0, 112, 16, 16 } },
      { id = 91, rect = { 16, 112, 16, 16 } },
      { id = 92, rect = { 32, 112, 16, 16 } },
      { id = 93, rect = { 48, 112, 16, 16 } },
      { id = 94, rect = { 64, 112, 16, 16 } },
      { id = 95, rect = { 80, 112, 16, 16 } },
      { id = 96, rect = { 96, 112, 16, 16 } },
      { id = 97, rect = { 112, 112, 16, 16 } },
      { id = 98, rect = { 128, 112, 16, 16 } },
      { id = 99, rect = { 144, 112, 16, 16 } },
      { id = 100, rect = { 160, 112, 16, 16 } },
      { id = 101, rect = { 176, 112, 16, 16 } },
      { id = 102, rect = { 192, 112, 16, 16 } },
      { id = 103, rect = { 208, 112, 16, 16 } },
      { id = 104, rect = { 224, 112, 16, 16 } },
      { id = 105, rect = { 240, 112, 16, 16 } },
      { id = 106, rect = { 224, 16, 16, 16 } },
      { id = 107, rect = { 240, 16, 16, 16 } },
    },
    scripts = {},
    labels = {
    },
  },
  [2] = {
    texture = "texture_2.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 112, 32 } },
      { id = 1, rect = { 0, 32, 160, 32 } },
      { id = 2, rect = { 0, 64, 224, 32 } },
      { id = 3, rect = { 0, 96, 192, 32 } },
      { id = 4, rect = { 160, 32, 96, 32 } },
      { id = 5, rect = { 0, 128, 224, 32 } },
      { id = 6, rect = { 0, 160, 112, 32 } },
      { id = 7, rect = { 112, 160, 64, 32 } },
      { id = 8, rect = { 176, 160, 80, 32 } },
      { id = 9, rect = { 0, 192, 32, 32 } },
      { id = 10, rect = { 32, 192, 32, 32 } },
      { id = 11, rect = { 64, 192, 32, 32 } },
      { id = 12, rect = { 96, 192, 32, 32 } },
      { id = 13, rect = { 128, 192, 32, 32 } },
      { id = 14, rect = { 0, 224, 160, 32 } },
      { id = 15, rect = { 192, 0, 64, 16 } },
      { id = 16, rect = { 192, 16, 64, 16 } },
      { id = 17, rect = { 128, 0, 64, 16 } },
      { id = 18, rect = { 128, 16, 64, 16 } },
      { id = 19, rect = { 192, 192, 64, 16 } },
      { id = 20, rect = { 192, 208, 64, 16 } },
      { id = 21, rect = { 192, 224, 64, 16 } },
      { id = 22, rect = { 192, 240, 64, 16 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 196 },
      { index = 2, offset = 392 },
      { index = 3, offset = 588 },
      { index = 4, offset = 784 },
      { index = 5, offset = 980 },
      { index = 6, offset = 1176 },
      { index = 7, offset = 1372 },
      { index = 8, offset = 1512 },
      { index = 9, offset = 1652 },
      { index = 10, offset = 1792 },
      { index = 11, offset = 1988 },
      { index = 12, offset = 2184 },
      { index = 13, offset = 2380 },
      { index = 14, offset = 2576 },
    },
    labels = {
      [0] = { 1, 2 },
      [1] = { 1, 2 },
      [2] = { 1, 2 },
      [3] = { 1, 2 },
      [4] = { 1, 2 },
      [5] = { 1, 2 },
      [6] = { 1, 2 },
      [7] = { 1, 2 },
      [8] = { 1, 2 },
      [9] = { 1, 2 },
      [10] = { 1, 2 },
      [11] = { 1, 2 },
      [12] = { 1, 2 },
      [13] = { 1, 2 },
      [14] = { 1, 2 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_TRANSLATION(1130364928, 1128267776, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1128267776, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1130364928, 1128267776, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_TRANSLATION(1132462080, 1132462080, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1132462080, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1132462080, 1132462080, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_TRANSLATION(1128267776, 1133641728, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1133641728, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1128267776, 1133641728, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_TRANSLATION(1131413504, 1134821376, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1134821376, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1131413504, 1134821376, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_TRANSLATION(1130364928, 1128267776, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1128267776, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1130364928, 1128267776, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:SET_TRANSLATION(1130364928, 1132462080, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1132462080, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1130364928, 1132462080, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 6
    [6] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:SET_TRANSLATION(1130364928, 1133641728, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1133641728, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1130364928, 1133641728, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 7
    [7] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(15)
        s:NOP22()
        s:SET_TRANSLATION(1135345664, 1138622464, 0)
        s:SET_ALPHA(0)
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 8
    [8] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(22)
        s:NOP22()
        s:SET_TRANSLATION(1135345664, 1138098176, 0)
        s:SET_ALPHA(0)
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 9
    [9] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(21)
        s:NOP22()
        s:SET_TRANSLATION(1135345664, 1137573888, 0)
        s:SET_ALPHA(0)
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 10
    [10] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_TRANSLATION(1130364928, 1128267776, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1128267776, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1130364928, 1128267776, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 11
    [11] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:SET_TRANSLATION(1130364928, 1130364928, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1130364928, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1130364928, 1130364928, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 12
    [12] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:SET_TRANSLATION(1130364928, 1132462080, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1132462080, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1130364928, 1132462080, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 13
    [13] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:SET_TRANSLATION(1130364928, 1133641728, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130364928, 1133641728, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1130364928, 1133641728, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 14
    [14] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        if at == 2 then goto L2 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1130692608, 1130364928, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 0)
        s:INTERP_ALPHA(20, 0, 255)
        s:INTERP_POS(20, 0, 1130692608, 1130364928, 0)
        s:at(20)
        s:STOP()
        ::L2::
        s:LABEL(2, 20)
        s:INTERP_ALPHA(20, 0, 0)
        s:INTERP_POS(20, 0, 1130692608, 1130364928, 0)
        s:at(40)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
}
