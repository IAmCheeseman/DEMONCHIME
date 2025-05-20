local lerped_num_t = {}
lerped_num_t.__index = lerped_num_t

local function create_lerped_num(init)
  local n = setmetatable({}, lerped_num_t)
  n.val = init or 0
  n.prev = n.val
  return n
end

function lerped_num_t:get()
  return core.interpolate(self.prev, self.val)
end

function lerped_num_t:getFinal()
  return self.val
end

function lerped_num_t:set(v)
  self.prev = self.val
  self.val = v
end

return create_lerped_num
