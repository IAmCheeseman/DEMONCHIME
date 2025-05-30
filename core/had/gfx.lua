local event = require("event")

local gfx = {}

gfx.view = {
  x = 0,
  y = 0,
  z = 0,
  yaw = 0,
  pitch = 0,
}

event.on("@frame", function()
  local v_yaw = core.mat4_identity()
  v_yaw:rotate(0, gfx.view.yaw, 0)
  local v_pitch = core.mat4_identity()
  v_pitch:rotate(gfx.view.pitch, 0, 0)

  local v_rot = v_pitch:mult(v_yaw)
  local v_pos = core.mat4_identity()
  v_pos:translate(gfx.view.x, gfx.view.y, gfx.view.z)
  local view = v_rot:mult(v_pos)

  core.set_view(view)

  local sw, sh = core.get_screen_size()
  local a = sw / sh

  local p = core.mat4_identity()
  p:perspective(45, a, 0.01, 100)
  core.set_projection(p)

  local o = core.mat4_identity()
  o:ortho(0, sw, 0, sh, 0, 100)
  core.set_2d_projection(o)
end)

return gfx
