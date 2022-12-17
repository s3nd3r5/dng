//========================================================================
// dng
//------------------------------------------------------------------------
// Copyright (c) 2022 Steph Enders <steph@senders.io>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#ifndef DNG_CAPI_H
#define DNG_CAPI_H

#include "Level.h"
#include "Scene.h"
#include "lua.hpp"
#include <memory>

extern std::shared_ptr<Level> lvl;
extern Scene scene;

void push_position_table(lua_State *L, std::vector<Pos> positions) {
  lua_createtable(L, int(positions.size()), 0);
  int idx = 0;

  for (auto &pos : positions) {
    lua_pushnumber(L, ++idx);
    lua_createtable(L, 0, 3);
    lua_pushnumber(L, pos.token);
    lua_setfield(L, -2, "token");
    lua_pushnumber(L, pos.id);
    lua_setfield(L, -2, "id");
    lua_pushnumber(L, pos.x + 1);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, pos.y + 1);
    lua_setfield(L, -2, "y");
    lua_settable(L, -3);
  }
}

/*
 * c_get_player_position(int x, int y)
 */
static int c_get_player_position(lua_State *L) {
  lua_createtable(L, 0, 2);
  lua_pushnumber(L, lvl->player.y + 1);
  lua_setfield(L, -2, "y");
  lua_pushnumber(L, lvl->player.x + 1);
  lua_setfield(L, -2, "x");
  return 1;
}

/*
 * c_move_player(int dx, int dy)
 */
static int c_move_player(lua_State *L) {
  // stack ordering
  int dy = static_cast<int>(lua_tonumber(L, -1));
  int dx = static_cast<int>(lua_tonumber(L, -2));

  if (lvl->playerCanStep(dx, dy)) {
    lvl->player.x += dx;
    lvl->player.y += dy;
  }

  lua_createtable(L, 0, 2);
  lua_pushnumber(L, lvl->player.y + 1);
  lua_setfield(L, -2, "y");
  lua_pushnumber(L, lvl->player.x + 1);
  lua_setfield(L, -2, "x");

  return 1;
}

/*
 * c_move_enemy(int id, int dx, int dy)
 */
static int c_move_enemy(lua_State *L) {
  // stack ordering
  int dy = static_cast<int>(lua_tonumber(L, -1));
  int dx = static_cast<int>(lua_tonumber(L, -2));
  int id = static_cast<int>(lua_tonumber(L, -3));

  int i = lvl->getEnemyIndex(id);
  // guard against enemy not found
  if (i == -1) {
    return 1;
  }

  if (lvl->enemyCanStep(lvl->enemyPositions[i], dx, dy)) {
    lvl->enemyPositions[i].x += dx;
    lvl->enemyPositions[i].y += dy;
  }
  lua_createtable(L, 0, 3);
  lua_pushnumber(L, lvl->enemyPositions[i].y + 1);
  lua_setfield(L, -2, "y");
  lua_pushnumber(L, lvl->enemyPositions[i].x + 1);
  lua_setfield(L, -2, "x");
  lua_pushnumber(L, lvl->enemyPositions[i].id);
  lua_setfield(L, -2, "id");

  return 1;
}

/*
 * c_get_enemies()
 */
static int c_get_enemies(lua_State *L) {
  push_position_table(L, lvl->enemyPositions);
  return 1;
}

/*
 * c_spawn_enemy(int x, int y)
 */
static int c_spawn_enemy(lua_State *L) { return 1; }

/*
 * c_destroy_enemy(int id)
 */
static int c_destroy_enemy(lua_State *L) { return 1; }

/*
 * c_get_map()
 */
static int c_get_map(lua_State *L) {
  lua_createtable(L, int(lvl->map.size()), 0);
  int idx = 0;
  for (auto &vec : lvl->map) {
    lua_pushnumber(L, ++idx);
    lua_createtable(L, int(vec.size()), 0);
    int inner_idx = 0;
    for (auto &c : vec) {
      lua_pushnumber(L, ++inner_idx);
      lua_pushnumber(L, c == WALL_SPACE ? 1 : 0);
      lua_rawset(L, -3);
    }
    lua_rawset(L, -3);
  }
  return 1;
}

/*
 * c_trigger_level_start()
 */
