local r = 0
local vry = core.create_lerped_num()

event.on("@tick", function()
  r = r + math.rad(2)
  vry:set(r * 0.5)
end)

local view_mat
local perspective_mat

event.on("@frame", function()
  local v_pitch = core.mat4_identity()
  v_pitch:rotate(math.rad(-45), 0, 0)
  local vt = core.mat4_identity()
  vt:translate(0, 7, -7)
  local v_yaw = core.mat4_identity()
  v_yaw:rotate(0, vry:get(), 0)
  view_mat = v_pitch:mult(vt:mult(v_yaw))

  local sw, sh = core.get_screen_size()
  local a = sw / sh
  perspective_mat = core.mat4_identity()
  perspective_mat:perspective(45, a, 1, 100)
end)

local cubes = ecs.query("model", "trans_mat")
local shader = core.load_shader("res/vdefault.glsl", "res/fdefault.glsl")
local tex = core.load_tex("res/textures/grass.png")
tex:set_filter(core.tex_filter.nearest, core.tex_filter.nearest)

event.on("@draw", function()
  shader:bind()
  shader:send_mat4("v", view_mat)
  shader:send_mat4("p", perspective_mat)

  tex:bind(0)
  shader:sendi("tex0", 0)

  for ent in cubes.ents:iter() do
    shader:send_mat4("m", ent.trans_mat)
    ent.model:draw()
  end
end)
