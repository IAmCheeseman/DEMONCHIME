function math.round(x)
  return math.floor(x + 0.5)
end

function math.snap(x, s)
  return math.round(x / s) * s
end

function math.clamp(x, min, max)
  return math.min(math.max(x, min), max)
end

function math.lerp(a, b, t)
  return (b - a) * t + a
end
