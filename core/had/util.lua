local util = {}

function util.is(v, t)
  return type(v) == t
end

function util.try(f, ...)
  if f then
    return true, f(...)
  end
  return false, nil
end

function util.expect_args(...)
  local args = {...}

  local i = 1
  local argi = 1
  while i < #args do
    if args[i+1] ~= "any" then
      if type(args[i]) ~= args[i+1] then
        error("expected type '" .. args[i+1] .. "' for argument " .. argi, 2)
      end
    end
    i = i + 2
    argi = argi + 1
  end
end

return util
