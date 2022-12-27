-- -----------------------------------------------------------------------
-- dng
-- ------------------------------------------------------------------------
-- Copyright (c) 2022 Steph Enders <steph@senders.io>
--
-- This software is provided 'as-is', without any express or implied
-- warranty. In no event will the authors be held liable for any damages
-- arising from the use of this software.
--
-- Permission is granted to anyone to use this software for any purpose,
-- including commercial applications, and to alter it and redistribute it
-- freely, subject to the following restrictions:
--
-- 1. The origin of this software must not be misrepresented; you must not
--    claim that you wrote the original software. If you use this software
--    in a product, an acknowledgment in the product documentation would
--    be appreciated but is not required.
--
-- 2. Altered source versions must be plainly marked as such, and must not
--    be misrepresented as being the original software.
--
-- 3. This notice may not be removed or altered from any source
--    distribution.
--
-- -----------------------------------------------------------------------

package.path="./?.lua;" .. package.path
require "dnglib.constants"
local Queue = require "dnglib.queue"

---@param map table
local function print_map(map)
    for i = 1, #map do
        row = map[i]
        line = ""
        for j = 1, #row do
            if row[j] == MAP_WALL then
                line = line .. "# "
            elseif row[j] == MAP_VISITED then
                line = line .. "x "
            else
                line = line .. "  "
            end
        end
        print(line)
    end
end

local function can_move(x, y, map)
    return map[y][x] == MAP_SPACE
end

---@param x number
---@param y number
---@param origin table
---@param map table
---@param queue table
---@return boolean found goal
local function push_moves(x, y, origin, map, queue, target_pos)
    map[y][x] = MAP_VISITED -- should be but just in case
    -- UP
    if can_move(x, y - 1, map) then
        pos = {
            x = x,
            y = y - 1,
            origin = origin
        }
        map[pos.y][pos.x] = MAP_VISITED
        if (pos.x == target_pos.x and pos.y == target_pos.y) then
            return true
        end
        queue:push(pos)
    end
    -- DOWN
    if can_move(x, y + 1, map) then
        pos = {
            x = x,
            y = y + 1,
            origin = origin
        }
        map[pos.y][pos.x] = MAP_VISITED
        if (pos.x == target_pos.x and pos.y == target_pos.y) then
            return true
        end
        queue:push(pos)
    end
    -- LEFT
    if can_move(x - 1, y, map) then
        pos = {
            x = x - 1,
            y = y,
            origin = origin
        }
        map[pos.y][pos.x] = MAP_VISITED
        if (pos.x == target_pos.x and pos.y == target_pos.y) then
            return true
        end
        queue:push(pos)
    end
    -- RIGHT
    if can_move(x + 1, y, map) then
        pos = {
            x = x + 1,
            y = y,
            origin = origin
        }
        map[pos.y][pos.x] = MAP_VISITED
        if (pos.x == target_pos.x and pos.y == target_pos.y) then
            return true
        end
        queue:push(pos)
    end
    return false
end

---
---@param start_pos table [x, y]
---@param target_pos table [x, y]
---@param map table 2D map array
---@return table best move to target [x, y]
---
local function best_effort_move(start_pos, target_pos, map)
  local diff_x = target_pos.x - start_pos.x
  local diff_y = target_pos.y - start_pos.y

  local move = { dx = 0, dy = 0 }

  if diff_x > 0 and can_move(start_pos.x + 1, start_pos.y, map) then
    move.dx = 1
    return move
  elseif diff_x < 0 and can_move(start_pos.x - 1, start_pos.y, map) then
    move.dx = -1
    return move
  end
  if diff_y > 0 and can_move(start_pos.x, start_pos.y + 1, map) then
    move.dy = 1
    return move
  elseif diff_y < 0 and can_move(start_pos.x, start_pos.y - 1, map) then
    move.dy = -1
    return move
  end

  -- return 0, 0 move
  return move
end


---
---@param start_pos table [x, y]
---@param target_pos table [x, y]
-- @param enemies list of enemy positions (cannot pass thru enemy)
-- @param treasures list of treasure positions (cannot pass thru treasure)
-- @param door_keys list of key positions (cannot pass thru treasure)
-- @param doors list of door positions (cannot pass thru treasure)
-- @param dirts list of dirt positions (player can pass, not enemy)
---@param map table 2D map array
---@return table best move to target [x, y]
---
local function pathfind(start_pos, target_pos, enemies, treasures, door_keys, doors, dirts, map)
    local queue = Queue:new()
    
    local visit_map = {}
    for k, v in ipairs(map) do
        row = {}
        for ik, iv in ipairs(v) do
            row[ik] = iv
        end
        visit_map[k] = row 
    end

    for _, e in ipairs(enemies) do
      if e ~= start_pos then
        visit_map[e.y][e.x] = MAP_WALL -- use wall value for impass
      end
    end
    for _, t in ipairs(treasures) do
      visit_map[t.y][t.x] = MAP_WALL -- use wall value for impass
    end
    for _, k in ipairs(door_keys) do
      visit_map[k.y][k.x] = MAP_WALL -- use wall value for impass
    end
    for _, d in ipairs(doors) do
      visit_map[d.y][d.x] = MAP_WALL -- use wall value for impass
    end
    for _, r in ipairs(dirts) do
      visit_map[r.y][r.x] = MAP_WALL -- use wall value for impass
    end

    -- since we mutate the visit_map let's calc this early if need be
    local best_effort = best_effort_move(start_pos, target_pos, visit_map)

    if (push_moves(start_pos.x, start_pos.y, nil, visit_map, queue, target_pos)) then
        return { dx = target_pos.x - start_pos.x, dy = target_pos.y - start_pos.y }
    end

    while queue:empty() ~= true do
        local pos = queue:pop()
        origin = pos.origin or { x = pos.x, y = pos.y }
        hit_target = push_moves(pos.x, pos.y, origin, visit_map, queue, target_pos)
        if hit_target then
            return { dx = origin.x - start_pos.x, dy = origin.y - start_pos.y }
        end
    end

    --- cannot find path - just move "towards" player
    return best_effort
end

return {
    pathfind = pathfind,
    print_map = print_map,
}
