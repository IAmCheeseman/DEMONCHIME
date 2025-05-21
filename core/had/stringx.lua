local str_mt = getmetatable("")
local str__index = str_mt.__index

function str_mt.__index(s, i)
  if type(i) == "number" then
    return s:sub(i, i)
  end
  return str__index[i]
end
