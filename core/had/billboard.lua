local event = require("event")
local ecs = require("ecs")

local billboards = ecs.query("billboard", xyz)

event.on("@draw", function()
  for ent in billboards.ents:iter() do
    local m = core.mat4_identity()
    m:translate(ent.x, ent.y, ent.z)
    ent.billboard:draw(m)
  end
end)

local billboard_t = {}
billboard_t.__index = billboard_t
billboard_t.__type = "billboard"

local function create_billboard(tex, scale)
  scale = scale or 0.005

  local tw, th = tex:get_size()
  local l = -tw / 2 * scale
  local r =  tw / 2 * scale
  local t = -th / 2 * scale
  local b =  th / 2 * scale
  local vertices = {
  -- pos       uv     color
    {l, t, 0,  0, 0,  1, 1, 1, 1},
    {r, t, 0,  1, 0,  1, 1, 1, 1},
    {l, b, 0,  0, 1,  1, 1, 1, 1},

    {r, t, 0,  1, 0,  1, 1, 1, 1},
    {l, b, 0,  0, 1,  1, 1, 1, 1},
    {r, b, 0,  1, 1,  1, 1, 1, 1},
  }

  local bb = setmetatable({}, billboard_t)
  bb.mesh = core.create_mesh(core.shader.billboard, true)
  bb.mesh:set_vertices(vertices)
  bb.mesh:finalize(true)
  bb.tex = tex
  bb.scale = scale
  return bb
end

function billboard_t:draw(m)
  self.mesh:draw(self.tex, m)
end

return create_billboard
