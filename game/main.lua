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

local rect_mesh = core.create_mesh(core.shader._2d, false)

local function draw_region(r, cr, cg, cb, ca)
  cr = cr or 0
  cg = cg or cr
  cb = cb or cg
  ca = ca or 1

  rect_mesh:set_vertices({
    {r.x,     r.y,     0, 0,  cr, cg, cb, ca},
    {r.x+r.w, r.y,     1, 0,  cr, cg, cb, ca},
    {r.x,     r.y+r.h, 0, 1,  cr, cg, cb, ca},
    {r.x+r.w, r.y,     1, 0,  cr, cg, cb, ca},
    {r.x+r.w, r.y+r.h, 1, 1,  cr, cg, cb, ca},
    {r.x,     r.y+r.h, 0, 1,  cr, cg, cb, ca},
  })
  rect_mesh:finalize(false)

  local ident = core.mat4_identity()
  rect_mesh:draw(core.gfx.white_tex, ident)
end

event.on("@uidraw", function()
  local region = core.create_region(10, 50, 180, 180)
  local title, content = region:split_v(0.1, 0.9)

  title = title:pad_px(1)
  content = content:pad_px(1)

  draw_region(title)
  draw_region(content)

  title = title:pad_px(1)
  content = content:pad_px(1)

  local sh = dh:get_height()

  dh:draw(title.x, title.y - sh * 0.5, "Stats")

  dh:draw(
    content.x, content.y + sh * 0,
    ("FPS: %d, %f ms"):format(core.get_fps(), 1/core.get_fps() * 1000),
    0.2, 0.4, 1)
  dh:draw(
    content.x, content.y + sh * 1,
    ("TPS: %d"):format(core.get_tps()),
    1, 0.2, 1)
  dh:draw(
    content.x, content.y + sh * 2,
    ("Entities: %d"):format(ecs.ent_count()),
    0, 1, 0.4)
  dh:draw(content.x, content.y + sh * 3, ("s: %d"):format(s))
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
