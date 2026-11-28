-- title01: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 512, 256 } },
    },
    scripts = {
      { index = 0, offset = 0 },
    },
    labels = {
      [0] = { 2, 5, 3, -1, 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 5 then goto L5 end
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(-1035993088, -1040187392, 0)
        ::L2::
        s:LABEL(2, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(-1035993088, -1040187392, 0, 60)
        s:at(60)
        s:STOP()
        ::L5::
        s:LABEL(5, 60)
        ::L3::
        s:LABEL(3, 60)
        s:POS_TIME_LINEAR(-1035993088, -1015021568, 0, 32)
        s:FADE(0, 32)
        ::Lm1::
        s:LABEL(-1, 92)
        s:at(92)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 92)
        s:FADE(0, 32)
        s:at(124)
        s:EXIT_HIDE2()
    end,
  },
  [1] = {
    texture = "texture_1.png",
    width = 512, height = 512, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 80, 32 } },
      { id = 1, rect = { 0, 64, 80, 32 } },
      { id = 2, rect = { 80, 0, 160, 32 } },
      { id = 3, rect = { 80, 64, 160, 32 } },
      { id = 4, rect = { 240, 0, 192, 32 } },
      { id = 5, rect = { 240, 64, 192, 32 } },
      { id = 6, rect = { 0, 32, 96, 32 } },
      { id = 7, rect = { 0, 96, 96, 32 } },
      { id = 8, rect = { 96, 32, 96, 32 } },
      { id = 9, rect = { 96, 96, 96, 32 } },
      { id = 10, rect = { 192, 32, 160, 32 } },
      { id = 11, rect = { 192, 96, 160, 32 } },
      { id = 12, rect = { 352, 32, 96, 32 } },
      { id = 13, rect = { 352, 96, 96, 32 } },
      { id = 14, rect = { 432, 0, 64, 32 } },
      { id = 15, rect = { 432, 64, 64, 32 } },
      { id = 16, rect = { 0, 128, 96, 32 } },
      { id = 17, rect = { 0, 192, 96, 32 } },
      { id = 18, rect = { 96, 128, 112, 32 } },
      { id = 19, rect = { 96, 192, 112, 32 } },
      { id = 20, rect = { 208, 128, 80, 32 } },
      { id = 21, rect = { 208, 192, 80, 32 } },
      { id = 22, rect = { 288, 128, 96, 32 } },
      { id = 23, rect = { 288, 192, 96, 32 } },
      { id = 24, rect = { 384, 128, 80, 32 } },
      { id = 25, rect = { 384, 192, 80, 32 } },
      { id = 26, rect = { 0, 160, 144, 32 } },
      { id = 27, rect = { 0, 224, 144, 32 } },
      { id = 28, rect = { 144, 160, 96, 32 } },
      { id = 29, rect = { 144, 224, 96, 32 } },
      { id = 30, rect = { 240, 160, 144, 32 } },
      { id = 31, rect = { 240, 224, 144, 32 } },
      { id = 32, rect = { 384, 160, 64, 32 } },
      { id = 33, rect = { 384, 224, 64, 32 } },
      { id = 34, rect = { 0, 256, 32, 32 } },
      { id = 35, rect = { 0, 320, 32, 32 } },
      { id = 36, rect = { 32, 256, 32, 32 } },
      { id = 37, rect = { 32, 320, 32, 32 } },
      { id = 38, rect = { 64, 256, 32, 32 } },
      { id = 39, rect = { 64, 320, 32, 32 } },
      { id = 40, rect = { 96, 256, 32, 32 } },
      { id = 41, rect = { 96, 320, 32, 32 } },
      { id = 42, rect = { 128, 256, 32, 32 } },
      { id = 43, rect = { 128, 320, 32, 32 } },
      { id = 44, rect = { 160, 256, 32, 32 } },
      { id = 45, rect = { 160, 320, 32, 32 } },
      { id = 46, rect = { 192, 256, 32, 32 } },
      { id = 47, rect = { 192, 320, 32, 32 } },
      { id = 48, rect = { 224, 256, 32, 32 } },
      { id = 49, rect = { 224, 320, 32, 32 } },
      { id = 50, rect = { 256, 256, 32, 32 } },
      { id = 51, rect = { 256, 320, 32, 32 } },
      { id = 52, rect = { 288, 256, 32, 32 } },
      { id = 53, rect = { 288, 320, 32, 32 } },
      { id = 54, rect = { 320, 256, 80, 32 } },
      { id = 55, rect = { 320, 320, 80, 32 } },
      { id = 56, rect = { 400, 256, 80, 32 } },
      { id = 57, rect = { 400, 320, 80, 32 } },
      { id = 58, rect = { 0, 288, 48, 32 } },
      { id = 59, rect = { 0, 352, 48, 32 } },
      { id = 60, rect = { 48, 288, 48, 32 } },
      { id = 61, rect = { 48, 352, 48, 32 } },
      { id = 62, rect = { 96, 288, 64, 32 } },
      { id = 63, rect = { 96, 352, 64, 32 } },
      { id = 64, rect = { 160, 288, 64, 32 } },
      { id = 65, rect = { 160, 352, 64, 32 } },
      { id = 66, rect = { 224, 288, 112, 32 } },
      { id = 67, rect = { 224, 352, 112, 32 } },
      { id = 68, rect = { 336, 288, 144, 32 } },
      { id = 69, rect = { 336, 352, 144, 32 } },
      { id = 70, rect = { 0, 384, 80, 32 } },
      { id = 71, rect = { 0, 448, 80, 32 } },
      { id = 72, rect = { 80, 384, 96, 32 } },
      { id = 73, rect = { 80, 448, 96, 32 } },
      { id = 74, rect = { 176, 384, 80, 32 } },
      { id = 75, rect = { 176, 448, 80, 32 } },
      { id = 76, rect = { 256, 384, 80, 32 } },
      { id = 77, rect = { 256, 448, 80, 32 } },
      { id = 78, rect = { 336, 384, 48, 32 } },
      { id = 79, rect = { 336, 448, 48, 32 } },
      { id = 80, rect = { 384, 384, 128, 32 } },
      { id = 81, rect = { 384, 448, 128, 32 } },
      { id = 82, rect = { 0, 416, 96, 32 } },
      { id = 83, rect = { 0, 480, 96, 32 } },
      { id = 84, rect = { 96, 416, 80, 32 } },
      { id = 85, rect = { 96, 480, 80, 32 } },
      { id = 86, rect = { 176, 416, 64, 32 } },
      { id = 87, rect = { 176, 480, 64, 32 } },
      { id = 88, rect = { 240, 416, 80, 32 } },
      { id = 89, rect = { 240, 480, 80, 32 } },
      { id = 90, rect = { 320, 416, 80, 32 } },
      { id = 91, rect = { 320, 480, 80, 32 } },
      { id = 92, rect = { 400, 416, 64, 32 } },
      { id = 93, rect = { 400, 480, 64, 32 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 332 },
      { index = 2, offset = 664 },
      { index = 3, offset = 996 },
      { index = 4, offset = 1328 },
      { index = 5, offset = 1660 },
      { index = 6, offset = 1992 },
      { index = 7, offset = 2324 },
      { index = 8, offset = 2656 },
      { index = 9, offset = 2900 },
      { index = 10, offset = 3144 },
      { index = 11, offset = 3388 },
      { index = 12, offset = 3632 },
      { index = 13, offset = 3876 },
      { index = 14, offset = 4120 },
      { index = 15, offset = 4364 },
      { index = 16, offset = 4608 },
      { index = 17, offset = 4852 },
      { index = 18, offset = 5096 },
      { index = 19, offset = 5340 },
      { index = 20, offset = 5584 },
      { index = 21, offset = 5828 },
      { index = 22, offset = 6072 },
      { index = 23, offset = 6316 },
      { index = 24, offset = 6560 },
      { index = 25, offset = 6804 },
      { index = 26, offset = 7048 },
      { index = 27, offset = 7292 },
      { index = 28, offset = 7536 },
      { index = 29, offset = 7780 },
      { index = 30, offset = 8024 },
      { index = 31, offset = 8268 },
      { index = 32, offset = 8512 },
      { index = 33, offset = 8756 },
      { index = 34, offset = 9000 },
      { index = 35, offset = 9244 },
      { index = 36, offset = 9488 },
      { index = 37, offset = 9732 },
      { index = 38, offset = 9976 },
      { index = 39, offset = 10220 },
      { index = 40, offset = 10464 },
      { index = 41, offset = 10708 },
      { index = 42, offset = 10952 },
      { index = 43, offset = 11196 },
      { index = 44, offset = 11440 },
      { index = 45, offset = 11684 },
      { index = 46, offset = 11928 },
      { index = 47, offset = 12172 },
      { index = 48, offset = 12416 },
      { index = 49, offset = 12660 },
      { index = 50, offset = 12904 },
      { index = 51, offset = 13148 },
      { index = 52, offset = 13392 },
      { index = 53, offset = 13636 },
      { index = 54, offset = 13880 },
      { index = 55, offset = 14124 },
      { index = 56, offset = 14368 },
      { index = 57, offset = 14612 },
      { index = 58, offset = 14856 },
      { index = 59, offset = 15100 },
      { index = 60, offset = 15344 },
      { index = 61, offset = 15588 },
      { index = 62, offset = 15832 },
      { index = 63, offset = 16076 },
      { index = 64, offset = 16320 },
      { index = 65, offset = 16564 },
    },
    labels = {
      [0] = { 2, 5, 3, -1, 1 },
      [1] = { 2, 5, 3, -1, 1 },
      [2] = { 2, 5, 3, -1, 1 },
      [3] = { 2, 5, 3, -1, 1 },
      [4] = { 2, 5, 3, -1, 1 },
      [5] = { 2, 5, 3, -1, 1 },
      [6] = { 2, 5, 3, -1, 1 },
      [7] = { 2, 5, 3, -1, 1 },
      [8] = { -1, 3, 2, 4, 1 },
      [9] = { -1, 3, 2, 4, 1 },
      [10] = { -1, 3, 2, 4, 1 },
      [11] = { -1, 3, 2, 4, 1 },
      [12] = { -1, 3, 2, 4, 1 },
      [13] = { -1, 3, 2, 4, 1 },
      [14] = { -1, 3, 2, 4, 1 },
      [15] = { -1, 3, 2, 4, 1 },
      [16] = { -1, 3, 2, 4, 1 },
      [17] = { -1, 3, 2, 4, 1 },
      [18] = { -1, 3, 2, 4, 1 },
      [19] = { -1, 3, 2, 4, 1 },
      [20] = { -1, 3, 2, 4, 1 },
      [21] = { -1, 3, 2, 4, 1 },
      [22] = { -1, 3, 2, 4, 1 },
      [23] = { -1, 3, 2, 4, 1 },
      [24] = { -1, 3, 2, 4, 1 },
      [25] = { -1, 3, 2, 4, 1 },
      [26] = { -1, 3, 2, 4, 1 },
      [27] = { -1, 3, 2, 4, 1 },
      [28] = { -1, 3, 2, 4, 1 },
      [29] = { -1, 3, 2, 4, 1 },
      [30] = { -1, 3, 2, 4, 1 },
      [31] = { -1, 3, 2, 4, 1 },
      [32] = { -1, 3, 2, 4, 1 },
      [33] = { -1, 4, 3, 2, 1 },
      [34] = { -1, 4, 3, 2, 1 },
      [35] = { -1, 4, 3, 2, 1 },
      [36] = { -1, 4, 3, 2, 1 },
      [37] = { -1, 4, 3, 2, 1 },
      [38] = { -1, 4, 3, 2, 1 },
      [39] = { -1, 4, 3, 2, 1 },
      [40] = { -1, 4, 3, 2, 1 },
      [41] = { -1, 4, 3, 2, 1 },
      [42] = { -1, 4, 3, 2, 1 },
      [43] = { -1, 4, 3, 2, 1 },
      [44] = { -1, 4, 3, 2, 1 },
      [45] = { -1, 4, 3, 2, 1 },
      [46] = { -1, 4, 3, 2, 1 },
      [47] = { -1, 4, 3, 2, 1 },
      [48] = { -1, 4, 3, 2, 1 },
      [49] = { -1, 4, 3, 2, 1 },
      [50] = { -1, 4, 3, 2, 1 },
      [51] = { -1, 4, 3, 2, 1 },
      [52] = { -1, 4, 3, 2, 1 },
      [53] = { -1, 4, 3, 2, 1 },
      [54] = { -1, 4, 3, 2, 1 },
      [55] = { -1, 4, 3, 2, 1 },
      [56] = { -1, 4, 3, 2, 1 },
      [57] = { -1, 4, 3, 2, 1 },
      [58] = { -1, 4, 3, 2, 1 },
      [59] = { -1, 4, 3, 2, 1 },
      [60] = { -1, 4, 3, 2, 1 },
      [61] = { -1, 4, 3, 2, 1 },
      [62] = { -1, 4, 3, 2, 1 },
      [63] = { -1, 4, 3, 2, 1 },
      [64] = { -1, 4, 3, 2, 1 },
      [65] = { -1, 4, 3, 2, 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 5 then goto L5 end
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1128792064, 0)
        ::L2::
        s:LABEL(2, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137180672, 1128792064, 0, 60)
        ::J116::
        s:at(256)
        s:POS_TIME_DECEL(409.6000061035156, 200.0, 0.0, 96)
        s:at(512)
        s:POS_TIME_DECEL(396.79998779296875, 200.0, 0.0, 96)
        s:at(768)
        s:SET_CLOCK(0, 116)
        goto J116
        ::L5::
        s:LABEL(5, 768)
        ::L3::
        s:LABEL(3, 768)
        s:POS_TIME_LINEAR(1145569280, 1128792064, 0, 32)
        s:FADE(0, 32)
        ::Lm1::
        s:LABEL(-1, 800)
        s:at(800)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 800)
        s:POS_TIME_LINEAR(1136656384, 1128792064, 0, 32)
        s:FADE(0, 32)
        s:at(832)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 5 then goto L5 end
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1130627072, 0)
        ::L2::
        s:LABEL(2, 8)
        s:at(8)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1138556928, 1130627072, 0, 60)
        ::J116::
        s:at(264)
        s:POS_TIME_DECEL(439.0, 228.0, 0.0, 96)
        s:at(520)
        s:POS_TIME_DECEL(443.0, 228.0, 0.0, 96)
        s:at(776)
        s:SET_CLOCK(8, 116)
        goto J116
        ::L5::
        s:LABEL(5, 776)
        ::L3::
        s:LABEL(3, 776)
        s:POS_TIME_LINEAR(1145569280, 1130627072, 0, 32)
        s:FADE(0, 32)
        ::Lm1::
        s:LABEL(-1, 808)
        s:at(808)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 808)
        s:POS_TIME_LINEAR(1138720768, 1130627072, 0, 32)
        s:FADE(0, 32)
        s:at(840)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 5 then goto L5 end
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1132462080, 0)
        ::L2::
        s:LABEL(2, 16)
        s:at(16)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137901568, 1132462080, 0, 60)
        ::J116::
        s:at(272)
        s:POS_TIME_DECEL(425.0, 256.0, 0.0, 96)
        s:at(528)
        s:POS_TIME_DECEL(421.0, 256.0, 0.0, 96)
        s:at(784)
        s:SET_CLOCK(16, 116)
        goto J116
        ::L5::
        s:LABEL(5, 784)
        ::L3::
        s:LABEL(3, 784)
        s:POS_TIME_LINEAR(1145569280, 1132462080, 0, 32)
        s:FADE(0, 32)
        ::Lm1::
        s:LABEL(-1, 816)
        s:at(816)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 816)
        s:POS_TIME_LINEAR(1137737728, 1132462080, 0, 32)
        s:FADE(0, 32)
        s:at(848)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 5 then goto L5 end
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1133379584, 0)
        ::L2::
        s:LABEL(2, 24)
        s:at(24)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1139277824, 1133379584, 0, 60)
        ::J116::
        s:at(280)
        s:POS_TIME_DECEL(454.3999938964844, 284.0, 0.0, 96)
        s:at(536)
        s:POS_TIME_DECEL(467.20001220703125, 284.0, 0.0, 96)
        s:at(792)
        s:SET_CLOCK(24, 116)
        goto J116
        ::L5::
        s:LABEL(5, 792)
        ::L3::
        s:LABEL(3, 792)
        s:POS_TIME_LINEAR(1145569280, 1133379584, 0, 32)
        s:FADE(0, 32)
        ::Lm1::
        s:LABEL(-1, 824)
        s:at(824)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 824)
        s:POS_TIME_LINEAR(1139802112, 1133379584, 0, 32)
        s:FADE(0, 32)
        s:at(856)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 5 then goto L5 end
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1134297088, 0)
        ::L2::
        s:LABEL(2, 32)
        s:at(32)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1136656384, 1134297088, 0, 60)
        ::J116::
        s:at(288)
        s:POS_TIME_DECEL(398.3999938964844, 312.0, 0.0, 96)
        s:at(544)
        s:POS_TIME_DECEL(379.20001220703125, 312.0, 0.0, 96)
        s:at(800)
        s:SET_CLOCK(32, 116)
        goto J116
        ::L5::
        s:LABEL(5, 800)
        ::L3::
        s:LABEL(3, 800)
        s:POS_TIME_LINEAR(1145569280, 1134297088, 0, 32)
        s:FADE(0, 32)
        ::Lm1::
        s:LABEL(-1, 832)
        s:at(832)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 832)
        s:POS_TIME_LINEAR(1135869952, 1134297088, 0, 32)
        s:FADE(0, 32)
        s:at(864)
        s:EXIT_HIDE2()
    end,

    -- stream 5
    [5] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 5 then goto L5 end
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(10)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1135214592, 0)
        ::L2::
        s:LABEL(2, 40)
        s:at(40)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137442816, 1135214592, 0, 60)
        ::J116::
        s:at(296)
        s:POS_TIME_DECEL(415.20001220703125, 340.0, 0.0, 96)
        s:at(552)
        s:POS_TIME_DECEL(405.6000061035156, 340.0, 0.0, 96)
        s:at(808)
        s:SET_CLOCK(40, 116)
        goto J116
        ::L5::
        s:LABEL(5, 808)
        ::L3::
        s:LABEL(3, 808)
        s:POS_TIME_LINEAR(1145569280, 1135214592, 0, 32)
        s:FADE(0, 32)
        ::Lm1::
        s:LABEL(-1, 840)
        s:at(840)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 840)
        s:POS_TIME_LINEAR(1137049600, 1135214592, 0, 32)
        s:FADE(0, 32)
        s:at(872)
        s:EXIT_HIDE2()
    end,

    -- stream 6
    [6] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 5 then goto L5 end
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(12)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1136132096, 0)
        ::L2::
        s:LABEL(2, 48)
        s:at(48)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1139015680, 1136132096, 0, 60)
        ::J116::
        s:at(304)
        s:POS_TIME_DECEL(448.79998779296875, 368.0, 0.0, 96)
        s:at(560)
        s:POS_TIME_DECEL(458.3999938964844, 368.0, 0.0, 96)
        s:at(816)
        s:SET_CLOCK(48, 116)
        goto J116
        ::L5::
        s:LABEL(5, 816)
        ::L3::
        s:LABEL(3, 816)
        s:POS_TIME_LINEAR(1145569280, 1136132096, 0, 32)
        s:FADE(0, 32)
        ::Lm1::
        s:LABEL(-1, 848)
        s:at(848)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 848)
        s:POS_TIME_LINEAR(1139408896, 1136132096, 0, 32)
        s:FADE(0, 32)
        s:at(880)
        s:EXIT_HIDE2()
    end,

    -- stream 7
    [7] = function(s)
        local at = s:entry()
        if at == 2 then goto L2 end
        if at == 5 then goto L5 end
        if at == 3 then goto L3 end
        if at == -1 then goto Lm1 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(14)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1137049600, 0)
        ::L2::
        s:LABEL(2, 56)
        s:at(56)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1138229248, 1137049600, 0, 60)
        ::J116::
        s:at(312)
        s:POS_TIME_DECEL(432.0, 396.0, 0.0, 96)
        s:at(568)
        s:POS_TIME_DECEL(432.0, 396.0, 0.0, 96)
        s:at(824)
        s:SET_CLOCK(56, 116)
        goto J116
        ::L5::
        s:LABEL(5, 824)
        ::L3::
        s:LABEL(3, 824)
        s:POS_TIME_LINEAR(1145569280, 1137049600, 0, 32)
        s:FADE(0, 32)
        ::Lm1::
        s:LABEL(-1, 856)
        s:at(856)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 856)
        s:POS_TIME_LINEAR(1138229248, 1137049600, 0, 32)
        s:FADE(0, 32)
        s:at(888)
        s:EXIT_HIDE2()
    end,

    -- stream 8
    [8] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(16)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1119879168, 1124073472, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124073472, 1124073472, 0, 30)
        s:at(30)
        s:STOP()
        ::L2::
        s:LABEL(2, 30)
        ::L4::
        s:LABEL(4, 30)
        s:POS_TIME_LINEAR(1119879168, 1124073472, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,

    -- stream 9
    [9] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(18)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1124073472, 1127219200, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 6)
        s:at(6)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124073472, 1126170624, 0, 30)
        s:at(36)
        s:STOP()
        ::L2::
        s:LABEL(2, 36)
        ::L4::
        s:LABEL(4, 36)
        s:POS_TIME_LINEAR(1124073472, 1127219200, 0, 48)
        s:FADE(0, 32)
        s:at(68)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 100)
        s:at(100)
        s:EXIT_HIDE2()
    end,

    -- stream 10
    [10] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(20)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1126170624, 1128267776, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 12)
        s:at(12)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124073472, 1128267776, 0, 30)
        s:at(42)
        s:STOP()
        ::L2::
        s:LABEL(2, 42)
        ::L4::
        s:LABEL(4, 42)
        s:POS_TIME_LINEAR(1126170624, 1128267776, 0, 48)
        s:FADE(0, 32)
        s:at(74)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 106)
        s:at(106)
        s:EXIT_HIDE2()
    end,

    -- stream 11
    [11] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(22)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1124073472, 1129316352, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 18)
        s:at(18)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124073472, 1130364928, 0, 30)
        s:at(48)
        s:STOP()
        ::L2::
        s:LABEL(2, 48)
        ::L4::
        s:LABEL(4, 48)
        s:POS_TIME_LINEAR(1124073472, 1129316352, 0, 48)
        s:FADE(0, 32)
        s:at(80)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 112)
        s:at(112)
        s:EXIT_HIDE2()
    end,

    -- stream 12
    [12] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(24)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1123024896, 1132462080, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 24)
        s:at(24)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124073472, 1132462080, 0, 30)
        s:at(54)
        s:STOP()
        ::L2::
        s:LABEL(2, 54)
        ::L4::
        s:LABEL(4, 54)
        s:POS_TIME_LINEAR(1123024896, 1132462080, 0, 48)
        s:FADE(0, 32)
        s:at(86)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 118)
        s:at(118)
        s:EXIT_HIDE2()
    end,

    -- stream 13
    [13] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(26)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1124073472, 1132462080, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 30)
        s:at(30)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124073472, 1133510656, 0, 30)
        s:at(60)
        s:STOP()
        ::L2::
        s:LABEL(2, 60)
        ::L4::
        s:LABEL(4, 60)
        s:POS_TIME_LINEAR(1124073472, 1132462080, 0, 48)
        s:FADE(0, 32)
        s:at(92)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 14
    [14] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(28)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1119879168, 1134559232, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 36)
        s:at(36)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124073472, 1134559232, 0, 30)
        s:at(66)
        s:STOP()
        ::L2::
        s:LABEL(2, 66)
        ::L4::
        s:LABEL(4, 66)
        s:POS_TIME_LINEAR(1119879168, 1134559232, 0, 48)
        s:FADE(0, 32)
        s:at(98)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 130)
        s:at(130)
        s:EXIT_HIDE2()
    end,

    -- stream 15
    [15] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(30)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1127219200, 1135607808, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 42)
        s:at(42)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124073472, 1135607808, 0, 30)
        s:at(72)
        s:STOP()
        ::L2::
        s:LABEL(2, 72)
        ::L4::
        s:LABEL(4, 72)
        s:POS_TIME_LINEAR(1127219200, 1135607808, 0, 48)
        s:FADE(0, 32)
        s:at(104)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 136)
        s:at(136)
        s:EXIT_HIDE2()
    end,

    -- stream 16
    [16] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(32)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1124073472, 1137704960, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 48)
        s:at(48)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124073472, 1136656384, 0, 30)
        s:at(78)
        s:STOP()
        ::L2::
        s:LABEL(2, 78)
        ::L4::
        s:LABEL(4, 78)
        s:POS_TIME_LINEAR(1124073472, 1137704960, 0, 48)
        s:FADE(0, 32)
        s:at(110)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 142)
        s:at(142)
        s:EXIT_HIDE2()
    end,

    -- stream 17
    [17] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(36)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1130364928, 1124073472, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 6)
        s:at(6)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1132462080, 1124073472, 0, 30)
        s:at(36)
        s:STOP()
        ::L2::
        s:LABEL(2, 36)
        ::L4::
        s:LABEL(4, 36)
        s:POS_TIME_LINEAR(1130364928, 1124073472, 0, 48)
        s:FADE(0, 32)
        s:at(68)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 100)
        s:at(100)
        s:EXIT_HIDE2()
    end,

    -- stream 18
    [18] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(38)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1134034944, 1119879168, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 12)
        s:at(12)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1134034944, 1124073472, 0, 30)
        s:at(42)
        s:STOP()
        ::L2::
        s:LABEL(2, 42)
        ::L4::
        s:LABEL(4, 42)
        s:POS_TIME_LINEAR(1134034944, 1119879168, 0, 48)
        s:FADE(0, 32)
        s:at(74)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 106)
        s:at(106)
        s:EXIT_HIDE2()
    end,

    -- stream 19
    [19] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(40)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136132096, 1124073472, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 18)
        s:at(18)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1135607808, 1124073472, 0, 30)
        s:at(48)
        s:STOP()
        ::L2::
        s:LABEL(2, 48)
        ::L4::
        s:LABEL(4, 48)
        s:POS_TIME_LINEAR(1136132096, 1124073472, 0, 48)
        s:FADE(0, 32)
        s:at(80)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 112)
        s:at(112)
        s:EXIT_HIDE2()
    end,

    -- stream 20
    [20] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(42)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137180672, 1125122048, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 24)
        s:at(24)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137180672, 1124073472, 0, 30)
        s:at(54)
        s:STOP()
        ::L2::
        s:LABEL(2, 54)
        ::L4::
        s:LABEL(4, 54)
        s:POS_TIME_LINEAR(1137180672, 1125122048, 0, 48)
        s:FADE(0, 32)
        s:at(86)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 118)
        s:at(118)
        s:EXIT_HIDE2()
    end,

    -- stream 21
    [21] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(44)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1139802112, 1124073472, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 30)
        s:at(30)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1138753536, 1124073472, 0, 30)
        s:at(60)
        s:STOP()
        ::L2::
        s:LABEL(2, 60)
        ::L4::
        s:LABEL(4, 60)
        s:POS_TIME_LINEAR(1139802112, 1124073472, 0, 48)
        s:FADE(0, 32)
        s:at(92)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 22
    [22] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(56)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1133510656, 1126170624, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 12)
        s:at(12)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1132462080, 1126170624, 0, 30)
        s:at(42)
        s:STOP()
        ::L2::
        s:LABEL(2, 42)
        ::L4::
        s:LABEL(4, 42)
        s:POS_TIME_LINEAR(1133510656, 1126170624, 0, 48)
        s:FADE(0, 32)
        s:at(74)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 106)
        s:at(106)
        s:EXIT_HIDE2()
    end,

    -- stream 23
    [23] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(54)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136656384, 1128267776, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 18)
        s:at(18)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1136656384, 1126170624, 0, 30)
        s:at(48)
        s:STOP()
        ::L2::
        s:LABEL(2, 48)
        ::L4::
        s:LABEL(4, 48)
        s:POS_TIME_LINEAR(1136656384, 1128267776, 0, 48)
        s:FADE(0, 32)
        s:at(80)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 112)
        s:at(112)
        s:EXIT_HIDE2()
    end,

    -- stream 24
    [24] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(60)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1132462080, 1131413504, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 18)
        s:at(18)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1132462080, 1128267776, 0, 30)
        s:at(48)
        s:STOP()
        ::L2::
        s:LABEL(2, 48)
        ::L4::
        s:LABEL(4, 48)
        s:POS_TIME_LINEAR(1132462080, 1131413504, 0, 48)
        s:FADE(0, 32)
        s:at(80)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 112)
        s:at(112)
        s:EXIT_HIDE2()
    end,

    -- stream 25
    [25] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(62)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1134034944, 1128267776, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 24)
        s:at(24)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1135083520, 1128267776, 0, 30)
        s:at(54)
        s:STOP()
        ::L2::
        s:LABEL(2, 54)
        ::L4::
        s:LABEL(4, 54)
        s:POS_TIME_LINEAR(1134034944, 1128267776, 0, 48)
        s:FADE(0, 32)
        s:at(86)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 118)
        s:at(118)
        s:EXIT_HIDE2()
    end,

    -- stream 26
    [26] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(64)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137704960, 1126170624, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 30)
        s:at(30)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1128267776, 0, 30)
        s:at(60)
        s:STOP()
        ::L2::
        s:LABEL(2, 60)
        ::L4::
        s:LABEL(4, 60)
        s:POS_TIME_LINEAR(1137704960, 1126170624, 0, 48)
        s:FADE(0, 32)
        s:at(92)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 27
    [27] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(60)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1132986368, 1130364928, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 24)
        s:at(24)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1132462080, 1130364928, 0, 30)
        s:at(54)
        s:STOP()
        ::L2::
        s:LABEL(2, 54)
        ::L4::
        s:LABEL(4, 54)
        s:POS_TIME_LINEAR(1132986368, 1130364928, 0, 48)
        s:FADE(0, 32)
        s:at(86)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 118)
        s:at(118)
        s:EXIT_HIDE2()
    end,

    -- stream 28
    [28] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(58)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1134559232, 1132462080, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 30)
        s:at(30)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1134559232, 1130364928, 0, 30)
        s:at(60)
        s:STOP()
        ::L2::
        s:LABEL(2, 60)
        ::L4::
        s:LABEL(4, 60)
        s:POS_TIME_LINEAR(1134559232, 1132462080, 0, 48)
        s:FADE(0, 32)
        s:at(92)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 29
    [29] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(68)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1132462080, 1129316352, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 30)
        s:at(30)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1132462080, 1132462080, 0, 30)
        s:at(60)
        s:STOP()
        ::L2::
        s:LABEL(2, 60)
        ::L4::
        s:LABEL(4, 60)
        s:POS_TIME_LINEAR(1132462080, 1129316352, 0, 48)
        s:FADE(0, 32)
        s:at(92)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 30
    [30] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(66)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138098176, 1132462080, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 36)
        s:at(36)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137180672, 1132462080, 0, 30)
        s:at(66)
        s:STOP()
        ::L2::
        s:LABEL(2, 66)
        ::L4::
        s:LABEL(4, 66)
        s:POS_TIME_LINEAR(1138098176, 1132462080, 0, 48)
        s:FADE(0, 32)
        s:at(98)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 130)
        s:at(130)
        s:EXIT_HIDE2()
    end,

    -- stream 31
    [31] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(60)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1130364928, 1133510656, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 36)
        s:at(36)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1132462080, 1133510656, 0, 30)
        s:at(66)
        s:STOP()
        ::L2::
        s:LABEL(2, 66)
        ::L4::
        s:LABEL(4, 66)
        s:POS_TIME_LINEAR(1130364928, 1133510656, 0, 48)
        s:FADE(0, 32)
        s:at(98)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 130)
        s:at(130)
        s:EXIT_HIDE2()
    end,

    -- stream 32
    [32] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 4 then goto L4 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(58)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1135083520, 1134559232, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L3::
        s:LABEL(3, 42)
        s:at(42)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1135083520, 1133510656, 0, 30)
        s:at(72)
        s:STOP()
        ::L2::
        s:LABEL(2, 72)
        ::L4::
        s:LABEL(4, 72)
        s:POS_TIME_LINEAR(1135083520, 1134559232, 0, 48)
        s:FADE(0, 32)
        s:at(104)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 136)
        s:at(136)
        s:EXIT_HIDE2()
    end,

    -- stream 33
    [33] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(30)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1130102784, 1117782016, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1132199936, 1117782016, 0, 30)
        s:at(30)
        s:STOP()
        ::L3::
        s:LABEL(3, 30)
        ::L2::
        s:LABEL(2, 30)
        s:POS_TIME_LINEAR(1130102784, 1117782016, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,

    -- stream 34
    [34] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(70)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1129054208, 1116995584, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1121189888, 0, 30)
        s:at(30)
        s:STOP()
        ::L3::
        s:LABEL(3, 30)
        ::L2::
        s:LABEL(2, 30)
        s:POS_TIME_LINEAR(1129054208, 1116995584, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,

    -- stream 35
    [35] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(72)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1126957056, 1124335616, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 6)
        s:at(6)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1124335616, 0, 30)
        s:at(36)
        s:STOP()
        ::L3::
        s:LABEL(3, 36)
        ::L2::
        s:LABEL(2, 36)
        s:POS_TIME_LINEAR(1126957056, 1124335616, 0, 48)
        s:FADE(0, 32)
        s:at(68)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 100)
        s:at(100)
        s:EXIT_HIDE2()
    end,

    -- stream 36
    [36] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(74)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1131151360, 1126039552, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 12)
        s:at(12)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1126039552, 0, 30)
        s:at(42)
        s:STOP()
        ::L3::
        s:LABEL(3, 42)
        ::L2::
        s:LABEL(2, 42)
        s:POS_TIME_LINEAR(1131151360, 1126039552, 0, 48)
        s:FADE(0, 32)
        s:at(74)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 106)
        s:at(106)
        s:EXIT_HIDE2()
    end,

    -- stream 37
    [37] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(76)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1129054208, 1128792064, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 18)
        s:at(18)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1127743488, 0, 30)
        s:at(48)
        s:STOP()
        ::L3::
        s:LABEL(3, 48)
        ::L2::
        s:LABEL(2, 48)
        s:POS_TIME_LINEAR(1129054208, 1128792064, 0, 48)
        s:FADE(0, 32)
        s:at(80)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 112)
        s:at(112)
        s:EXIT_HIDE2()
    end,

    -- stream 38
    [38] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(82)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1129054208, 1130496000, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 24)
        s:at(24)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1129447424, 0, 30)
        s:at(54)
        s:STOP()
        ::L3::
        s:LABEL(3, 54)
        ::L2::
        s:LABEL(2, 54)
        s:POS_TIME_LINEAR(1129054208, 1130496000, 0, 48)
        s:FADE(0, 32)
        s:at(86)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 118)
        s:at(118)
        s:EXIT_HIDE2()
    end,

    -- stream 39
    [39] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(78)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1129578496, 1131151360, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 30)
        s:at(30)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1131151360, 0, 30)
        s:at(60)
        s:STOP()
        ::L3::
        s:LABEL(3, 60)
        ::L2::
        s:LABEL(2, 60)
        s:POS_TIME_LINEAR(1129578496, 1131151360, 0, 48)
        s:FADE(0, 32)
        s:at(92)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 40
    [40] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(84)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1129054208, 1129709568, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 36)
        s:at(36)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1132658688, 0, 30)
        s:at(66)
        s:STOP()
        ::L3::
        s:LABEL(3, 66)
        ::L2::
        s:LABEL(2, 66)
        s:POS_TIME_LINEAR(1129054208, 1129709568, 0, 48)
        s:FADE(0, 32)
        s:at(98)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 130)
        s:at(130)
        s:EXIT_HIDE2()
    end,

    -- stream 41
    [41] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(86)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1132199936, 1133510656, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 42)
        s:at(42)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1133510656, 0, 30)
        s:at(72)
        s:STOP()
        ::L3::
        s:LABEL(3, 72)
        ::L2::
        s:LABEL(2, 72)
        s:POS_TIME_LINEAR(1132199936, 1133510656, 0, 48)
        s:FADE(0, 32)
        s:at(104)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 136)
        s:at(136)
        s:EXIT_HIDE2()
    end,

    -- stream 42
    [42] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(88)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1126957056, 1134362624, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 48)
        s:at(48)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1134362624, 0, 30)
        s:at(78)
        s:STOP()
        ::L3::
        s:LABEL(3, 78)
        ::L2::
        s:LABEL(2, 78)
        s:POS_TIME_LINEAR(1126957056, 1134362624, 0, 48)
        s:FADE(0, 32)
        s:at(110)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 142)
        s:at(142)
        s:EXIT_HIDE2()
    end,

    -- stream 43
    [43] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(80)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1129054208, 1133641728, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 54)
        s:at(54)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1135214592, 0, 30)
        s:at(84)
        s:STOP()
        ::L3::
        s:LABEL(3, 84)
        ::L2::
        s:LABEL(2, 84)
        s:POS_TIME_LINEAR(1129054208, 1133641728, 0, 48)
        s:FADE(0, 32)
        s:at(116)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 148)
        s:at(148)
        s:EXIT_HIDE2()
    end,

    -- stream 44
    [44] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(90)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1129054208, 1137639424, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 60)
        s:at(60)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1136066560, 0, 30)
        s:at(90)
        s:STOP()
        ::L3::
        s:LABEL(3, 90)
        ::L2::
        s:LABEL(2, 90)
        s:POS_TIME_LINEAR(1129054208, 1137639424, 0, 48)
        s:FADE(0, 32)
        s:at(122)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 154)
        s:at(154)
        s:EXIT_HIDE2()
    end,

    -- stream 45
    [45] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(92)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1125908480, 1136918528, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 66)
        s:at(66)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129054208, 1136918528, 0, 30)
        s:at(96)
        s:STOP()
        ::L3::
        s:LABEL(3, 96)
        ::L2::
        s:LABEL(2, 96)
        s:POS_TIME_LINEAR(1125908480, 1136918528, 0, 48)
        s:FADE(0, 32)
        s:at(128)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 160)
        s:at(160)
        s:EXIT_HIDE2()
    end,

    -- stream 46
    [46] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136001024, 1121189888, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 6)
        s:at(6)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1121189888, 0, 30)
        s:at(36)
        s:STOP()
        ::L3::
        s:LABEL(3, 36)
        ::L2::
        s:LABEL(2, 36)
        s:POS_TIME_LINEAR(1136001024, 1121189888, 0, 48)
        s:FADE(0, 32)
        s:at(68)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 100)
        s:at(100)
        s:EXIT_HIDE2()
    end,

    -- stream 47
    [47] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137704960, 1124728832, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 12)
        s:at(12)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1121189888, 0, 30)
        s:at(42)
        s:STOP()
        ::L3::
        s:LABEL(3, 42)
        ::L2::
        s:LABEL(2, 42)
        s:POS_TIME_LINEAR(1137704960, 1124728832, 0, 48)
        s:FADE(0, 32)
        s:at(74)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 106)
        s:at(106)
        s:EXIT_HIDE2()
    end,

    -- stream 48
    [48] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138098176, 1124335616, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 12)
        s:at(12)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1124335616, 0, 30)
        s:at(42)
        s:STOP()
        ::L3::
        s:LABEL(3, 42)
        ::L2::
        s:LABEL(2, 42)
        s:POS_TIME_LINEAR(1138098176, 1124335616, 0, 48)
        s:FADE(0, 32)
        s:at(74)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 106)
        s:at(106)
        s:EXIT_HIDE2()
    end,

    -- stream 49
    [49] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138229248, 1124335616, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 18)
        s:at(18)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1124335616, 0, 30)
        s:at(48)
        s:STOP()
        ::L3::
        s:LABEL(3, 48)
        ::L2::
        s:LABEL(2, 48)
        s:POS_TIME_LINEAR(1138229248, 1124335616, 0, 48)
        s:FADE(0, 32)
        s:at(80)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 112)
        s:at(112)
        s:EXIT_HIDE2()
    end,

    -- stream 50
    [50] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137049600, 1123811328, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 18)
        s:at(18)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1126039552, 0, 30)
        s:at(48)
        s:STOP()
        ::L3::
        s:LABEL(3, 48)
        ::L2::
        s:LABEL(2, 48)
        s:POS_TIME_LINEAR(1137049600, 1123811328, 0, 48)
        s:FADE(0, 32)
        s:at(80)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 112)
        s:at(112)
        s:EXIT_HIDE2()
    end,

    -- stream 51
    [51] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137704960, 1126563840, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 24)
        s:at(24)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1126039552, 0, 30)
        s:at(54)
        s:STOP()
        ::L3::
        s:LABEL(3, 54)
        ::L2::
        s:LABEL(2, 54)
        s:POS_TIME_LINEAR(1137704960, 1126563840, 0, 48)
        s:FADE(0, 32)
        s:at(86)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 118)
        s:at(118)
        s:EXIT_HIDE2()
    end,

    -- stream 52
    [52] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1135476736, 1127743488, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 24)
        s:at(24)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1127743488, 0, 30)
        s:at(54)
        s:STOP()
        ::L3::
        s:LABEL(3, 54)
        ::L2::
        s:LABEL(2, 54)
        s:POS_TIME_LINEAR(1135476736, 1127743488, 0, 48)
        s:FADE(0, 32)
        s:at(86)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 118)
        s:at(118)
        s:EXIT_HIDE2()
    end,

    -- stream 53
    [53] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137442816, 1127743488, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 30)
        s:at(30)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1127743488, 0, 30)
        s:at(60)
        s:STOP()
        ::L3::
        s:LABEL(3, 60)
        ::L2::
        s:LABEL(2, 60)
        s:POS_TIME_LINEAR(1137442816, 1127743488, 0, 48)
        s:FADE(0, 32)
        s:at(92)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 54
    [54] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137049600, 1131020288, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 30)
        s:at(30)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1129447424, 0, 30)
        s:at(60)
        s:STOP()
        ::L3::
        s:LABEL(3, 60)
        ::L2::
        s:LABEL(2, 60)
        s:POS_TIME_LINEAR(1137049600, 1131020288, 0, 48)
        s:FADE(0, 32)
        s:at(92)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 55
    [55] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138491392, 1129447424, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 36)
        s:at(36)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1129447424, 0, 30)
        s:at(66)
        s:STOP()
        ::L3::
        s:LABEL(3, 66)
        ::L2::
        s:LABEL(2, 66)
        s:POS_TIME_LINEAR(1138491392, 1129447424, 0, 48)
        s:FADE(0, 32)
        s:at(98)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 130)
        s:at(130)
        s:EXIT_HIDE2()
    end,

    -- stream 56
    [56] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137049600, 1132593152, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 36)
        s:at(36)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1131151360, 0, 30)
        s:at(66)
        s:STOP()
        ::L3::
        s:LABEL(3, 66)
        ::L2::
        s:LABEL(2, 66)
        s:POS_TIME_LINEAR(1137049600, 1132593152, 0, 48)
        s:FADE(0, 32)
        s:at(98)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 130)
        s:at(130)
        s:EXIT_HIDE2()
    end,

    -- stream 57
    [57] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138491392, 1131151360, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 42)
        s:at(42)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1131151360, 0, 30)
        s:at(72)
        s:STOP()
        ::L3::
        s:LABEL(3, 72)
        ::L2::
        s:LABEL(2, 72)
        s:POS_TIME_LINEAR(1138491392, 1131151360, 0, 48)
        s:FADE(0, 32)
        s:at(104)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 136)
        s:at(136)
        s:EXIT_HIDE2()
    end,

    -- stream 58
    [58] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137049600, 1134231552, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 42)
        s:at(42)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1132658688, 0, 30)
        s:at(72)
        s:STOP()
        ::L3::
        s:LABEL(3, 72)
        ::L2::
        s:LABEL(2, 72)
        s:POS_TIME_LINEAR(1137049600, 1134231552, 0, 48)
        s:FADE(0, 32)
        s:at(104)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 136)
        s:at(136)
        s:EXIT_HIDE2()
    end,

    -- stream 59
    [59] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136656384, 1132658688, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 48)
        s:at(48)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1132658688, 0, 30)
        s:at(78)
        s:STOP()
        ::L3::
        s:LABEL(3, 78)
        ::L2::
        s:LABEL(2, 78)
        s:POS_TIME_LINEAR(1136656384, 1132658688, 0, 48)
        s:FADE(0, 32)
        s:at(110)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 142)
        s:at(142)
        s:EXIT_HIDE2()
    end,

    -- stream 60
    [60] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137836032, 1133510656, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 48)
        s:at(48)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1133510656, 0, 30)
        s:at(78)
        s:STOP()
        ::L3::
        s:LABEL(3, 78)
        ::L2::
        s:LABEL(2, 78)
        s:POS_TIME_LINEAR(1137836032, 1133510656, 0, 48)
        s:FADE(0, 32)
        s:at(110)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 142)
        s:at(142)
        s:EXIT_HIDE2()
    end,

    -- stream 61
    [61] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137704960, 1132462080, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 54)
        s:at(54)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1133510656, 0, 30)
        s:at(84)
        s:STOP()
        ::L3::
        s:LABEL(3, 84)
        ::L2::
        s:LABEL(2, 84)
        s:POS_TIME_LINEAR(1137704960, 1132462080, 0, 48)
        s:FADE(0, 32)
        s:at(116)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 148)
        s:at(148)
        s:EXIT_HIDE2()
    end,

    -- stream 62
    [62] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137049600, 1135411200, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 54)
        s:at(54)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1134362624, 0, 30)
        s:at(84)
        s:STOP()
        ::L3::
        s:LABEL(3, 84)
        ::L2::
        s:LABEL(2, 84)
        s:POS_TIME_LINEAR(1137049600, 1135411200, 0, 48)
        s:FADE(0, 32)
        s:at(116)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 148)
        s:at(148)
        s:EXIT_HIDE2()
    end,

    -- stream 63
    [63] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(34)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136918528, 1134362624, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 60)
        s:at(60)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137704960, 1134362624, 0, 30)
        s:at(90)
        s:STOP()
        ::L3::
        s:LABEL(3, 90)
        ::L2::
        s:LABEL(2, 90)
        s:POS_TIME_LINEAR(1136918528, 1134362624, 0, 48)
        s:FADE(0, 32)
        s:at(122)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 154)
        s:at(154)
        s:EXIT_HIDE2()
    end,

    -- stream 64
    [64] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(60)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1138622464, 1135214592, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 60)
        s:at(60)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1137049600, 1135214592, 0, 30)
        s:at(90)
        s:STOP()
        ::L3::
        s:LABEL(3, 90)
        ::L2::
        s:LABEL(2, 90)
        s:POS_TIME_LINEAR(1138622464, 1135214592, 0, 48)
        s:FADE(0, 32)
        s:at(122)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 154)
        s:at(154)
        s:EXIT_HIDE2()
    end,

    -- stream 65
    [65] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 4 then goto L4 end
        if at == 3 then goto L3 end
        if at == 2 then goto L2 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(58)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1139671040, 1136263168, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L4::
        s:LABEL(4, 66)
        s:at(66)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1139671040, 1135214592, 0, 30)
        s:at(96)
        s:STOP()
        ::L3::
        s:LABEL(3, 96)
        ::L2::
        s:LABEL(2, 96)
        s:POS_TIME_LINEAR(1139671040, 1136263168, 0, 48)
        s:FADE(0, 32)
        s:at(128)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 160)
        s:at(160)
        s:EXIT_HIDE2()
    end,
  },
  [2] = {
    texture = "texture_2.png",
    width = 512, height = 512, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 96 } },
      { id = 1, rect = { 256, 0, 256, 96 } },
      { id = 2, rect = { 0, 96, 256, 96 } },
      { id = 3, rect = { 256, 96, 256, 96 } },
      { id = 4, rect = { 0, 192, 256, 96 } },
      { id = 5, rect = { 256, 192, 256, 96 } },
      { id = 6, rect = { 0, 288, 256, 96 } },
      { id = 7, rect = { 256, 288, 256, 96 } },
      { id = 8, rect = { 0, 384, 256, 96 } },
      { id = 9, rect = { 256, 384, 256, 96 } },
      { id = 10, rect = { 0, 480, 256, 32 } },
    },
    scripts = {
      { index = -28, offset = 0 },
      { index = -27, offset = 340 },
      { index = -26, offset = 680 },
      { index = -25, offset = 1020 },
    },
    labels = {
      [0] = { -1, 7, 6, 8, 10, 9, 18, 1 },
      [1] = { -1, 7, 6, 8, 10, 9, 18, 1 },
      [2] = { -1, 7, 6, 8, 10, 9, 18, 1 },
      [3] = { -1, 7, 6, 8, 10, 9, 18, 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 7 then goto L7 end
        if at == 6 then goto L6 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 9 then goto L9 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1123549184, 1119879168, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L7::
        s:LABEL(7, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1132199936, 1119879168, 0, 30)
        s:at(30)
        s:STOP()
        ::L6::
        s:LABEL(6, 30)
        ::L8::
        s:LABEL(8, 30)
        ::L10::
        s:LABEL(10, 30)
        s:POS_TIME_LINEAR(1123549184, 1119879168, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 62)
        s:POS_TIME_DECEL(16.0, 384.0, 0.0, 20)
        s:at(92)
        s:STOP()
        ::L18::
        s:LABEL(18, 92)
        s:SET_TRANSLATION(1098907648, 1136656384, 0)
        s:at(122)
        s:STOP()
        ::L1::
        s:LABEL(1, 154)
        s:at(154)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 7 then goto L7 end
        if at == 6 then goto L6 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 9 then goto L9 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1126432768, 1127743488, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L7::
        s:LABEL(7, 10)
        s:at(10)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129578496, 1127743488, 0, 30)
        s:at(40)
        s:STOP()
        ::L6::
        s:LABEL(6, 40)
        ::L8::
        s:LABEL(8, 40)
        ::L10::
        s:LABEL(10, 40)
        s:POS_TIME_LINEAR(1126432768, 1127743488, 0, 48)
        s:FADE(0, 32)
        s:at(72)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 72)
        s:POS_TIME_DECEL(16.0, 384.0, 0.0, 20)
        s:at(102)
        s:STOP()
        ::L18::
        s:LABEL(18, 102)
        s:SET_TRANSLATION(1098907648, 1136656384, 0)
        s:at(132)
        s:STOP()
        ::L1::
        s:LABEL(1, 164)
        s:at(164)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 7 then goto L7 end
        if at == 6 then goto L6 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 9 then goto L9 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1130102784, 1132986368, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L7::
        s:LABEL(7, 20)
        s:at(20)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1126957056, 1132986368, 0, 30)
        s:at(50)
        s:STOP()
        ::L6::
        s:LABEL(6, 50)
        ::L8::
        s:LABEL(8, 50)
        ::L10::
        s:LABEL(10, 50)
        s:POS_TIME_LINEAR(1130102784, 1132986368, 0, 48)
        s:FADE(0, 32)
        s:at(82)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 82)
        s:POS_TIME_DECEL(16.0, 384.0, 0.0, 20)
        s:at(112)
        s:STOP()
        ::L18::
        s:LABEL(18, 112)
        s:SET_TRANSLATION(1098907648, 1136656384, 0)
        s:at(142)
        s:STOP()
        ::L1::
        s:LABEL(1, 174)
        s:at(174)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 7 then goto L7 end
        if at == 6 then goto L6 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 9 then goto L9 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1132593152, 1135869952, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L7::
        s:LABEL(7, 30)
        s:at(30)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1124335616, 1135869952, 0, 30)
        s:at(60)
        s:STOP()
        ::L6::
        s:LABEL(6, 60)
        ::L8::
        s:LABEL(8, 60)
        ::L10::
        s:LABEL(10, 60)
        s:POS_TIME_LINEAR(1132593152, 1135869952, 0, 48)
        s:FADE(0, 32)
        s:at(92)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 92)
        s:POS_TIME_DECEL(16.0, 384.0, 0.0, 20)
        s:at(122)
        s:STOP()
        ::L18::
        s:LABEL(18, 122)
        s:SET_TRANSLATION(1098907648, 1136656384, 0)
        s:at(152)
        s:STOP()
        ::L1::
        s:LABEL(1, 184)
        s:at(184)
        s:EXIT_HIDE2()
    end,
  },
  [3] = {
    texture = "texture_3.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 480 } },
      { id = 1, rect = { 256, 0, 256, 128 } },
      { id = 2, rect = { 256, 128, 256, 128 } },
      { id = 3, rect = { 256, 256, 256, 128 } },
      { id = 4, rect = { 256, 384, 256, 128 } },
    },
    scripts = {
      { index = -35, offset = 0 },
      { index = -34, offset = 332 },
      { index = -33, offset = 576 },
    },
    labels = {
      [0] = { -1, 9, 7, 12, 22, 8, 10, 18, 1 },
      [1] = { -1, 10, 18, 8, 1 },
      [2] = { -1, 10, 18, 8, 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 9 then goto L9 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1141374976, 1131413504, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1138753536, 1131413504, 0, 30)
        s:at(30)
        s:STOP()
        ::L7::
        s:LABEL(7, 30)
        ::L12::
        s:LABEL(12, 30)
        ::L22::
        s:LABEL(22, 30)
        ::L8::
        s:LABEL(8, 30)
        s:FADE(0, 32)
        s:POS_TIME_LINEAR(1135607808, 1131413504, 0, 48)
        s:at(62)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 62)
        ::L18::
        s:LABEL(18, 62)
        s:FADE(128, 32)
        s:POS_TIME_LINEAR(1138753536, 1131413504, 0, 38)
        s:at(94)
        s:STOP()
        ::L1::
        s:LABEL(1, 126)
        s:at(126)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 8 then goto L8 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136132096, 1132462080, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1136132096, 1128267776, 0, 30)
        s:at(30)
        s:STOP()
        ::L18::
        s:LABEL(18, 30)
        ::L8::
        s:LABEL(8, 30)
        s:POS_TIME_LINEAR(1136132096, 1132462080, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 8 then goto L8 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136132096, 1132986368, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1136132096, 1135083520, 0, 30)
        s:at(30)
        s:STOP()
        ::L18::
        s:LABEL(18, 30)
        ::L8::
        s:LABEL(8, 30)
        s:POS_TIME_LINEAR(1136132096, 1132986368, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,
  },
  [4] = {
    texture = "texture_4.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 480 } },
      { id = 1, rect = { 256, 0, 256, 128 } },
      { id = 2, rect = { 256, 128, 256, 128 } },
      { id = 3, rect = { 256, 256, 256, 128 } },
      { id = 4, rect = { 256, 384, 256, 128 } },
    },
    scripts = {
      { index = -37, offset = 0 },
      { index = -36, offset = 332 },
      { index = -35, offset = 576 },
    },
    labels = {
      [0] = { -1, 9, 7, 12, 22, 8, 10, 18, 1 },
      [1] = { -1, 10, 18, 8, 1 },
      [2] = { -1, 10, 18, 8, 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 9 then goto L9 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1141374976, 1131413504, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1138753536, 1131413504, 0, 30)
        s:at(30)
        s:STOP()
        ::L7::
        s:LABEL(7, 30)
        ::L12::
        s:LABEL(12, 30)
        ::L22::
        s:LABEL(22, 30)
        ::L8::
        s:LABEL(8, 30)
        s:FADE(0, 32)
        s:POS_TIME_LINEAR(1135607808, 1131413504, 0, 48)
        s:at(62)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 62)
        ::L18::
        s:LABEL(18, 62)
        s:FADE(128, 32)
        s:POS_TIME_LINEAR(1138753536, 1131413504, 0, 38)
        s:at(94)
        s:STOP()
        ::L1::
        s:LABEL(1, 126)
        s:at(126)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 8 then goto L8 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136132096, 1132462080, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1136132096, 1128267776, 0, 30)
        s:at(30)
        s:STOP()
        ::L18::
        s:LABEL(18, 30)
        ::L8::
        s:LABEL(8, 30)
        s:POS_TIME_LINEAR(1136132096, 1132462080, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 8 then goto L8 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136132096, 1132986368, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1136132096, 1135083520, 0, 30)
        s:at(30)
        s:STOP()
        ::L18::
        s:LABEL(18, 30)
        ::L8::
        s:LABEL(8, 30)
        s:POS_TIME_LINEAR(1136132096, 1132986368, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,
  },
  [5] = {
    texture = "texture_5.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 480 } },
      { id = 1, rect = { 256, 0, 256, 128 } },
      { id = 2, rect = { 256, 128, 256, 128 } },
      { id = 3, rect = { 256, 256, 256, 128 } },
      { id = 4, rect = { 256, 384, 256, 128 } },
    },
    scripts = {
      { index = -39, offset = 0 },
      { index = -38, offset = 332 },
      { index = -37, offset = 576 },
    },
    labels = {
      [0] = { -1, 9, 7, 12, 22, 8, 10, 18, 1 },
      [1] = { -1, 10, 18, 8, 1 },
      [2] = { -1, 10, 18, 8, 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 9 then goto L9 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1141374976, 1131413504, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1138753536, 1131413504, 0, 30)
        s:at(30)
        s:STOP()
        ::L7::
        s:LABEL(7, 30)
        ::L12::
        s:LABEL(12, 30)
        ::L22::
        s:LABEL(22, 30)
        ::L8::
        s:LABEL(8, 30)
        s:FADE(0, 32)
        s:POS_TIME_LINEAR(1135607808, 1131413504, 0, 48)
        s:at(62)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 62)
        ::L18::
        s:LABEL(18, 62)
        s:FADE(128, 32)
        s:POS_TIME_LINEAR(1138753536, 1131413504, 0, 38)
        s:at(94)
        s:STOP()
        ::L1::
        s:LABEL(1, 126)
        s:at(126)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 8 then goto L8 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136132096, 1132462080, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1136132096, 1128267776, 0, 30)
        s:at(30)
        s:STOP()
        ::L18::
        s:LABEL(18, 30)
        ::L8::
        s:LABEL(8, 30)
        s:POS_TIME_LINEAR(1136132096, 1132462080, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 8 then goto L8 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1136132096, 1132986368, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1136132096, 1135083520, 0, 30)
        s:at(30)
        s:STOP()
        ::L18::
        s:LABEL(18, 30)
        ::L8::
        s:LABEL(8, 30)
        s:POS_TIME_LINEAR(1136132096, 1132986368, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,
  },
  [6] = {
    texture = "texture_6.png",
    width = 512, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 64 } },
      { id = 1, rect = { 0, 64, 256, 64 } },
      { id = 2, rect = { 0, 128, 256, 64 } },
      { id = 3, rect = { 256, 0, 256, 64 } },
      { id = 4, rect = { 256, 64, 256, 64 } },
      { id = 5, rect = { 256, 128, 256, 64 } },
    },
    scripts = {
      { index = -41, offset = 0 },
      { index = -40, offset = 320 },
      { index = -39, offset = 640 },
      { index = -38, offset = 960 },
      { index = -37, offset = 1280 },
      { index = -36, offset = 1600 },
    },
    labels = {
      [0] = { -1, 9, 7, 12, 22, 8, 10, 1 },
      [1] = { -1, 9, 7, 12, 22, 8, 10, 1 },
      [2] = { -1, 9, 7, 12, 22, 8, 10, 1 },
      [3] = { -1, 9, 7, 12, 22, 8, 10, 1 },
      [4] = { -1, 9, 7, 12, 22, 8, 10, 1 },
      [5] = { -1, 9, 7, 12, 22, 8, 10, 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 9 then goto L9 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137704960, 1134559232, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1139802112, 1134559232, 0, 30)
        s:at(30)
        s:STOP()
        ::L7::
        s:LABEL(7, 30)
        ::L12::
        s:LABEL(12, 30)
        ::L22::
        s:LABEL(22, 30)
        ::L8::
        s:LABEL(8, 30)
        s:FADE(0, 32)
        s:POS_TIME_LINEAR(1141374976, 1134559232, 0, 48)
        s:at(62)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 62)
        s:FADE(128, 32)
        s:POS_TIME_LINEAR(1139802112, 1134559232, 0, 38)
        s:at(94)
        s:STOP()
        ::L1::
        s:LABEL(1, 126)
        s:at(126)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 9 then goto L9 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137704960, 1134559232, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1139802112, 1134559232, 0, 30)
        s:at(30)
        s:STOP()
        ::L7::
        s:LABEL(7, 30)
        ::L12::
        s:LABEL(12, 30)
        ::L22::
        s:LABEL(22, 30)
        ::L8::
        s:LABEL(8, 30)
        s:FADE(0, 32)
        s:POS_TIME_LINEAR(1141374976, 1134559232, 0, 48)
        s:at(62)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 62)
        s:FADE(128, 32)
        s:POS_TIME_LINEAR(1139802112, 1134559232, 0, 38)
        s:at(94)
        s:STOP()
        ::L1::
        s:LABEL(1, 126)
        s:at(126)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 9 then goto L9 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1137704960, 1134559232, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1139802112, 1134559232, 0, 30)
        s:at(30)
        s:STOP()
        ::L7::
        s:LABEL(7, 30)
        ::L12::
        s:LABEL(12, 30)
        ::L22::
        s:LABEL(22, 30)
        ::L8::
        s:LABEL(8, 30)
        s:FADE(0, 32)
        s:POS_TIME_LINEAR(1141374976, 1134559232, 0, 48)
        s:at(62)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 62)
        s:FADE(128, 32)
        s:POS_TIME_LINEAR(1139802112, 1134559232, 0, 38)
        s:at(94)
        s:STOP()
        ::L1::
        s:LABEL(1, 126)
        s:at(126)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 9 then goto L9 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1141112832, 1134821376, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 10)
        s:at(10)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1141112832, 1136918528, 0, 30)
        s:at(40)
        s:STOP()
        ::L7::
        s:LABEL(7, 40)
        ::L12::
        s:LABEL(12, 40)
        ::L22::
        s:LABEL(22, 40)
        ::L8::
        s:LABEL(8, 40)
        s:FADE(0, 32)
        s:POS_TIME_LINEAR(1141112832, 1139015680, 0, 48)
        s:at(72)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 72)
        s:FADE(128, 32)
        s:POS_TIME_LINEAR(1141112832, 1136918528, 0, 38)
        s:at(104)
        s:STOP()
        ::L1::
        s:LABEL(1, 136)
        s:at(136)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 9 then goto L9 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1141112832, 1134821376, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 10)
        s:at(10)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1141112832, 1136918528, 0, 30)
        s:at(40)
        s:STOP()
        ::L7::
        s:LABEL(7, 40)
        ::L12::
        s:LABEL(12, 40)
        ::L22::
        s:LABEL(22, 40)
        ::L8::
        s:LABEL(8, 40)
        s:FADE(0, 32)
        s:POS_TIME_LINEAR(1141112832, 1139015680, 0, 48)
        s:at(72)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 72)
        s:FADE(128, 32)
        s:POS_TIME_LINEAR(1141112832, 1136918528, 0, 38)
        s:at(104)
        s:STOP()
        ::L1::
        s:LABEL(1, 136)
        s:at(136)
        s:EXIT_HIDE2()
    end,

    -- stream 5
    [5] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 9 then goto L9 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1141112832, 1134821376, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 10)
        s:at(10)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1141112832, 1136918528, 0, 30)
        s:at(40)
        s:STOP()
        ::L7::
        s:LABEL(7, 40)
        ::L12::
        s:LABEL(12, 40)
        ::L22::
        s:LABEL(22, 40)
        ::L8::
        s:LABEL(8, 40)
        s:FADE(0, 32)
        s:POS_TIME_LINEAR(1141112832, 1139015680, 0, 48)
        s:at(72)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 72)
        s:FADE(128, 32)
        s:POS_TIME_LINEAR(1141112832, 1136918528, 0, 38)
        s:at(104)
        s:STOP()
        ::L1::
        s:LABEL(1, 136)
        s:at(136)
        s:EXIT_HIDE2()
    end,
  },
  [7] = {
    texture = "texture_7.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 48 } },
      { id = 1, rect = { 0, 48, 256, 48 } },
      { id = 2, rect = { 0, 96, 256, 48 } },
      { id = 3, rect = { 0, 144, 256, 48 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 248 },
      { index = 2, offset = 472 },
      { index = 3, offset = 696 },
      { index = 4, offset = 860 },
    },
    labels = {
      [0] = { -1, 7, 12, 22, 8, 1 },
      [1] = { -1, 8, 10, 1 },
      [2] = { -1, 10, 18, 1 },
      [3] = { -1, 18, 1 },
      [4] = { -1, 18, 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 7 then goto L7 end
        if at == 12 then goto L12 end
        if at == 22 then goto L22 end
        if at == 8 then goto L8 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1134559232, 1111490560, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L7::
        s:LABEL(7, 0)
        ::L12::
        s:LABEL(12, 0)
        ::L22::
        s:LABEL(22, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1134559232, 1111490560, 0, 30)
        s:at(30)
        s:STOP()
        ::L8::
        s:LABEL(8, 30)
        s:POS_TIME_LINEAR(1134559232, 0, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(1)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1134559232, 1111490560, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L8::
        s:LABEL(8, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1134559232, 1111490560, 0, 30)
        s:at(30)
        s:STOP()
        ::L10::
        s:LABEL(10, 30)
        s:POS_TIME_LINEAR(1134559232, 0, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 10 then goto L10 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1134559232, 1111490560, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L10::
        s:LABEL(10, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1134559232, 1111490560, 0, 30)
        s:at(30)
        s:STOP()
        ::L18::
        s:LABEL(18, 30)
        s:POS_TIME_LINEAR(1134559232, 0, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L1::
        s:LABEL(1, 94)
        s:at(94)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1134559232, 1111490560, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L18::
        s:LABEL(18, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1134559232, 1111490560, 0, 30)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 62)
        s:at(62)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 18 then goto L18 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(3)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:SET_TRANSLATION(1142947840, 1134559232, 0)
        s:STOP_HIDE()
        ::L18::
        s:LABEL(18, 0)
        s:INTERP_POS(20, 4, 1132462080, 1134559232, 0)
        s:at(30)
        s:STOP()
        ::L1::
        s:LABEL(1, 62)
        s:at(62)
        s:EXIT_HIDE2()
    end,
  },
  [8] = {
    texture = "texture_8.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 32 } },
      { id = 1, rect = { 0, 32, 48, 16 } },
      { id = 2, rect = { 0, 48, 48, 16 } },
      { id = 3, rect = { 0, 64, 64, 16 } },
      { id = 4, rect = { 0, 80, 48, 16 } },
      { id = 5, rect = { 0, 96, 64, 16 } },
      { id = 6, rect = { 0, 112, 48, 16 } },
      { id = 7, rect = { 0, 128, 64, 16 } },
      { id = 8, rect = { 0, 144, 32, 16 } },
      { id = 9, rect = { 0, 160, 80, 16 } },
      { id = 10, rect = { 128, 32, 128, 32 } },
      { id = 11, rect = { 128, 64, 128, 48 } },
      { id = 12, rect = { 128, 112, 128, 48 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 172 },
      { index = 2, offset = 316 },
      { index = 3, offset = 500 },
      { index = 4, offset = 716 },
      { index = 5, offset = 932 },
      { index = 6, offset = 1148 },
      { index = 7, offset = 1364 },
      { index = 8, offset = 1580 },
      { index = 9, offset = 1796 },
      { index = 10, offset = 2012 },
      { index = 11, offset = 2228 },
      { index = 12, offset = 2444 },
      { index = 13, offset = 2660 },
      { index = 14, offset = 2876 },
      { index = 15, offset = 3092 },
      { index = 16, offset = 3308 },
      { index = 17, offset = 3524 },
      { index = 18, offset = 3740 },
      { index = 19, offset = 3904 },
      { index = 20, offset = 4068 },
      { index = 21, offset = 4232 },
      { index = 22, offset = 4396 },
      { index = 23, offset = 4560 },
      { index = 24, offset = 4724 },
      { index = 25, offset = 4888 },
      { index = 26, offset = 5052 },
      { index = 27, offset = 5340 },
      { index = 28, offset = 5628 },
    },
    labels = {
      [0] = { 13, 14, 15, 19, -1 },
      [1] = { 15, -1 },
      [2] = { 13, 14, 15, 19, -1 },
      [3] = { 17, 13, 14, 15, -1 },
      [4] = { 17, 13, 14, 15, -1 },
      [5] = { 17, 13, 14, 15, -1 },
      [6] = { 17, 13, 14, 15, -1 },
      [7] = { 17, 13, 14, 15, -1 },
      [8] = { 17, 13, 14, 15, -1 },
      [9] = { 17, 13, 14, 15, -1 },
      [10] = { 17, 13, 14, 15, -1 },
      [11] = { 17, 13, 14, 15, -1 },
      [12] = { 17, 13, 14, 15, -1 },
      [13] = { 17, 13, 14, 15, -1 },
      [14] = { 17, 13, 14, 15, -1 },
      [15] = { 17, 13, 14, 15, -1 },
      [16] = { 17, 13, 14, 15, -1 },
      [17] = { 17, 13, 14, 15, -1 },
      [18] = { 15, 19, -1 },
      [19] = { 15, 19, -1 },
      [20] = { 15, 19, -1 },
      [21] = { 15, 19, -1 },
      [22] = { 15, 19, -1 },
      [23] = { 15, 19, -1 },
      [24] = { 15, 19, -1 },
      [25] = { 15, 19, -1 },
      [26] = { 19, 20, 21, -1 },
      [27] = { 19, 20, 21, -1 },
      [28] = { 19, 20, 21, -1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_TRANSLATION(1134559232, -1040187392, 0)
        s:STOP_HIDE()
        ::L13::
        s:LABEL(13, 0)
        ::L14::
        s:LABEL(14, 0)
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:POS_TIME_DECEL(320.0, 96.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(320.0, -32.0, 0.0, 30)
        s:at(60)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:NOP22()
        s:STOP_HIDE()
        ::L15::
        s:LABEL(15, 0)
        s:POS_TIME_DECEL(24.0, 192.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L13::
        s:LABEL(13, 0)
        ::L14::
        s:LABEL(14, 0)
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:POS_TIME_DECEL(24.0, 128.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 3
    [3] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 4
    [4] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 177.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 5
    [5] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 194.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 6
    [6] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 211.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 7
    [7] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 228.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 8
    [8] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 245.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 9
    [9] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 262.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 10
    [10] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 279.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 11
    [11] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 296.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 12
    [12] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 313.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 13
    [13] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 330.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 14
    [14] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 347.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 15
    [15] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 364.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 16
    [16] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 381.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 17
    [17] = function(s)
        local at = s:entry()
        if at == 17 then goto L17 end
        if at == 13 then goto L13 end
        if at == 14 then goto L14 end
        if at == 15 then goto L15 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L17::
        s:LABEL(17, 0)
        s:POS_TIME_DECEL(24.0, 160.0, 0.0, 30)
        s:at(30)
        s:STOP()
        ::L13::
        s:LABEL(13, 30)
        ::L14::
        s:LABEL(14, 30)
        s:POS_TIME_DECEL(24.0, 398.0, 0.0, 30)
        s:at(60)
        s:STOP()
        ::L15::
        s:LABEL(15, 60)
        ::Lm1::
        s:LABEL(-1, 60)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(120)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 18
    [18] = function(s)
        local at = s:entry()
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1103101952, 1132462080, 0)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 19
    [19] = function(s)
        local at = s:entry()
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1103101952, 1133019136, 0)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 20
    [20] = function(s)
        local at = s:entry()
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1103101952, 1133576192, 0)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 21
    [21] = function(s)
        local at = s:entry()
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1103101952, 1134133248, 0)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 22
    [22] = function(s)
        local at = s:entry()
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1103101952, 1134690304, 0)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 23
    [23] = function(s)
        local at = s:entry()
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1103101952, 1135247360, 0)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 24
    [24] = function(s)
        local at = s:entry()
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1103101952, 1135804416, 0)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 25
    [25] = function(s)
        local at = s:entry()
        if at == 15 then goto L15 end
        if at == 19 then goto L19 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:SET_TRANSLATION(1103101952, -1040187392, 0)
        s:SET_ALPHA(0)
        s:STOP_HIDE()
        ::L15::
        s:LABEL(15, 0)
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1103101952, 1136361472, 0)
        s:at(30)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 30)
        s:POS_TIME_DECEL(24.0, -32.0, 0.0, 30)
        s:at(90)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 26
    [26] = function(s)
        local at = s:entry()
        if at == 19 then goto L19 end
        if at == 20 then goto L20 end
        if at == 21 then goto L21 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(10)
        s:SET_TRANSLATION(1136656384, -1040187392, 0)
        s:SET_ALPHA(255)
        s:STOP_HIDE()
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1136656384, 1132462080, 0)
        s:at(30)
        s:STOP()
        ::L20::
        s:LABEL(20, 30)
        s:INTERP_POS(10, 4, 1136525312, 1132199936, 0)
        s:INTERP_ALPHA(10, 4, 255)
        s:at(40)
        s:STOP()
        ::L21::
        s:LABEL(21, 40)
        s:INTERP_POS(10, 4, 1136656384, 1132462080, 0)
        s:INTERP_ALPHA(10, 4, 96)
        s:at(50)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 50)
        s:POS_TIME_DECEL(384.0, -32.0, 0.0, 30)
        s:at(110)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 27
    [27] = function(s)
        local at = s:entry()
        if at == 19 then goto L19 end
        if at == 20 then goto L20 end
        if at == 21 then goto L21 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(11)
        s:SET_TRANSLATION(1136656384, -1040187392, 0)
        s:SET_ALPHA(255)
        s:STOP_HIDE()
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1136656384, 1134034944, 0)
        s:at(30)
        s:STOP()
        ::L20::
        s:LABEL(20, 30)
        s:INTERP_POS(10, 4, 1136525312, 1133903872, 0)
        s:INTERP_ALPHA(10, 4, 255)
        s:at(40)
        s:STOP()
        ::L21::
        s:LABEL(21, 40)
        s:INTERP_POS(10, 4, 1136656384, 1134034944, 0)
        s:INTERP_ALPHA(10, 4, 96)
        s:at(50)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 50)
        s:POS_TIME_DECEL(384.0, -32.0, 0.0, 30)
        s:at(110)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 28
    [28] = function(s)
        local at = s:entry()
        if at == 19 then goto L19 end
        if at == 20 then goto L20 end
        if at == 21 then goto L21 end
        if at == -1 then goto Lm1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(12)
        s:SET_TRANSLATION(1136656384, -1040187392, 0)
        s:SET_ALPHA(255)
        s:STOP_HIDE()
        ::L19::
        s:LABEL(19, 0)
        s:INTERP_POS(30, 4, 1136656384, 1135607808, 0)
        s:at(30)
        s:STOP()
        ::L20::
        s:LABEL(20, 30)
        s:INTERP_POS(10, 4, 1136525312, 1135476736, 0)
        s:INTERP_ALPHA(10, 4, 255)
        s:at(40)
        s:STOP()
        ::L21::
        s:LABEL(21, 40)
        s:INTERP_POS(10, 4, 1136656384, 1135607808, 0)
        s:INTERP_ALPHA(10, 4, 96)
        s:at(50)
        s:STOP()
        ::Lm1::
        s:LABEL(-1, 50)
        s:POS_TIME_DECEL(384.0, -32.0, 0.0, 30)
        s:at(110)
        s:STOP_HIDE()
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
  [9] = {
    texture = "texture_9.png",
    width = 512, height = 512, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 96 } },
      { id = 1, rect = { 256, 0, 256, 96 } },
      { id = 2, rect = { 0, 96, 256, 96 } },
      { id = 3, rect = { 256, 96, 256, 96 } },
      { id = 4, rect = { 0, 480, 256, 32 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 300 },
      { index = 2, offset = 600 },
    },
    labels = {
      [0] = { -1, 12, 6, 8, 10, 9, 1 },
      [1] = { -1, 22, 6, 8, 10, 9, 1 },
      [2] = { -1, 22, 6, 8, 10, 9, 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 12 then goto L12 end
        if at == 6 then goto L6 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 9 then goto L9 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1128267776, 1125122048, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L12::
        s:LABEL(12, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1128267776, 1128267776, 0, 30)
        s:at(30)
        s:STOP()
        ::L6::
        s:LABEL(6, 30)
        ::L8::
        s:LABEL(8, 30)
        ::L10::
        s:LABEL(10, 30)
        s:POS_TIME_LINEAR(1128267776, 1125122048, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 62)
        s:POS_TIME_DECEL(16.0, 384.0, 0.0, 20)
        s:at(92)
        s:STOP()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 1
    [1] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 22 then goto L22 end
        if at == 6 then goto L6 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 9 then goto L9 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1126432768, 1127743488, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L22::
        s:LABEL(22, 0)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1129578496, 1127743488, 0, 30)
        s:at(30)
        s:STOP()
        ::L6::
        s:LABEL(6, 30)
        ::L8::
        s:LABEL(8, 30)
        ::L10::
        s:LABEL(10, 30)
        s:POS_TIME_LINEAR(1126432768, 1127743488, 0, 48)
        s:FADE(0, 32)
        s:at(62)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 62)
        s:POS_TIME_DECEL(16.0, 384.0, 0.0, 20)
        s:at(92)
        s:STOP()
        ::L1::
        s:LABEL(1, 124)
        s:at(124)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        local at = s:entry()
        if at == -1 then goto Lm1 end
        if at == 22 then goto L22 end
        if at == 6 then goto L6 end
        if at == 8 then goto L8 end
        if at == 10 then goto L10 end
        if at == 9 then goto L9 end
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(2)
        s:NOP22()
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:SET_TRANSLATION(1130102784, 1132986368, 0)
        ::Lm1::
        s:LABEL(-1, 0)
        s:STOP_HIDE()
        ::L22::
        s:LABEL(22, 10)
        s:at(10)
        s:FADE(255, 16)
        s:POS_TIME_LINEAR(1126957056, 1132986368, 0, 30)
        s:at(40)
        s:STOP()
        ::L6::
        s:LABEL(6, 40)
        ::L8::
        s:LABEL(8, 40)
        ::L10::
        s:LABEL(10, 40)
        s:POS_TIME_LINEAR(1130102784, 1132986368, 0, 48)
        s:FADE(0, 32)
        s:at(72)
        s:STOP_HIDE()
        ::L9::
        s:LABEL(9, 72)
        s:POS_TIME_DECEL(16.0, 384.0, 0.0, 20)
        s:at(102)
        s:STOP()
        ::L1::
        s:LABEL(1, 134)
        s:at(134)
        s:EXIT_HIDE2()
    end,
  },
}
