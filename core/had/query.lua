local create_sparse = require("sparse_set")

local query_t = {}
query_t.__index = query_t

local function create_query(...)
  local q = setmetatable({}, query_t)
  q.filters = {...}
  for i, filter in ipairs(q.filters) do
    q.filters[filter] = i
  end

  q.ents = create_sparse()
  return q
end

function query_t:filter_ent(ent)
  for _, component in ipairs(self.filters) do
    if not rawget(ent, component) then
      return false
    end
  end
  return true
end

function query_t:len()
  return self.ents:len()
end

return create_query
