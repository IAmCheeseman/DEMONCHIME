local vec3 = {}

function vec3.len(x, y, z)
  return math.sqrt(x*x + y*y + z*z)
end

function vec3.sqlen(x, y, z)
  return x*x + y*y + z*z
end

function vec3.dot(x, y, z, xx, yy, zz)
  return x*xx + y*yy + z*zz
end

function vec3.cross(x, y, z, xx, yy, zz)
  return y * zz - z * yy, z * xx - x * zz, x * yy - y * xx
end

function vec3.normalize(x, y, z)
  if x == 0 and y == 0 and z == 0 then
    return 0, 0, 0
  end
  local l = vec3.len(x, y, z)
  return x / l, y / l, z / l
end

function vec3.floor(x, y, z)
  return math.floor(x), math.floor(y), math.floor(z)
end

function vec3.ceil(x, y, z)
  return math.ceil(x), math.ceil(y), math.ceil(z)
end

function vec3.round(x, y, z)
  return math.round(x), math.round(y), math.round(z)
end

-- WHY does it need to be like this :sob:
function vec3.rotate(x, y, z, ax, ay, az, r)
  local rc = math.cos(r)
  local rs = math.sin(r)

  local b00 = ax*ax + rc * (1 - ax*ax)
  local b11 = ay*ay + rc * (1 - ay*ay)
  local b22 = az*az + rc * (1 - az*az)

  local b01 = ax * ay * (1 - rc) - ax * rs
  local b10 = ax * ay * (1 - rc) + ax * rs
  local b02 = ax * az * (1 - rc) + ay * rs
  local b20 = ax * az * (1 - rc) - ay * rs
  local b12 = ay * az * (1 - rc) - ax * rs
  local b21 = ay * az * (1 - rc) + ax * rs

  return
    vec3.dot(b00, b01, b02, x, y, z),
    vec3.dot(b10, b11, b12, x, y, z),
    vec3.dot(b20, b21, b22, x, y, z)
end

function vec3.lerp(x, y, z, xx, yy, zz, t)
  return math.lerp(x, xx, t), math.lerp(y, yy, y), math.lerp(z, zz, t)
end

return vec3
