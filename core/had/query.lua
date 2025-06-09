local create_sparse = require("sparse_set")

local query_t = {}
query_t.__index = query_t
query_t.__type = "query"

local function create_query(filters)
  local q = setmetatable({}, query_t)
  q.filters = filters
  for i, filter in ipairs(q.filters) do
    q.filters[filter] = i
  end

  q.ents = create_sparse()
  q.added_cb = {}
  q.rem_cb = {}
  return q
end

function query_t:emit_callback(t, ent)
  for _, cb in ipairs(t) do
    cb(ent)
  end
end

function query_t:on_ent_added(cb)
  table.insert(self.added_cb, cb)
end

function query_t:on_ent_rem(cb)
  table.insert(self.rem_cb, cb)
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
