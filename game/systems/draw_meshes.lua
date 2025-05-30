local r = 0
local vry = core.create_lerped_num()

event.on("@tick", function()
  r = r + math.rad(2)
  vry:set(r * 0.5)

  core.gfx.view.x = math.cos(vry:get()) * 10
  core.gfx.view.y = 10
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

ecs.add_ent(bbent)

local t = core.create_lerped_num()
event.on("@tick", function()
  t:set(t.val + 0.05)
  bbent.x = math.cos(t:get())
  bbent.z = math.sin(t:get())
end)
