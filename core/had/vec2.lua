local vec2 = {}

function vec2.len(x, y)
  return math.sqrt(x*x + y*y)
end

function vec2.sqlen(x, y)
  return x*x + y*y
end

function vec2.mlen(x, y)
  return math.abs(x) + math.abs(y)
end

function vec2.dot(x, y, xx, yy)
  return x*xx + y*yy
end

function vec2.cross(x, y, xx, yy)
  return x*xx - y*yy
end

function vec2.normalize(x, y)
  if x == 0 and y == 0 then
    return 0, 0
  end
  local l = vec2.len(x, y)
  return x / l, y / l
end

function vec2.angle(x, y)
  return math.atan2(y, x)
end

function vec2.floor(x, y)
  return math.floor(x), math.floor(y)
end

function vec2.ceil(x, y)
  return math.ceil(x), math.ceil(y)
end

function vec2.round(x, y)
  return math.round(x), math.round(y)
end

function vec2.rotate(r, x, y)
  local s = math.sin(r)
  local c = math.cos(r)
  return x * c - y * s, x * s + y * c
end

function vec2.lerp(x, y, xx, yy, t)
  return math.lerp(x, xx, t), math.lerp(y, yy, y)
end

return vec2
