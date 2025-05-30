if core.os == "windows" then
  package.path = package.path .. "./?/init.lua"
elseif core.os == "linux" then
  package.path = package.path .. ";./?/init.lua"
end

require("systems.draw_meshes")

local cube = core.create_mesh(core.shader.default, false)
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
local s = 0

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

local dh = core.load_font("res/fonts/dbl_homicide.ttf", 16);

event.on("@uidraw", function()
  local screenw, _ = core.get_screen_size()

  local st = ("s: %d"):format(s)
  local sw = dh:get_width(st)
  local sh = dh:get_height()

  dh:draw(screenw - sw - 5, 0, st)

  dh:draw(
    2, sh * 0,
    ("FPS: %d, %f ms"):format(core.get_fps(), 1/core.get_fps() * 1000))
  dh:draw(
    2, sh * 1,
    ("TPS: %d"):format(core.get_tps()))
  dh:draw(
    2, sh * 2,
    ("Entities: %d"):format(ecs.ent_count()))
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
