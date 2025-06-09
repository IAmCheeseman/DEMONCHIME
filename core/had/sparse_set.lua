local sparse_set_t = {}
sparse_set_t.__index = sparse_set_t
sparse_set_t.__type = "sparse_set"

local function create_sparse()
  local s = setmetatable({}, sparse_set_t)
  s.sparse = {}
  s.dense = {}
  return s
end

function sparse_set_t:len()
  return #self.dense
end

function sparse_set_t:get(v)
  return self.dense[self.sparse[v]]
end

function sparse_set_t:idx(i)
  return self.dense[i]
end

function sparse_set_t:add(v)
  if self.sparse[v] then
    return
  end

  table.insert(self.dense, v)
  self.sparse[v] = #self.dense
end

function sparse_set_t:rem(v)
  local idx = self.sparse[v]
  if not idx then
    error("attempt to remove value that doesn't exist in sparse set")
  end

  table.swaprem(self.dense, idx)

  local swapped = self.dense[idx]
  if swapped then
    self.sparse[swapped] = idx
  end

  self.sparse[v] = nil
end

function sparse_set_t:has(v)
  return not not self.sparse[v]
end

function sparse_set_t:clear()
  self.sparse = {}
  self.dense = {}
end

function sparse_set_t:ordered_iter()
  return ipairs(self.dense)
end

function sparse_set_t:iter()
  local k
  return function()
    k = next(self.sparse, k)
    return k
  end
end

return create_sparse
