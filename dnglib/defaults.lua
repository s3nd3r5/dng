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

--[[
These are the default implementations of the override actions.
If you want to add custom logic into your game you can define a "proc.lua" in your map dir.

The following functions are also available via our C library:

void c_update_player_pos (dx, dy)
boolean c_player_can_move (dx, dy)
boolean c_enemy_can_move (id, dx, dy)
c_spawn_enemy (x, y)
c_destroy_enemy (id)
c_trigger_win()
c_trigger_loss(msg)
c_fatal(msg)

--]]
package.path = "./?.lua;" .. package.path
require "dnglib.constants";
local algs = require "dnglib.algs";
local hasLost = false;
local hasWon = false;
local hasIntro = false;

keys = {
    up = KEY_UP,
    down = KEY_DOWN,
    left = KEY_LEFT,
    right = KEY_RIGHT,
    quit = KEY_ESCAPE,
    start = KEY_SPACE,
    restart = KEY_SPACE,
}

--- setup random
--math.randomseed(os.time())

-- Checks if x,y equals for both objects
local function is_collision(a, b)
  return a.x == b.x and a.y == b.y
end


---@param pressedKey number
function onKeyPress(pressedKey)
    scene = c_get_scene()
    assert(type(scene) == "number", "scene is not a number")
    if scene == SCENE_INTRO then
        if pressedKey == keys.start then
            c_trigger_level_start();
        end
    elseif scene == SCENE_LEVEL then
        dx = 0
        dy = 0
        if pressedKey == keys.up then
            dy = -1
        elseif pressedKey == keys.left then
            dx = -1
        elseif pressedKey == keys.down then
            dy = 1
        elseif pressedKey == keys.right then
            dx = 1
        end

        c_move_player(dx, dy)
    elseif scene == SCENE_WIN or scene == SCENE_LOSS then
        if pressedKey == keys.restart then
            c_trigger_restart()
        end
    end
end

local diff_time = 0
---@param dt number
function onUpdate(dt)
    diff_time = diff_time + dt
    enemies = c_get_enemies()
    assert(type(enemies) == "table", "Enemies not a table")

    player = c_get_player_position()
    assert(type(player) == "table", "Player is not a table")
    
    treasures = c_get_treasures()
    assert(type(treasures) == "table", "treasures is not a table")

    door_keys = c_get_keys()
    assert(type(door_keys) == "table", "keys is not a table")

    doors = c_get_doors()
    assert(type(doors) == "table", "doors is not a table")

    map = c_get_map();
    assert(type(map) == "table", "map is not a table")

    for i, v in ipairs(enemies) do
        local next;
        if diff_time >= MOV_TIME then
            next = algs.pathfind(v, player, enemies, treasures, door_keys, doors, map)
        else
            next = { dx = 0, dy = 0 }
        end

        new_pos = c_move_enemy(v.id, next.dx, next.dy)
        assert(type(new_pos) == "table", "new_pos is not a table")
        if is_collision(new_pos, player)  then
            c_trigger_loss()
        end
        enemies[i] = new_pos -- update new position for pathfinding
    end

    for _, t in ipairs(treasures) do
        if is_collision(t, player) then
            c_score_treasure(t.id)
            if #treasures == 1 then
                c_trigger_win()
            end
        end
    end

    for _, k in ipairs(door_keys) do
      if is_collision(k, player) then
        c_take_key(k.id)
      end
    end

    for _, d in ipairs(doors) do
      if is_collision(d, player) then
        c_open_door(d.id)
      end
    end

    if diff_time > MOV_TIME then
        diff_time = 0
    end
end

function onWin()
    if hasWon == false then
        hasWon = true
        print("You WIN!!!!!!!!!")
    end
end

function onLoss()
    if hasLost == false then
        hasLost = true
        print("Oh no! You lost!")
    end
end

function onIntro()
    if hasIntro == false then
        hasIntro = true
        print([[
...................
..      dng      ..
...................
..    controls   ..
...................
.. start -- space..
.. move  -- wasd ..
.. quit  -- q    ..
...................
..      goal     ..
...................
.. Find treasure ..
.. Avoid enemies ..
...................
        ]])
    end
end

--- allow for requiring in other files for usage
return {
    onKeyPress = onKeyPress,
    onUpdate = onUpdate,
    onWin = onWin,
    onLoss = onLoss,
    onIntro = onIntro,
    keys = keys,
}
