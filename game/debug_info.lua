local show = false

event.on("@uidraw", function()
  if not show then
    return
  end

  local texts = {
    {
      str = "Debug Info",
      color = {1, 1, 1},
    },
    {
      str = ("FPS: %d, %f ms"):format(core.get_fps(), 1/core.get_fps() * 1000),
      color = {0.2, 0.4, 1},
    },
    {
      str = ("TPS: %d"):format(core.get_tps()),
      color = {1, 0.2, 1},
    },
    {
      str = ("Entities: %d"):format(ecs.ent_count()),
      color = {0, 1, 0.4},
    },
    {
      str = ("Draw Calls: %d"):format(core.get_draw_call_count()),
      color = {1, 1, 0.4},
    },
  }

  local font = core.gui.font

  local region = core.gui.create_region(
    2, 2,
    180, font:get_height() * #texts + 2)

  local text_boxes = region:rows(#texts)
  for i, box in ipairs(text_boxes) do
    text_boxes[i] = box:pad_px(1)
    core.gui.queue_region_draw(text_boxes[i], 0, 0, 0, 0.5)
  end
  core.gui.flush_regions()

  for i, text in ipairs(texts) do
    local box = text_boxes[i]
    local y = box.y - box.h + font:get_height() / 2
    font:draw(box.x, y, text.str, unpack(text.color))
  end
end)

event.on("@keydown", function(key, is_repeated)
  if key ~= "f3" or is_repeated then
    return
  end

  show = not show
end)
