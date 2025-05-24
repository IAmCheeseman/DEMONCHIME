local create_sparse = require("sparse_set")
local create_query = require("query")
local event = require("event")

local add_queue = create_sparse()
local rem_queue = create_sparse()

local query_filters = {}
local queries = {}

local ents = create_sparse()
local ent_queries = {}

local ecs = {}

local function update_queries_with_comp(ent, comp)
  local filter2query = query_filters[comp]
  if filter2query then
    for _, query in ipairs(filter2query) do
      local passes = query:filter_ent(ent)

      if passes and not query.ents:has(ent) then
        query.ents:add(ent)
        ent_queries[ent]:add(query)
      elseif not passes and query.ents:has(ent) then
        query.ents:rem(ent)
        ent_queries[ent]:rem(query)
      end
    end
  end
end

local ent_mt = {
  __newindex = function(ent, k, v)
    rawset(ent, k, v)
    update_queries_with_comp(ent, k)
  end,
  rem = function(ent, k)
    if rawget(ent, k) == nil then
      return
    end
    rawset(ent, k, nil)
    update_queries_with_comp(ent, k)
  end
}
ent_mt.__index = ent_mt

function ecs.add_ent(ent)
  add_queue:add(ent)
end

function ecs.rem_ent(ent)
  if not ents:has(ent) then
    return
  end

  if add_queue:has(ent) then
    add_queue:rem(ent)
    return
  end
  rem_queue:add(ent)
end

local function add_ent_to_queries(ent)
  ent_queries[ent] = create_sparse()

  for comp_name, _ in pairs(ent) do
    local filter2query = query_filters[comp_name]
    if filter2query then
      for _, query in ipairs(filter2query) do
        if query:filter_ent(ent) then
          query.ents:add(ent)
          ent_queries[ent]:add(query)
        end
      end
    end
  end

end

local function flush_add()
  for _, ent in add_queue:ordered_iter() do
    event.call("@preentityadd", ent)

    setmetatable(ent, ent_mt)
    ents:add(ent)

    add_ent_to_queries(ent)

    event.call("@entityadd", ent)
  end
  add_queue:clear()
end

local function flush_rem()
  for _, ent in rem_queue:ordered_iter() do
    event.call("@entityrem", ent)

    ents:rem(ent)
    for query in ent_queries[ent]:iter() do
      query.ents:rem(ent)
    end
    ent_queries[ent] = nil

    setmetatable(ent, nil)
  end
  rem_queue:clear()
end

function ecs.step()
  flush_add()
  flush_rem()
end

function ecs.ent_count()
  return ents:len()
end

function ecs.query(...)
  local filters = {...}

  -- don't duplicate queries
  for _, query in ipairs(queries) do
    local same = #query.filters == #filters
    for _, comp in ipairs(filters) do
      if not query.filters[comp] then
        same = false
        break
      end
    end

    if same then
      return query
    end
  end

  -- no query that already exists and matches the filters; make new one 
  local query = create_query(...)
  table.insert(queries, query)

  for _, filter in ipairs(filters) do
    if not query_filters[filter] then
      query_filters[filter] = {}
    end
    table.insert(query_filters[filter], query)
  end

  -- this is okay to go through every entity; the only time queries should be
  -- made is before any entities exist, but i want them to work as you expect
  -- in all situations
  for _, ent in ipairs(ents) do
    if query:filter_ent(ent) then
      query.ents:add(ent)
      table.insert(ent_queries[ent], query)
    end
  end

  return query
end

return ecs
