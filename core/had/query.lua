local query_t = {}
query_t.__index = query_t

local function create_query(...)
  local q = setmetatable({}, query_t)
  q.filters = {...}
  q.ents = {}
  return q
end

function query_t:filter_ent(ent)
  for _, component in pairs(self.filters) do
    if not ent[component] then
      return false
    end
  end
  return true
end

function query_t:__len()
  return #self.ents
end

return create_query
