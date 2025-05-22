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

event.on("@tick", function()
  t = t + 1

  if t < 5 then
    return
  end

  t = 0

  cubet.x = prng:rangef(-15, 15)
  cubet.y = prng:rangef(-2, 2)
  cubet.z = prng:rangef(-15, 15)
  cubet.rx = prng:nextf() * math.pi * 2
  cubet.ry = prng:nextf() * math.pi * 2
  cubet.rz = prng:nextf() * math.pi * 2

  local cube_ent = {}
  cube_ent.model = cube
  cube_ent.trans_mat = core.mat4_from_trans(cubet)
  ecs.add_ent(cube_ent)
end)

event.on("@tick", function()
  if core.is_key_down(core.key.ESCAPE) then
    core.close_engine()
  end
end)

event.on("@tick", function()
  if core.is_key_down(core.key.SPACE) then
    log_info(
      ("%d FPS, %d TPS, %f ms"):format(
        core.get_fps(),
        core.get_tps(),
        (1 / core.get_fps()) * 1000))
  end
end)

local metal_mania = core.load_font("res/fonts/metal_mania.ttf", 48);

event.on("@uidraw", function()
  metal_mania:draw(0, 0, ("FPS: %d"):format(core.get_fps()), 1, 0, 0)
  metal_mania:draw(0, 48, ("TPS: %d"):format(core.get_tps()), 1, 0, 0)
end)
