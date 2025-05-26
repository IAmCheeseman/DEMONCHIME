local event = require("event")
local ecs = require("ecs")

function update(dt)
  event.call("@frame", dt)
end

function step()
  event.call("@pretick")
  ecs.step()
  event.call("@tick")
  event.call("@posttick")
end

function draw()
  event.call("@predraw")
  event.call("@draw")
  event.call("@transparentdraw")
end

function uidraw()
  event.call("@preuidraw")
  event.call("@uidraw")
end

function keydown(key, is_repeated)
  event.call("@keydown", key, is_repeated)
end

function keyup(key)
  event.call("@keyup", key)
end
