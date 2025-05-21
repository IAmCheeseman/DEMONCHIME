local event = {}

local registered_events = {}

function event.on(ev, cb)
  if not registered_events[ev] then
    registered_events[ev] = {}
  end
  table.insert(registered_events[ev], cb)
end

function event.call(ev, ...)
  if not registered_events[ev] then
    return
  end

  for _, cb in ipairs(registered_events[ev]) do
    cb(...)
  end
end

return event
