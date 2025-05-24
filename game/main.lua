if core.os == "windows" then
  package.path = package.path .. "./?/init.lua"
elseif core.os == "linux" then
  package.path = package.path .. ";./?/init.lua"
end

require("systems.draw_meshes")

local fmt = core.create_vert_fmt()
fmt:add_attr("float", 3) -- position
fmt:add_attr("float", 3) -- normal
fmt:add_attr("float", 2) -- uv
fmt:add_attr("float", 4) -- color

local cube = core.create_mesh(fmt)
cube:set_vertices(require("cube"))
cube:finalize(true)

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
local s = 1

event.on("@keydown", function(key)
  if key == core.key.up then s = s + 1 end
  if key == core.key.down then s = s - 1 end
  s = math.max(s, 0)
end)

event.on("@tick", function()
  t = t + 1

  if t < 5 and not core.is_key_down(core.key.space) then
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
  if core.is_key_down(core.key.escape) then
    core.close_engine()
  end
end)

local metal_mania = core.load_font("res/fonts/dbl_homicide.ttf", 48);
local models = ecs.query("model")

event.on("@keydown", function(key, is_repeated)
  if is_repeated then
    return
  end

  if key == core.key.e then
    for ent in models.ents:iter() do
      ent:rem("trans_mat")
    end
  end

  if key == core.key.r then
    for ent in models.ents:iter() do
      ecs.rem_ent(ent)
    end
  end
end)

event.on("@uidraw", function()
  local st = ("s: %d"):format(s)
  local sw = metal_mania:get_width(st)

  metal_mania:draw(320 * 5 - sw - 5, 0, st)

  metal_mania:draw(
    12, 0,
    ("FPS: %d, %f ms"):format(core.get_fps(), 1/core.get_fps() * 1000),
    1, 0.1, 0.3)
  metal_mania:draw(
    12, 48,
    ("TPS: %d"):format(core.get_tps()),
    1, 0.2, 1)
  metal_mania:draw(
    12, 48 * 2,
    ("Entities: %d"):format(ecs.ent_count()),
    0, 1, 0.4)
end)

local fullscreen = core.fullscreen.none

event.on("@keydown", function(key, is_repeated)
  if key ~= core.key.f11 or is_repeated then
    return
  end

  if fullscreen == core.fullscreen.none then
    fullscreen = core.fullscreen.borderless
  else
    fullscreen = core.fullscreen.none
  end
  core.set_fullscreen(fullscreen)
end)
