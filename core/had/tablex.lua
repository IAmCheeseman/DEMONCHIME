function table.swaprem(t, i)
  t[i] = t[#t]
  t[#t] = nil
  return t[i]
end

function table.find(t, x)
  for i, v in ipairs(t) do
    if v == x then
      return i
    end
  end
  return nil
end

function table.addiv(dst, src)
  for _, v in ipairs(src) do
    table.insert(dst, v)
  end
end

function table.copykv(dst, src)
  for k, v in ipairs(src) do
    -- don't copy over indices
    if not util.is(k, "number") then
      dst[k] = v
    end
  end
end
