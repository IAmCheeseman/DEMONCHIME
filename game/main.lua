if core.os == "windows" then
  package.path = package.path .. "./?/init.lua"
elseif core.os == "linux" then
  package.path = package.path .. ";./?/init.lua"
end

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

local r = 0
local rx = lcore.create_lerped_num()
local ry = lcore.create_lerped_num()
local rz = lcore.create_lerped_num()

function step()
  if core.is_key_down(core.key.ESCAPE) then
    core.close_engine()
  end

  if core.is_key_down(core.key.SPACE) then
    log_info(core.get_fps(), core.get_tps())
  end

  r = r + (1/30)
  rx:set(r * 0.25)
  ry:set(r)
  rz:set(r * 0.1)
end

function draw()
  shader:bind()

  local cubet = {
    x = 0,
    y = 0,
    z = -3,
    -- rx = 0,
    rx = rx:get(),--math.rad(-24),
    ry = ry:get(),--math.rad(45),
    rz = rz:get(),
    sx = 1,
    sz = 1,
    sy = 1,
  }

  local m = core.mat4_from_trans(cubet)

  local v = core.mat4_identity()

  local sw, sh = core.get_screen_size()
  local a = sw / sh

  local p = core.mat4_identity()
  p:perspective(45, a, 1, 100)

  shader:send_mat4("m", m)
  shader:send_mat4("v", v)
  shader:send_mat4("p", p)

  tex:bind(0)
  shader:sendi("tex0", 0)

  cube:draw()
end
