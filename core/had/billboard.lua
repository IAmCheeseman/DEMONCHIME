local billboard_t = {}
billboard_t.__index = billboard_t
billboard_t.__type = "billboard"

local fmt = core.create_vert_fmt()
fmt:add_attr("float", 3) -- position
fmt:add_attr("float", 2) -- uv

local function create_billboard(tex, scale)
  scale = scale or 0.005

  local tw, th = tex:get_size()
  local l = -tw / 2 * scale
  local r =  tw / 2 * scale
  local t = -th / 2 * scale
  local b =  th / 2 * scale
  local vertices = {
    {l, t, 0,  0, 0},
    {r, t, 0,  1, 0},
    {l, b, 0,  0, 1},

    {r, t, 0,  1, 0},
    {l, b, 0,  0, 1},
    {r, b, 0,  1, 1},
  }

  local bb = setmetatable({}, billboard_t)
  bb.mesh = core.create_mesh(fmt)
  bb.mesh:set_vertices(vertices)
  bb.mesh:finalize(true)
  bb.tex = tex
  bb.scale = scale
  return bb
end

function billboard_t:draw(m, v, p)
  local tw, th = self.tex:get_size()
  self.tex:bind(0)

  core.billboard_shader:bind()
  core.billboard_shader:send_mat4("m", m)
  core.billboard_shader:send_mat4("v", v)
  core.billboard_shader:send_mat4("p", p)
  core.billboard_shader:sendf("scale", self.scale)
  core.billboard_shader:sendf("tex_size", tw, th)
  core.billboard_shader:sendi("tex0", 0)

  self.mesh:draw()
end

return create_billboard
