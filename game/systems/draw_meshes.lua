local r = 0
local vry = core.create_lerped_num()

event.on("@tick", function()
  -- r = r + math.rad(2)
  -- vry:set(r * 0.5)

  if core.is_key_down(core.key.left) then
    r = r + 0.1
  end
  if core.is_key_down(core.key.right) then
    r = r - 0.1
  end
  vry:set(r * 0.5)

  core.gfx.view.x = math.cos(vry:get()) * 10
  core.gfx.view.y = 10
  -- core.gfx.view.z = -10
  core.gfx.view.z = math.sin(vry:get()) * 10

  core.gfx.view.pitch = math.rad(-45)

  local yaw = core.vec2.angle(core.gfx.view.x, core.gfx.view.z) + math.pi / 2
  core.gfx.view.yaw = yaw
end)

local cubes = ecs.query("model", "trans_mat")
local tex = core.load_tex("res/textures/grass.png")
tex:set_filter(core.tex_filter.nearest, core.tex_filter.nearest)

event.on("@draw", function()
  for ent in cubes.ents:iter() do
    ent.model:draw(tex, ent.trans_mat)
  end
end)

local bb_tex = core.load_tex("res/textures/billboard.png")
local bb = core.create_billboard(bb_tex)

local bbent = {
  billboard = bb,
  x = 0,
  y = 0,
  z = 0,
}

local bbent2 = {
  billboard = bb,
  x = 0,
  y = 0,
  z = 0,
}

local bbent3 = {
  billboard = bb,
  x = 0,
  y = 0,
  z = 1,
}

ecs.add_ent(bbent)
ecs.add_ent(bbent2)
ecs.add_ent(bbent3)


local t = 0
local lt = core.create_lerped_num()

event.on("@tick", function()
  t = t + 0.1
  lt:set(t)
  bbent.x = math.cos(lt:get()) * 1.5
  bbent.z = math.sin(lt:get()) * 1.5
end)
