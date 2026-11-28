-- etama: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 240, 8, 8 } },
      { id = 1, rect = { 8, 240, 8, 8 } },
      { id = 2, rect = { 16, 240, 8, 8 } },
      { id = 3, rect = { 24, 240, 8, 8 } },
      { id = 4, rect = { 32, 240, 8, 8 } },
      { id = 5, rect = { 40, 240, 8, 8 } },
      { id = 6, rect = { 48, 240, 8, 8 } },
      { id = 7, rect = { 56, 240, 8, 8 } },
      { id = 8, rect = { 0, 248, 8, 8 } },
      { id = 9, rect = { 8, 248, 8, 8 } },
      { id = 10, rect = { 16, 248, 8, 8 } },
      { id = 11, rect = { 24, 248, 8, 8 } },
      { id = 12, rect = { 32, 248, 8, 8 } },
      { id = 13, rect = { 40, 248, 8, 8 } },
      { id = 14, rect = { 48, 248, 8, 8 } },
      { id = 15, rect = { 56, 248, 8, 8 } },
      { id = 16, rect = { 0, 32, 16, 16 } },
      { id = 17, rect = { 16, 32, 16, 16 } },
      { id = 18, rect = { 32, 32, 16, 16 } },
      { id = 19, rect = { 48, 32, 16, 16 } },
      { id = 20, rect = { 64, 32, 16, 16 } },
      { id = 21, rect = { 80, 32, 16, 16 } },
      { id = 22, rect = { 96, 32, 16, 16 } },
      { id = 23, rect = { 112, 32, 16, 16 } },
      { id = 24, rect = { 128, 32, 16, 16 } },
      { id = 25, rect = { 144, 32, 16, 16 } },
      { id = 26, rect = { 160, 32, 16, 16 } },
      { id = 27, rect = { 176, 32, 16, 16 } },
      { id = 28, rect = { 192, 32, 16, 16 } },
      { id = 29, rect = { 208, 32, 16, 16 } },
      { id = 30, rect = { 224, 32, 16, 16 } },
      { id = 31, rect = { 240, 32, 16, 16 } },
      { id = 32, rect = { 1, 64, 14, 16 } },
      { id = 33, rect = { 17, 64, 14, 16 } },
      { id = 34, rect = { 33, 64, 14, 16 } },
      { id = 35, rect = { 49, 64, 14, 16 } },
      { id = 36, rect = { 65, 64, 14, 16 } },
      { id = 37, rect = { 81, 64, 14, 16 } },
      { id = 38, rect = { 97, 64, 14, 16 } },
      { id = 39, rect = { 113, 64, 14, 16 } },
      { id = 40, rect = { 129, 64, 14, 16 } },
      { id = 41, rect = { 145, 64, 14, 16 } },
      { id = 42, rect = { 161, 64, 14, 16 } },
      { id = 43, rect = { 177, 64, 14, 16 } },
      { id = 44, rect = { 193, 64, 14, 16 } },
      { id = 45, rect = { 209, 64, 14, 16 } },
      { id = 46, rect = { 225, 64, 14, 16 } },
      { id = 47, rect = { 241, 64, 14, 16 } },
      { id = 48, rect = { 0, 48, 16, 16 } },
      { id = 49, rect = { 16, 48, 16, 16 } },
      { id = 50, rect = { 32, 48, 16, 16 } },
      { id = 51, rect = { 48, 48, 16, 16 } },
      { id = 52, rect = { 64, 48, 16, 16 } },
      { id = 53, rect = { 80, 48, 16, 16 } },
      { id = 54, rect = { 96, 48, 16, 16 } },
      { id = 55, rect = { 112, 48, 16, 16 } },
      { id = 56, rect = { 128, 48, 16, 16 } },
      { id = 57, rect = { 144, 48, 16, 16 } },
      { id = 58, rect = { 160, 48, 16, 16 } },
      { id = 59, rect = { 176, 48, 16, 16 } },
      { id = 60, rect = { 192, 48, 16, 16 } },
      { id = 61, rect = { 208, 48, 16, 16 } },
      { id = 62, rect = { 224, 48, 16, 16 } },
      { id = 63, rect = { 240, 48, 16, 16 } },
      { id = 64, rect = { 1, 80, 14, 16 } },
      { id = 65, rect = { 17, 80, 14, 16 } },
      { id = 66, rect = { 33, 80, 14, 16 } },
      { id = 67, rect = { 49, 80, 14, 16 } },
      { id = 68, rect = { 65, 80, 14, 16 } },
      { id = 69, rect = { 81, 80, 14, 16 } },
      { id = 70, rect = { 97, 80, 14, 16 } },
      { id = 71, rect = { 113, 80, 14, 16 } },
      { id = 72, rect = { 129, 80, 14, 16 } },
      { id = 73, rect = { 145, 80, 14, 16 } },
      { id = 74, rect = { 161, 80, 14, 16 } },
      { id = 75, rect = { 177, 80, 14, 16 } },
      { id = 76, rect = { 193, 80, 14, 16 } },
      { id = 77, rect = { 209, 80, 14, 16 } },
      { id = 78, rect = { 225, 80, 14, 16 } },
      { id = 79, rect = { 241, 80, 14, 16 } },
      { id = 80, rect = { 1, 96, 14, 16 } },
      { id = 81, rect = { 17, 96, 14, 16 } },
      { id = 82, rect = { 33, 96, 14, 16 } },
      { id = 83, rect = { 49, 96, 14, 16 } },
      { id = 84, rect = { 65, 96, 14, 16 } },
      { id = 85, rect = { 81, 96, 14, 16 } },
      { id = 86, rect = { 97, 96, 14, 16 } },
      { id = 87, rect = { 113, 96, 14, 16 } },
      { id = 88, rect = { 129, 96, 14, 16 } },
      { id = 89, rect = { 145, 96, 14, 16 } },
      { id = 90, rect = { 161, 96, 14, 16 } },
      { id = 91, rect = { 177, 96, 14, 16 } },
      { id = 92, rect = { 193, 96, 14, 16 } },
      { id = 93, rect = { 209, 96, 14, 16 } },
      { id = 94, rect = { 225, 96, 14, 16 } },
      { id = 95, rect = { 241, 96, 14, 16 } },
      { id = 96, rect = { 1, 16, 14, 16 } },
      { id = 97, rect = { 17, 16, 14, 16 } },
      { id = 98, rect = { 33, 16, 14, 16 } },
      { id = 99, rect = { 49, 16, 14, 16 } },
      { id = 100, rect = { 65, 16, 14, 16 } },
      { id = 101, rect = { 81, 16, 14, 16 } },
      { id = 102, rect = { 97, 16, 14, 16 } },
      { id = 103, rect = { 113, 16, 14, 16 } },
      { id = 104, rect = { 129, 16, 14, 16 } },
      { id = 105, rect = { 145, 16, 14, 16 } },
      { id = 106, rect = { 161, 16, 14, 16 } },
      { id = 107, rect = { 177, 16, 14, 16 } },
      { id = 108, rect = { 193, 16, 14, 16 } },
      { id = 109, rect = { 209, 16, 14, 16 } },
      { id = 110, rect = { 225, 16, 14, 16 } },
      { id = 111, rect = { 241, 16, 14, 16 } },
      { id = 112, rect = { 0, 112, 32, 32 } },
      { id = 113, rect = { 32, 112, 32, 32 } },
      { id = 114, rect = { 64, 112, 32, 32 } },
      { id = 115, rect = { 96, 112, 32, 32 } },
      { id = 116, rect = { 128, 112, 32, 32 } },
      { id = 117, rect = { 160, 112, 32, 32 } },
      { id = 118, rect = { 192, 112, 32, 32 } },
      { id = 119, rect = { 224, 112, 32, 32 } },
      { id = 120, rect = { 0, 176, 32, 32 } },
      { id = 121, rect = { 32, 176, 32, 32 } },
      { id = 122, rect = { 64, 176, 32, 32 } },
      { id = 123, rect = { 96, 176, 32, 32 } },
      { id = 124, rect = { 128, 176, 32, 32 } },
      { id = 125, rect = { 160, 176, 32, 32 } },
      { id = 126, rect = { 192, 176, 32, 32 } },
      { id = 127, rect = { 224, 176, 32, 32 } },
      { id = 128, rect = { 0, 144, 32, 32 } },
      { id = 129, rect = { 32, 144, 32, 32 } },
      { id = 130, rect = { 64, 144, 32, 32 } },
      { id = 131, rect = { 96, 144, 32, 32 } },
      { id = 132, rect = { 128, 144, 32, 32 } },
      { id = 133, rect = { 160, 144, 32, 32 } },
      { id = 134, rect = { 192, 144, 32, 32 } },
      { id = 135, rect = { 224, 144, 32, 32 } },
      { id = 136, rect = { 144, 240, 16, 16 } },
      { id = 137, rect = { 160, 240, 16, 16 } },
      { id = 138, rect = { 176, 240, 16, 16 } },
      { id = 139, rect = { 192, 240, 16, 16 } },
      { id = 140, rect = { 208, 240, 16, 16 } },
      { id = 141, rect = { 64, 240, 16, 16 } },
      { id = 142, rect = { 80, 240, 16, 16 } },
      { id = 143, rect = { 96, 240, 16, 16 } },
      { id = 144, rect = { 112, 240, 16, 16 } },
      { id = 145, rect = { 128, 240, 16, 16 } },
      { id = 146, rect = { 1, 209, 30, 30 } },
      { id = 147, rect = { 33, 209, 30, 30 } },
      { id = 148, rect = { 97, 209, 30, 30 } },
      { id = 149, rect = { 161, 209, 30, 30 } },
      { id = 150, rect = { 193, 209, 30, 30 } },
      { id = 151, rect = { 225, 209, 30, 30 } },
      { id = 152, rect = { 0, 1, 16, 14 } },
      { id = 153, rect = { 16, 1, 16, 14 } },
      { id = 154, rect = { 32, 1, 16, 14 } },
      { id = 155, rect = { 48, 1, 16, 14 } },
      { id = 156, rect = { 64, 1, 16, 14 } },
      { id = 157, rect = { 80, 1, 16, 14 } },
      { id = 158, rect = { 96, 1, 16, 14 } },
      { id = 159, rect = { 112, 1, 16, 14 } },
      { id = 160, rect = { 128, 1, 16, 14 } },
      { id = 161, rect = { 144, 1, 16, 14 } },
      { id = 162, rect = { 160, 1, 16, 14 } },
      { id = 163, rect = { 176, 1, 16, 14 } },
      { id = 164, rect = { 192, 1, 16, 14 } },
      { id = 165, rect = { 208, 1, 16, 14 } },
      { id = 166, rect = { 224, 1, 16, 14 } },
      { id = 167, rect = { 240, 1, 16, 14 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 28 },
      { index = 2, offset = 56 },
      { index = 3, offset = 96 },
      { index = 4, offset = 124 },
      { index = 5, offset = 164 },
      { index = 6, offset = 204 },
      { index = 7, offset = 244 },
      { index = 8, offset = 272 },
      { index = 9, offset = 312 },
      { index = 10, offset = 352 },
      { index = 11, offset = 392 },
      { index = 12, offset = 432 },
      { index = 13, offset = 460 },
      { index = 14, offset = 488 },
      { index = 15, offset = 528 },
      { index = 16, offset = 588 },
      { index = 17, offset = 664 },
      { index = 18, offset = 740 },
      { index = 19, offset = 828 },
      { index = 20, offset = 916 },
      { index = 21, offset = 1004 },
      { index = 22, offset = 1092 },
      { index = 23, offset = 1180 },
      { index = 24, offset = 1268 },
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
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(16)
        s:EXIT()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(32)
        s:SET_AUTO_ROTATE(1)
        s:EXIT()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(48)
        s:EXIT()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(64)
        s:SET_AUTO_ROTATE(1)
        s:EXIT()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(80)
        s:SET_AUTO_ROTATE(1)
        s:EXIT()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(96)
        s:SET_AUTO_ROTATE(1)
        s:EXIT()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(112)
        s:EXIT()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_AUTO_ROTATE(1)
        s:SET_ACTIVE_SPRITE(120)
        s:EXIT()
    end,

    -- stream 9
    [9] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(128)
        s:SET_AUTO_ROTATE(1)
        s:EXIT()
    end,

    -- stream 10
    [10] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(152)
        s:SET_BLEND(1)
        s:EXIT()
    end,

    -- stream 11
    [11] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(48)
        s:SET_BLEND(1)
        s:EXIT()
    end,

    -- stream 12
    [12] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(152)
        s:EXIT()
    end,

    -- stream 13
    [13] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(48)
        s:EXIT()
    end,

    -- stream 14
    [14] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(96)
        s:SET_BLEND(1)
        s:EXIT()
    end,

    -- stream 15
    [15] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(136)
        s:FADE(0, 12)
        s:SET_SCALE_SPEED(1034594987, 1034594987)
        s:at(12)
        s:EXIT_HIDE2()
    end,

    -- stream 16
    [16] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(136)
        s:FADE(0, 12)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_SCALE_SPEED(1034594987, 1034594987)
        s:at(12)
        s:EXIT_HIDE2()
    end,

    -- stream 17
    [17] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(136)
        s:FADE(0, 12)
        s:SET_SCALE(1077936128, 1077936128)
        s:SET_SCALE_SPEED(1042983595, 1042983595)
        s:at(12)
        s:EXIT_HIDE2()
    end,

    -- stream 18
    [18] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(141)
        s:SET_ALPHA(0)
        s:FADE(144, 8)
        s:SET_SCALE(1073741824, 1073741824)
        s:SET_SCALE_SPEED(-1114486491, -1114486491)
        s:at(10)
        s:EXIT_HIDE2()
    end,

    -- stream 19
    [19] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(146)
        s:SET_ALPHA(0)
        s:FADE(144, 16)
        s:SET_SCALE(1077936128, 1077936128)
        s:SET_SCALE_SPEED(-1112888661, -1112888661)
        s:at(16)
        s:EXIT_HIDE2()
    end,

    -- stream 20
    [20] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(146)
        s:SET_ALPHA(0)
        s:FADE(144, 32)
        s:SET_SCALE(1082130432, 1082130432)
        s:SET_SCALE_SPEED(-1114007142, -1114007142)
        s:at(32)
        s:EXIT_HIDE2()
    end,

    -- stream 21
    [21] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(146)
        s:SET_ALPHA(0)
        s:FADE(144, 8)
        s:SET_SCALE(1073741824, 1073741824)
        s:SET_SCALE_SPEED(-1114486491, -1114486491)
        s:at(10)
        s:EXIT_HIDE2()
    end,

    -- stream 22
    [22] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(146)
        s:SET_ALPHA(0)
        s:FADE(144, 16)
        s:SET_SCALE(1077936128, 1077936128)
        s:SET_SCALE_SPEED(-1112888661, -1112888661)
        s:at(16)
        s:EXIT_HIDE2()
    end,

    -- stream 23
    [23] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(146)
        s:SET_ALPHA(0)
        s:FADE(144, 32)
        s:SET_SCALE(1082130432, 1082130432)
        s:SET_SCALE_SPEED(-1114007142, -1114007142)
        s:at(32)
        s:EXIT_HIDE2()
    end,

    -- stream 24
    [24] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(146)
        s:SET_ALPHA(0)
        s:FADE(144, 32)
        s:SET_SCALE(1086324736, 1086324736)
        s:SET_SCALE_SPEED(-1110651699, -1110651699)
        s:at(32)
        s:EXIT_HIDE2()
    end,
  },
  [1] = {
    texture = "texture_1.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 64, 64 } },
      { id = 1, rect = { 64, 0, 64, 64 } },
      { id = 2, rect = { 128, 0, 64, 64 } },
      { id = 3, rect = { 192, 0, 64, 64 } },
      { id = 4, rect = { 0, 64, 16, 16 } },
      { id = 5, rect = { 16, 64, 16, 16 } },
      { id = 6, rect = { 32, 64, 16, 16 } },
      { id = 7, rect = { 48, 64, 16, 16 } },
      { id = 8, rect = { 64, 64, 16, 16 } },
      { id = 9, rect = { 80, 64, 16, 16 } },
      { id = 10, rect = { 96, 64, 16, 16 } },
      { id = 11, rect = { 112, 64, 16, 16 } },
      { id = 12, rect = { 0, 80, 16, 16 } },
      { id = 13, rect = { 112, 64, 16, 16 } },
      { id = 14, rect = { 128, 64, 16, 16 } },
      { id = 15, rect = { 144, 64, 16, 16 } },
      { id = 16, rect = { 160, 64, 16, 16 } },
      { id = 17, rect = { 176, 64, 16, 16 } },
      { id = 18, rect = { 192, 64, 16, 16 } },
      { id = 19, rect = { 208, 64, 16, 16 } },
      { id = 20, rect = { 224, 64, 16, 16 } },
      { id = 21, rect = { 240, 64, 16, 16 } },
      { id = 22, rect = { 240, 64, 16, 16 } },
      { id = 23, rect = { 240, 64, 16, 16 } },
      { id = 24, rect = { 193, 177, 62, 62 } },
      { id = 25, rect = { 65, 209, 30, 30 } },
      { id = 26, rect = { 97, 209, 30, 30 } },
      { id = 27, rect = { 64, 112, 64, 64 } },
      { id = 28, rect = { 128, 112, 64, 64 } },
      { id = 29, rect = { 1, 241, 14, 14 } },
      { id = 30, rect = { 17, 241, 14, 14 } },
      { id = 31, rect = { 33, 241, 14, 14 } },
      { id = 32, rect = { 49, 241, 14, 14 } },
      { id = 33, rect = { 65, 241, 14, 14 } },
      { id = 34, rect = { 81, 241, 14, 14 } },
      { id = 35, rect = { 97, 241, 14, 14 } },
      { id = 36, rect = { 113, 241, 14, 14 } },
      { id = 37, rect = { 129, 241, 14, 14 } },
      { id = 38, rect = { 145, 241, 14, 14 } },
      { id = 39, rect = { 161, 241, 14, 14 } },
      { id = 40, rect = { 177, 241, 14, 14 } },
      { id = 41, rect = { 193, 241, 14, 14 } },
      { id = 42, rect = { 209, 241, 14, 14 } },
      { id = 43, rect = { 225, 241, 14, 14 } },
      { id = 44, rect = { 241, 241, 14, 14 } },
      { id = 45, rect = { 1, 176, 63, 63 } },
      { id = 46, rect = { 160, 224, 16, 16 } },
      { id = 47, rect = { 64, 176, 32, 32 } },
      { id = 48, rect = { 96, 176, 32, 32 } },
      { id = 49, rect = { 0, 112, 64, 64 } },
    },
    scripts = {
      { index = 0, offset = 0 },
      { index = 1, offset = 40 },
      { index = 2, offset = 112 },
      { index = 3, offset = 212 },
      { index = 4, offset = 300 },
      { index = 5, offset = 420 },
      { index = 6, offset = 520 },
      { index = 7, offset = 620 },
      { index = 8, offset = 824 },
      { index = 9, offset = 1028 },
      { index = 10, offset = 1264 },
      { index = 11, offset = 1472 },
      { index = 12, offset = 1592 },
      { index = 13, offset = 1712 },
      { index = 14, offset = 1832 },
      { index = 15, offset = 1952 },
      { index = 16, offset = 2084 },
      { index = 17, offset = 2216 },
      { index = 18, offset = 2348 },
      { index = 19, offset = 2480 },
      { index = 20, offset = 2580 },
      { index = 21, offset = 2720 },
      { index = 22, offset = 2832 },
      { index = 23, offset = 2960 },
      { index = 24, offset = 3088 },
      { index = 25, offset = 3304 },
      { index = 26, offset = 3540 },
      { index = 27, offset = 3724 },
      { index = 28, offset = 3824 },
      { index = 29, offset = 3852 },
      { index = 30, offset = 3880 },
      { index = 31, offset = 3908 },
      { index = 32, offset = 3936 },
      { index = 33, offset = 3964 },
      { index = 34, offset = 3992 },
      { index = 35, offset = 4032 },
      { index = 36, offset = 4060 },
      { index = 37, offset = 4240 },
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
      [26] = { 1 },
      [27] = {  },
      [28] = {  },
      [29] = {  },
      [30] = {  },
      [31] = {  },
      [32] = {  },
      [33] = {  },
      [34] = {  },
      [35] = {  },
      [36] = {  },
      [37] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_BLEND(1)
        s:EXIT()
    end,

    -- stream 1
    [1] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_BLEND(1)
        s:FADE(0, 16)
        s:SET_SCALE_SPEED(-1115684864, -1115684864)
        s:at(16)
        s:EXIT_HIDE2()
    end,

    -- stream 2
    [2] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:FADE(255, 32)
        s:SET_SCALE(1073741824, 1073741824)
        s:SET_SCALE_SPEED(-1121277269, -1121277269)
        s:at(24)
        s:EXIT_HIDE2()
    end,

    -- stream 3
    [3] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(24)
        s:SET_SCALE(1056964608, 1056964608)
        s:SET_SCALE_SPEED(1033476506, 1033476506)
        s:FADE(0, 20)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20)
        s:EXIT_HIDE2()
    end,

    -- stream 4
    [4] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(26)
        s:SET_BLEND(1)
        s:FADE(0, 20)
        s:SET_SCALE(1056964608, 1056964608)
        s:SET_SCALE_SPEED(1033476506, 1033476506)
        s:SET_ANGLE_VEL(0, 0, -1088841318)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20)
        s:EXIT_HIDE2()
    end,

    -- stream 5
    [5] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(45)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_SCALE(1065353216, 1065353216)
        s:SET_SCALE_SPEED(1041865114, 1041865114)
        s:at(20)
        s:FADE(0, 20)
        s:at(40)
        s:EXIT_HIDE2()
    end,

    -- stream 6
    [6] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(27)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:FADE(0, 40)
        s:SET_SCALE(1077936128, 1077936128)
        s:SET_SCALE_SPEED(-1114007142, -1114007142)
        s:at(40)
        s:EXIT_HIDE2()
    end,

    -- stream 7
    [7] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(27)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:FADE(128, 40)
        s:RAND_FLOAT({r = 1176260608}, 1065353216)
        s:ADD_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1069547520)
        s:SET_SCALE({r = 1176260608}, {r = 1176260608})
        s:RAND_FLOAT({r = 1176260608}, 1048971922)
        s:SUB_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1040583314)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        s:SET_SCALE_SPEED(-1127428915, -1127428915)
        s:at(60)
        s:EXIT_HIDE2()
    end,

    -- stream 8
    [8] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(27)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:FADE(48, 120)
        s:RAND_FLOAT({r = 1176260608}, 1065353216)
        s:ADD_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1069547520)
        s:SET_SCALE({r = 1176260608}, {r = 1176260608})
        s:RAND_FLOAT({r = 1176260608}, 1048971922)
        s:SUB_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1040583314)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        s:SET_SCALE_SPEED(-1144206131, -1144206131)
        s:at(240)
        s:EXIT_HIDE2()
    end,

    -- stream 9
    [9] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(48)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(60, 0, 255)
        s:RAND_FLOAT({r = 1176260608}, 1065353216)
        s:ADD_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1080033280)
        s:SET_SCALE({r = 1176260608}, {r = 1176260608})
        s:RAND_FLOAT({r = 1176260608}, 1048971922)
        s:SUB_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1040583314)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        s:INTERP_SCALE_2(120, 4, 1053609165, 1053609165)
        s:at(60)
        s:INTERP_ALPHA(60, 0, 0)
        s:at(120)
        s:EXIT_HIDE2()
    end,

    -- stream 10
    [10] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(28)
        s:SET_ZWRITE_DISABLE(1)
        s:RAND_FLOAT({r = 1176260608}, 1086918619)
        s:SUB_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1078530011)
        s:SET_ROTATION(0, 0, {r = 1176260608})
        s:RAND({r = 10000}, 2)
        s:JUMP_IF_NEQ({r = 10000}, 0, 140, 0)
        s:SET_ANGLE_VEL(0, 0, 1059117436)
        s:SET_CLOCK(0, 176)
        goto J176
        s:SET_ANGLE_VEL(0, 0, -1088366212)
        ::J176::
        s:at(10)
        s:FADE(0, 20)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 11
    [11] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(29)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(192)
        s:FADE(0, 30)
        s:SET_SCALE(1065353216, 1065353216)
        s:INTERP_SCALE_2(30, 0, 0, 0)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 12
    [12] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(33)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(128)
        s:FADE(0, 30)
        s:SET_SCALE(1065353216, 1065353216)
        s:INTERP_SCALE_2(30, 0, 0, 0)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 13
    [13] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(37)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(128)
        s:FADE(0, 30)
        s:SET_SCALE(1065353216, 1065353216)
        s:INTERP_SCALE_2(30, 0, 0, 0)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 14
    [14] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(41)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(128)
        s:FADE(0, 30)
        s:SET_SCALE(1065353216, 1065353216)
        s:INTERP_SCALE_2(30, 0, 0, 0)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 15
    [15] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(29)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(128)
        s:FADE(0, 30)
        s:SET_SCALE(1060320051, 1060320051)
        s:SET_SCALE_SPEED(-1128323700, -1128323700)
        s:SET_ANGLE_VEL(0, 0, 1058642330)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 16
    [16] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(33)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(128)
        s:FADE(0, 30)
        s:SET_SCALE(1060320051, 1060320051)
        s:SET_SCALE_SPEED(-1128323700, -1128323700)
        s:SET_ANGLE_VEL(0, 0, 1058642330)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 17
    [17] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(37)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(128)
        s:FADE(0, 30)
        s:SET_SCALE(1060320051, 1060320051)
        s:SET_SCALE_SPEED(-1128323700, -1128323700)
        s:SET_ANGLE_VEL(0, 0, 1058642330)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 18
    [18] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(41)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(128)
        s:FADE(0, 30)
        s:SET_SCALE(1060320051, 1060320051)
        s:SET_SCALE_SPEED(-1128323700, -1128323700)
        s:SET_ANGLE_VEL(0, 0, 1058642330)
        s:at(30)
        s:EXIT_HIDE2()
    end,

    -- stream 19
    [19] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(45)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:FADE(0, 40)
        s:SET_SCALE(1056964608, 1056964608)
        s:SET_SCALE_SPEED(1050253722, 1050253722)
        s:at(40)
        s:EXIT_HIDE2()
    end,

    -- stream 20
    [20] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(46)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ANGLE_VEL(0, 0, -1102508069)
        ::J72::
        s:at(60)
        s:SET_ALPHA(255)
        s:at(120)
        s:FADE(64, 60)
        s:at(180)
        s:FADE(255, 60)
        s:at(240)
        s:SET_CLOCK(0, 72)
        goto J72
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 21
    [21] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(29)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:SET_ALPHA(48)
        s:FADE(0, 120)
        s:SET_SCALE(1077936128, 1077936128)
        s:SET_SCALE_SPEED(-1127428915, -1127428915)
        s:at(120)
        s:EXIT_HIDE2()
    end,

    -- stream 22
    [22] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(25)
        s:SET_BLEND(1)
        s:SET_SCALE(1069547520, 1069547520)
        s:SET_ALPHA(0)
        s:FADE(128, 20)
        s:at(20)
        s:SET_SCALE_SPEED(-1148680055, -1148680055)
        s:at(140)
        s:SET_SCALE_SPEED(998803593, 998803593)
        s:SET_ZWRITE_DISABLE(1)
        s:at(20)
        s:EXIT_HIDE2()
    end,

    -- stream 23
    [23] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(47)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_CAMERA_MODE(1)
        s:SET_ALPHA(0)
        s:FADE(64, 30)
        s:SET_SCALE(1045220557, 1045220557)
        s:SET_SCALE_SPEED(-1171340482, -1171340482)
        s:at(30)
        s:FADE(0, 300)
        s:at(300)
        s:EXIT_HIDE2()
    end,

    -- stream 24
    [24] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(47)
        s:SET_BLEND(1)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(255)
        s:RAND_FLOAT({r = 1176260608}, 1065353216)
        s:ADD_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1056964608)
        s:SET_SCALE({r = 1176260608}, {r = 1176260608})
        s:INTERP_SCALE_2(90, 1, 0, 0)
        s:RAND_FLOAT({r = 1176260608}, 1029076816)
        s:SUB_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1020688208)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        s:at(30)
        s:INTERP_ALPHA(60, 1, 0)
        s:at(90)
        s:EXIT_HIDE2()
    end,

    -- stream 25
    [25] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(48)
        s:SET_BLEND(1)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(255)
        s:RAND_FLOAT({r = 1176260608}, 1073741824)
        s:ADD_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1056964608)
        s:SET_SCALE({r = 1176260608}, {r = 1176260608})
        s:INTERP_SCALE_2(90, 1, 0, 0)
        s:SET_ROTATION(0, 0, {r = 1176260608})
        s:RAND_FLOAT({r = 1176260608}, 1029076816)
        s:SUB_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1020688208)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        s:at(30)
        s:INTERP_ALPHA(60, 1, 0)
        s:at(90)
        s:EXIT_HIDE2()
    end,

    -- stream 26
    [26] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(49)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_ALPHA(0)
        s:RAND_FLOAT({r = 1176260608}, 1065353216)
        s:ADD_FLOAT_2({r = 1176260608}, {r = 1176260608}, 1056964608)
        s:MOV_FLOAT({r = 1176260608}, 1020688208)
        s:SET_ANGLE_VEL(0, 0, {r = 1176260608})
        s:INTERP_ALPHA(20, 1, 255)
        s:at(20)
        s:STOP()
        ::L1::
        s:LABEL(1, 20)
        s:INTERP_ALPHA(30, 1, 0)
        s:at(50)
        s:EXIT_HIDE2()
    end,

    -- stream 27
    [27] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(45)
        s:SET_ZWRITE_DISABLE(1)
        s:SET_BLEND(1)
        s:FADE(0, 40)
        s:SET_SCALE(1084227584, 1084227584)
        s:SET_SCALE_SPEED(-1107296256, -1107296256)
        s:at(40)
        s:EXIT_HIDE2()
    end,

    -- stream 28
    [28] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(4)
        s:EXIT()
    end,

    -- stream 29
    [29] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(5)
        s:EXIT()
    end,

    -- stream 30
    [30] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(6)
        s:EXIT()
    end,

    -- stream 31
    [31] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(7)
        s:EXIT()
    end,

    -- stream 32
    [32] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(8)
        s:EXIT()
    end,

    -- stream 33
    [33] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(9)
        s:EXIT()
    end,

    -- stream 34
    [34] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(10)
        s:SET_ALPHA(192)
        s:EXIT()
    end,

    -- stream 35
    [35] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(11)
        s:EXIT()
    end,

    -- stream 36
    [36] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(12)
        s:SUB_FLOAT_2({r = 1176260608}, 0, 1078530011)
        s:SET_ROTATION(0, 0, {r = 1176260608})
        s:MOV({r = 10000}, 0)
        s:JUMP_IF_NEQ({r = 10000}, 0, 112, 0)
        s:SET_ANGLE_VEL(0, 0, 1053364187)
        s:SET_CLOCK(0, 148)
        goto J148
        s:SET_ANGLE_VEL(0, 0, -1094119461)
        ::J148::
        s:NOP()
        s:at(30000)
        s:SET_CLOCK(0, 148)
        goto J148
        s:at(0)
        s:EXIT_HIDE()
    end,

    -- stream 37
    [37] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(11)
        s:SET_COLOR(12632256)
        s:SET_ALPHA(192)
        s:EXIT()
    end,
  },
  [2] = {
    texture = "texture_2.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 256 } },
    },
    scripts = {
      { index = 0, offset = 0 },
    },
    labels = {
      [0] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ANGLE_VEL(0, 0, 1023454717)
        s:SET_BLEND(1)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(120, 0, 160)
        s:SET_SCALE(1069547520, 1069547520)
        ::J92::
        s:at(10)
        s:NOP()
        s:at(-15526)
        s:SET_CLOCK(0, 92)
        goto J92
        s:at(0)
        s:EXIT_HIDE()
    end,
  },
  [3] = {
    texture = "texture_3.png",
    width = 256, height = 256, format = 5,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 256 } },
    },
    scripts = {
      { index = 0, offset = 0 },
    },
    labels = {
      [0] = {  },
    },

    -- stream 0
    [0] = function(s)
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:SET_ANGLE_VEL(0, 0, 994960174)
        s:SET_ALPHA(0)
        s:INTERP_ALPHA(30, 0, 160)
        s:SET_SCALE(1065353216, 1065353216)
        s:MOV({r = 10000}, 32)
        s:at(1)
        s:WAIT({r = 10000})
        s:EXIT_HIDE2()
    end,
  },
}
