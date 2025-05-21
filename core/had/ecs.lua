local create_query = require("query")

local ents = {}
local ent_info = {}
local queries = {}
local add_queue = {}
local rem_queue = {}

local ecs = {}

function ecs.add_ent(ent)
  -- table.find is O(n), maybe add entities to a set?
  if ent_info[ent] then
    return
  end
  table.insert(add_queue, ent)
  ent_info[ent] = {}
end

function ecs.rem_ent(ent)
  -- same as the comment in `add_ent`
  if not ent_info[ent] or ent_info[ent].rem_queued then
    return
  end
  table.insert(rem_queue, ent)
  ent_info[ent].rem_queued = true
end

local function flush_add()
  for i=#add_queue, 1, -1 do
    local ent = add_queue[i]

    table.insert(ents, ent)

    -- created when ent was added to the queue
    local info = ent_info[ent]
    info.index = #ents
    info.queries = {}

    -- find queries
    for _, query in ipairs(queries) do
      if query:filter_ent(ent) then
        table.insert(query.ents, ent)
        info.queries[query] = #query.ents
      end
    end

  end

  add_queue = {}
end

local function flush_rem()
  for i=#rem_queue, 1, -1 do
    local ent = rem_queue[i]
    local info = ent_info[ent]

    local rswapped = table.swaprem(ents, info.index)
    if ent_info[rswapped] then
      ent_info[rswapped].index = info.index
    end

    -- remove from queries
    for query, index in pairs(info.queries) do
      local qswapped = table.swaprem(query.ents, index)
      if ent_info[qswapped] then
        ent_info[qswapped].queries[query] = index
      end
    end

    ent_info[ent] = nil
  end

  rem_queue = {}
end

function ecs.ecs_step()
  flush_add()
  flush_rem()

  -- verify every entity is in the queries it should be in
  for _, ent in ipairs(ents) do
    local info = ent_info[ent]

    for _, query in ipairs(queries) do
      local belongs = query:filter_ent(ent)
      if not belongs and info.queries[query] then
        -- is in query, but doesn't belong
        local index = info.queries[query]
        local swapped = table.swaprem(query.ents, index)
        if ent_info[swapped] then
          ent_info[swapped].queries[query] = index
        end
        info.queries[query] = nil
      elseif belongs and not info.queries[query] then
        -- not in query, but should be
        table.insert(query.ents, ent)
        info.queries[query] = #query.ents
      end
    end
  end

end

-- once queries are created, they cannot be deleted
function ecs.query(...)
  local query = create_query(...)
  table.insert(queries, query)

  -- populate query
  for _, ent in ipairs(ents) do
    if query:filter_ent(ent) then
      table.insert(query.ents, ent)
      local info = ent_info[ent]
      info.queries[query] = #query.ents
    end
  end

  return query
end

return ecs
