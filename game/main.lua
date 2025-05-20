if core.os == "windows" then
  package.path = package.path .. "./?/init.lua"
elseif core.os == "linux" then
  package.path = package.path .. ";./?/init.lua"
end

core.set_tick_rate(30)

local lcore = require("core")

local tex = core.load_tex("res/textures/puzzle_cube.png")
tex:set_filter(core.tex_filter.NEAREST, core.tex_filter.NEAREST)

local fmt = core.create_vert_fmt()
fmt:add_attr("float", 3) -- position
fmt:add_attr("float", 3) -- normal
fmt:add_attr("float", 2) -- uv
fmt:add_attr("float", 4) -- color

local shader = core.load_shader("res/vdefault.glsl", "res/fdefault.glsl")

local cube = core.create_mesh(fmt)
local vp = {
  {0.5, 0.5, -0.5},
  {0.5, -0.5, -0.5},
  {0.5, 0.5, 0.5},
  {0.5, -0.5, 0.5},
  {-0.5, 0.5, -0.5},
  {-0.5, -0.5, -0.5},
  {-0.5, 0.5, 0.5},
  {-0.5, -0.5, 0.5},
}

-- 1 2
-- |/|
-- 3 4

-- 1 2 3  1 3 4
cube:set_vertices({
  {vp[7][1], vp[7][2], vp[7][3],  0, 1, 0,  1, 1,  1, 0.5, 0, 1},
  {vp[5][1], vp[5][2], vp[5][3],  0, 1, 0,  1, 0,  1, 0.5, 0, 1},
  {vp[1][1], vp[1][2], vp[1][3],  0, 1, 0,  0, 0,  1, 0.5, 0, 1}, -- orange OK
  {vp[3][1], vp[3][2], vp[3][3],  0, 1, 0,  0, 1,  1, 0.5, 0, 1},
  {vp[7][1], vp[7][2], vp[7][3],  0, 1, 0,  1, 1,  1, 0.5, 0, 1},
  {vp[1][1], vp[1][2], vp[1][3],  0, 1, 0,  0, 0,  1, 0.5, 0, 1},

  -- 4 3 7 8
  {vp[7][1], vp[7][2], vp[7][3],  0, 0, 1,  0, 1,  0, 0, 1, 1}, -- red
  {vp[3][1], vp[3][2], vp[3][3],  0, 0, 1,  1, 1,  0, 0, 1, 1}, -- 1
  {vp[4][1], vp[4][2], vp[4][3],  0, 0, 1,  1, 0,  0, 0, 1, 1}, -- 2
  {vp[8][1], vp[8][2], vp[8][3],  0, 0, 1,  0, 0,  0, 0, 1, 1}, -- 3
  {vp[7][1], vp[7][2], vp[7][3],  0, 0, 1,  0, 1,  0, 0, 1, 1}, -- 4
  {vp[4][1], vp[4][2], vp[4][3],  0, 0, 1,  1, 0,  0, 0, 1, 1}, -- 5

  -- 8 7 5 6
  {vp[5][1], vp[5][2], vp[5][3],  -1, 0, 0,  0, 1,  1, 1, 0, 1}, -- green
  {vp[7][1], vp[7][2], vp[7][3],  -1, 0, 0,  1, 1,  1, 1, 0, 1},
  {vp[8][1], vp[8][2], vp[8][3],  -1, 0, 0,  1, 0,  1, 1, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  -1, 0, 0,  0, 0,  1, 1, 0, 1},
  {vp[5][1], vp[5][2], vp[5][3],  -1, 0, 0,  0, 1,  1, 1, 0, 1},
  {vp[8][1], vp[8][2], vp[8][3],  -1, 0, 0,  1, 0,  1, 1, 0, 1},

  -- 6 2 4 8
  {vp[4][1], vp[4][2], vp[4][3],  0, -1, 0,  0, 1,  1, 0, 0, 1},
  {vp[2][1], vp[2][2], vp[2][3],  0, -1, 0,  1, 1,  1, 0, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  0, -1, 0,  1, 0,  1, 0, 0, 1}, -- blue OK
  {vp[8][1], vp[8][2], vp[8][3],  0, -1, 0,  0, 0,  1, 0, 0, 1},
  {vp[4][1], vp[4][2], vp[4][3],  0, -1, 0,  0, 1,  1, 0, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  0, -1, 0,  1, 0,  1, 0, 0, 1},

  -- 2 1 3 4
  {vp[3][1], vp[3][2], vp[3][3],  1, 0, 0,  0, 1,  1, 1, 1, 1}, -- white
  {vp[1][1], vp[1][2], vp[1][3],  1, 0, 0,  1, 1,  1, 1, 1, 1},
  {vp[2][1], vp[2][2], vp[2][3],  1, 0, 0,  1, 0,  1, 1, 1, 1},
  {vp[4][1], vp[4][2], vp[4][3],  1, 0, 0,  0, 0,  1, 1, 1, 1},
  {vp[3][1], vp[3][2], vp[3][3],  1, 0, 0,  0, 1,  1, 1, 1, 1},
  {vp[2][1], vp[2][2], vp[2][3],  1, 0, 0,  1, 0,  1, 1, 1, 1},

  -- 6 5 1 2
  {vp[1][1], vp[1][2], vp[1][3],  0, 0, -1,  0, 1,  0, 1, 0, 1},
  {vp[5][1], vp[5][2], vp[5][3],  0, 0, -1,  1, 1,  0, 1, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  0, 0, -1,  1, 0,  0, 1, 0, 1}, -- yellow OK
  {vp[2][1], vp[2][2], vp[2][3],  0, 0, -1,  0, 0,  0, 1, 0, 1},
  {vp[1][1], vp[1][2], vp[1][3],  0, 0, -1,  0, 1,  0, 1, 0, 1},
  {vp[6][1], vp[6][2], vp[6][3],  0, 0, -1,  1, 0,  0, 1, 0, 1},
})
cube:finalize(true)

