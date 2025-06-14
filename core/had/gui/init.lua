local gui = {}

gui.create_region = require("gui.kirigami.init").Region

local rect_mesh = core.create_mesh("2d", false)

local region_vertices = {}
local ident = core.mat4_identity()

-- batch drawing regions together to minimize draw calls
function gui.queue_region_draw(r, cr, cg, cb, ca)
  cr = cr or 0
  cg = cg or cr
  cb = cb or cg
  ca = ca or 1

  table.insert(region_vertices, {r.x,     r.y,     0, 0,  cr, cg, cb, ca})
  table.insert(region_vertices, {r.x+r.w, r.y,     1, 0,  cr, cg, cb, ca})
  table.insert(region_vertices, {r.x,     r.y+r.h, 0, 1,  cr, cg, cb, ca})
  table.insert(region_vertices, {r.x+r.w, r.y,     1, 0,  cr, cg, cb, ca})
  table.insert(region_vertices, {r.x+r.w, r.y+r.h, 1, 1,  cr, cg, cb, ca})
  table.insert(region_vertices, {r.x,     r.y+r.h, 0, 1,  cr, cg, cb, ca})
end

function gui.flush_regions()
  rect_mesh:set_vertices(region_vertices)
  rect_mesh:finalize("dynamic")

  rect_mesh:draw(core.gfx.white_tex, ident)

  region_vertices = {}
end

return gui
