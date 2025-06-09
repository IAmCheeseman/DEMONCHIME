local region_t = {}
region_t.__index = region_t
region_t.__type = "region"

local function create_region(x, y, w, h)
  local r = setmetatable({}, region_t)
  r.x = x
  r.y = y
  r.w = w
  r.h = h
  return r
end

function region_t:split_h(...)
  local splits = {...}

  local regions = {}

  local x = self.x
  local tw = 0
  for _, v in ipairs(splits) do
    local w = self.w * v
    local split = create_region(x, self.y, w, self.h)
    x = x + w
    tw = tw + w
    table.insert(regions, split)
  end

  table.insert(regions, create_region(x, self.y, self.w - tw, self.h))

  return unpack(regions)
end

function region_t:split_v(...)
  local splits = {...}

  local regions = {}

  local y = self.y
  local th = 0
  for _, v in ipairs(splits) do
    local h = self.h * v
    local split = create_region(self.x, y, self.w, h)
    y = y + h
    th = th + h
    table.insert(regions, split)
  end

  table.insert(regions, create_region(self.x, y, self.w, self.h - th))

  return unpack(regions)
end

function region_t:shrink_by(...)
  local args = {...}
  local l, r, t, b
  if #args == 1 then
    l, r, t, b = args[1], args[1], args[1], args[1]
  elseif #args == 2 then
    l, r, t, b = args[1], args[1], args[2], args[2]
  elseif #args == 4 then
    l, r, t, b = args[1], args[2], args[3], args[4]
  else
    error("expected 1, 2, or 4 arguments to 'shrink_by'")
  end

  return create_region(
    self.x + l,
    self.y + t,
    self.w - r,
    self.h - b)
end

return create_region
