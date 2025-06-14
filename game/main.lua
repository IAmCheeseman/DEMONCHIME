require("systems.draw_meshes")

local cube = core.create_mesh("default", false)
cube:set_vertices(require("cube"))
cube:finalize("static")

local cubet = {
  x = 0,
  y = 0,
  z = 0,
  rx = 0,
  ry = 0,
  rz = 0,
  sx = 1,
  sz = 1,
  sy = 1,
}

local prng = core.create_prng()
local t = 0
local s = 0

event.on("@keydown", function(key)
  if key == "up" then s = s + 1 end
  if key == "down" then s = s - 1 end
  s = math.max(s, 0)
end)

event.on("@tick", function()
  t = t + 1

  if t < 5 and not core.is_key_down("space") then
    return
  end

  t = 0

  for _=1, s do
    cubet.x = prng:rangef(-5, 5)
    cubet.y = prng:rangef(-2, 2)
    cubet.z = prng:rangef(-5, 5)
    cubet.rx = prng:nextf() * math.pi * 2
    cubet.ry = prng:nextf() * math.pi * 2
    cubet.rz = prng:nextf() * math.pi * 2

    local cube_ent = {}
    cube_ent.model = cube
    cube_ent.trans_mat = core.mat4_from_trans(cubet)
    ecs.add_ent(cube_ent)
  end
end)

event.on("@tick", function()
  if core.is_key_down("escape") then
    core.close_engine()
  end
end)

local models = ecs.query("model")

event.on("@keydown", function(key, is_repeated)
  if is_repeated then
    return
  end

  if key == "e" then
    for ent in models.ents:iter() do
      ent:rem("trans_mat")
    end
  end

  if key == "r" then
    for ent in models.ents:iter() do
      ecs.rem_ent(ent)
    end
  end
end)

local dh = core.load_font("res/fonts/dbl_homicide.ttf", 16);

local function draw_text(x, y, text, r, g, b)
end

event.on("@uidraw", function()
  local texts = {
    {
      str = "Stats",
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
    {
      str = ("s: %d"):format(s),
      color = {1, 1, 1},
    },
  }

  local region = core.gui.create_region(
    2, 2,
    180, dh:get_height() * #texts + 2)

  local text_boxes = region:rows(#texts)
  for i, box in ipairs(text_boxes) do
    text_boxes[i] = box:pad_px(1)
    core.gui.queue_region_draw(text_boxes[i])
  end
  core.gui.flush_regions()

  for i, text in ipairs(texts) do
    local box = text_boxes[i]
    local y = box.y - box.h + dh:get_height() / 2
    dh:draw(box.x, y, text.str, unpack(text.color))
  end
end)

local fullscreen = "none"

event.on("@keydown", function(key, is_repeated)
  if key ~= "f11" or is_repeated then
    return
  end

  if fullscreen == "none" then
    fullscreen = "borderless"
  else
    fullscreen = "none"
  end
  core.set_fullscreen(fullscreen)
end)