local cubet = {
  x = 0,
  y = 0,
  z = 0,
  rx = 0,
  ry = 0,
  rz = 0,
  sx = 1,
  sz = 1,
  sy = 1,
}

local model_mat4s = {}
local prng = core.create_prng()
for _=1, 4000 do
  cubet.x = prng:rangef(-15, 15)
  cubet.y = prng:rangef(-2, 2)
  cubet.z = prng:rangef(-15, 15)
  cubet.rx = prng:nextf() * math.pi * 2
  cubet.ry = prng:nextf() * math.pi * 2
  cubet.rz = prng:nextf() * math.pi * 2
  table.insert(model_mat4s, core.mat4_from_trans(cubet))
end

local r = 0
local vry = lcore.create_lerped_num()

function step()
  if core.is_key_down(core.key.ESCAPE) then
    core.close_engine()
  end

  if core.is_key_down(core.key.SPACE) then
    log_info(
      ("%d FPS, %d TPS, %f ms"):format(
        core.get_fps(),
        core.get_tps(),
        (1 / core.get_fps()) * 1000))
  end

  r = r + core.get_tick_rate_ms()
  vry:set(r * 0.5)
end

function draw()
  local v_pitch = core.mat4_identity()
  v_pitch:rotate(math.rad(-45), 0, 0)
  local vt = core.mat4_identity()
  vt:translate(0, 7.5, -7.5)
  local v_yaw = core.mat4_identity()
  v_yaw:rotate(0, vry:get(), 0)
  local v = v_pitch:mult(vt:mult(v_yaw))

  local sw, sh = core.get_screen_size()
  local a = sw / sh
  local p = core.mat4_identity()
  p:perspective(45, a, 1, 100)

  shader:bind()
  shader:send_mat4("v", v)
  shader:send_mat4("p", p)

  tex:bind(0)
  shader:sendi("tex0", 0)

  for _, m in ipairs(model_mat4s) do
    shader:send_mat4("m", m)
    cube:draw()
  end
end
