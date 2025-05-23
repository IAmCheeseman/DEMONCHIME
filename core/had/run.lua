local event = require("event")
local ecs = require("ecs")

local ecs_step = ecs.ecs_step
ecs.ecs_step = nil

function update(dt)
  event.call("@frame", dt)
end

function step()
  ecs_step()
  event.call("@tick")
end

function draw()
  event.call("@draw")
end

function uidraw()
  event.call("@uidraw")
end

function keydown(key, is_repeated)
  event.call("@keydown", key, is_repeated)
end

function keyup(key)
  event.call("@keyup", key)
end
