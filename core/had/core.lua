util = require("util")

local old_type = type
function type(v)
  local mt = getmetatable(v)
  if mt and mt.__type then
    return mt.__type
  end
  return old_type(v)
end

require("tablex")
require("mathx")
require("stringx")

core.create_lerped_num = require("lerped_num")
core.create_billboard = require("billboard")
core.gfx = require("gfx")
core.vec2 = require("vec2")
core.vec3 = require("vec3")

event = require("event")
ecs = require("ecs")

local view_matrix = core.mat4_identity()

function core.get_view()
  return view_matrix
end

function core.set_view(mat)
  util.expect_args(mat, "mat4")
  view_matrix = mat
end

core.default_fmt = core.create_vert_fmt()
core.default_fmt:add_attr("float", 3) -- position
core.default_fmt:add_attr("float", 3) -- normal
core.default_fmt:add_attr("float", 2) -- uv
core.default_fmt:add_attr("float", 4) -- color

core.default_shader = core.load_shader(
  "res/vdefault.glsl", "res/fdefault.glsl")
core.billboard_shader = core.load_shader(
  "res/vbillboard.glsl", "res/fdefault.glsl")

require("run")
