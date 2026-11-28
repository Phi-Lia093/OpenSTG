-- loading3: how the animation is put together and what it does.
--
-- One table per entry. Each names its texture, gives the rectangles into it, and
-- carries its instruction streams as functions of the script object.

return {
  [0] = {
    texture = "texture_0.png",
    width = 512, height = 512, format = 3,
    sprites = {
      { id = 0, rect = { 0, 0, 384, 448 } },
    },
    scripts = {
      { index = 0, offset = 0 },
    },
    labels = {
      [0] = { 1 },
    },

    -- stream 0
    [0] = function(s)
        local at = s:entry()
        if at == 1 then goto L1 end
        s:at(0)
        s:SET_ACTIVE_SPRITE(0)
        s:NOP22()
        s:SET_TRANSLATION(1107296256, 1098907648, 0)
        s:STOP()
        ::L1::
        s:LABEL(1, 0)
        s:EXIT_HIDE2()
    end,
  },
}
