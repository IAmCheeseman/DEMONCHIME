local util = {}

function util.is(v, t)
  return type(v) == t
end

function util.try(f, ...)
  if f then
    return true, f(...)
  end
  return false, nil
end

function util.expect(v, t, i)
  if not util.is(v, t) then
    error("expected type '" .. t .. "' for argument " .. tostring(i))
  end
end

return util
