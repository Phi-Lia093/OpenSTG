-- stg4bg2: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 256, format = 1,
    sprites = {
      { id = 0, rect = { 1, 1, 382, 254 } },
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
        s:SET_TRANSLATION(1107296256, 1098907648, 1065017672)
        s:NOP22()
        s:SET_SCALE(1065397135, 1071760263)
        s:SET_COLOR(0)
        s:at(512)
        s:INTERP_COLOR(600, 0, 16777215)
        s:SET_SCROLLVEL_X(947912704)
        s:at(3908)
        s:INTERP_COLOR(400, 0, 0)
        s:at(4308)
        s:EXIT()
    end,
  },
}
