-- face_07_00: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 1, 1, 126, 510 } },
      { id = 1, rect = { 129, 1, 126, 510 } },
      { id = 2, rect = { 257, 1, 126, 510 } },
      { id = 3, rect = { 385, 1, 126, 510 } },
    },
    scripts = {},
    labels = {
    },
  },
  [1] = {
    texture = "texture_1.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 512 } },
      { id = 1, rect = { 256, 0, 256, 512 } },
    },
    scripts = {},
    labels = {
    },
  },
  [2] = {
    texture = "texture_2.png",
    width = 512, height = 512, format = 1,
    sprites = {
      { id = 0, rect = { 0, 0, 256, 512 } },
      { id = 1, rect = { 256, 0, 256, 512 } },
    },
    scripts = {},
    labels = {
    },
  },
}