static int c_trigger_level_start(lua_State *L) {
  scene = Scene::LEVEL;
  lua_pushboolean(L, true);
  return 1;
}
/*
 * c_trigger_win()
 */
static int c_trigger_win(lua_State *L) {
  scene = Scene::WIN;
  lua_pushboolean(L, true);
  return 1;
}

/*
 * c_trigger_loss()
 */
static int c_trigger_loss(lua_State *L) {
  scene = Scene::LOSS;
  lua_pushboolean(L, true);
  return 1;
}

static int c_get_scene(lua_State *L) {
  lua_pushnumber(L, scene);
  return 1;
}

static int c_get_treasures(lua_State *L) {
  push_position_table(L, lvl->treasurePositions);
  return 1;
}

static int c_score_treasure(lua_State *L) {
  int id = static_cast<int>(lua_tonumber(L, -1));

#ifdef __APPLE__
  for (int i = 0; i < lvl->treasurePositions.size(); i++) {
    if (lvl->treasurePositions[i].id == id) {
      lvl->treasurePositions.erase(lvl->treasurePositions.begin() + i);
      break;
    }
  }
#else
  erase_if(lvl->treasurePositions, [id](Pos t) { return t.id == id; });
#endif

  return 1;
}

static int c_trigger_restart(lua_State *L) {
  lvl->reset();
  scene = Scene::INTRO;
  return 1;
}

static int c_get_doors(lua_State *L) {
  push_position_table(L, lvl->doorPositions);
  return 1;
}

/**
 * c_open_door(id)
 * if you have a key it will open the door and use the key
 */
static int c_open_door(lua_State *L) {
  int id = static_cast<int>(lua_tonumber(L, -1));
  bool can_open = false;
  for (int i = 0; i < lvl->doorPositions.size(); i++) {
    if (lvl->doorPositions[i].id == id) {
      char c = lvl->doorPositions[i].token;
      for (int k = lvl->heldKeys.size() - 1; k >= 0; k--) {
        char mapped_door = KEY_DOOR_MAPPING[lvl->heldKeys[k] - KEY_TKN_START];
        if (mapped_door == c) {
          can_open = true;
          // erase key
          lvl->heldKeys.erase(lvl->heldKeys.begin() + k);
          lvl->doorPositions.erase(lvl->doorPositions.begin() + i);
          lvl->map[lvl->doorPositions[i].y][lvl->doorPositions[i].x] =
              BLANK_SPACE;
          break;
        }
      }
    }
  }

  return 1;
}

static int c_get_keys(lua_State *L) {
  push_position_table(L, lvl->keyPositions);
  return 1;
}

static int c_take_key(lua_State *L) {
  int id = static_cast<int>(lua_tonumber(L, -1));
  for (int i = 0; i < lvl->keyPositions.size(); i++) {
    if (lvl->keyPositions[i].id == id) {
      lvl->heldKeys.push_back(lvl->keyPositions[i].token);
      lvl->keyPositions.erase(lvl->keyPositions.begin() + i);
      break;
    }
  }
  return 1;
}

// not for lua use
void init_c_api(lua_State *L) {
  lua_register(L, "c_move_player", c_move_player);
  lua_register(L, "c_move_enemy", c_move_enemy);
  lua_register(L, "c_spawn_enemy", c_spawn_enemy);
  lua_register(L, "c_destroy_enemy", c_destroy_enemy);
  lua_register(L, "c_get_enemies", c_get_enemies);
  lua_register(L, "c_get_player_position", c_get_player_position);
  lua_register(L, "c_get_map", c_get_map);
  lua_register(L, "c_trigger_level_start", c_trigger_level_start);
  lua_register(L, "c_trigger_win", c_trigger_win);
  lua_register(L, "c_trigger_loss", c_trigger_loss);
  lua_register(L, "c_get_scene", c_get_scene);
  lua_register(L, "c_score_treasure", c_score_treasure);
  lua_register(L, "c_get_treasures", c_get_treasures);
  lua_register(L, "c_trigger_restart", c_trigger_restart);
  lua_register(L, "c_get_doors", c_get_doors);
  lua_register(L, "c_open_door", c_open_door);
  lua_register(L, "c_get_keys", c_get_keys);
  lua_register(L, "c_take_key", c_take_key);
}

#endif // DNG_CAPI_H
