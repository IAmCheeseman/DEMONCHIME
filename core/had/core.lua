local path = "./?/init.lua"
-- why is this not consistent between operating systems?
if core.os ~= "windows" then
  path = ";" .. path
end
package.path = package.path .. path

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

xyz = {"x", "y", "z"}
vxyz = {"vx", "vy", "vz"}

core.create_lerped_num = require("lerped_num")
core.create_billboard = require("billboard")
core.gui = require("gui")
core.gfx = require("gfx")
core.vec2 = require("vec2")
core.vec3 = require("vec3")

event = require("event")
ecs = require("ecs")

require("run")
