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

#include "Level.h"
#include "SfmlUtils.h"
#include <fstream>
#include <iostream>
#include <string>

bool canStep(Pos pos, int dx, int dy, std::vector<std::vector<char>> map) {
  return map[pos.y + dy][pos.x + dx] != WALL_SPACE;
}

void Level::loadLevelFromFile(const char *filePath) {
  std::ifstream mapFile(filePath);
  if (mapFile.is_open()) {
    this->file = filePath;
    // each element in the map has a unique ID
    // some magic but player is always 0
    const int playerId = 0;
    // from 1 -> N each enemy and treasure has its  own unique ID
    // IDs are unique entirely, not just per enemy or treasure

    std::string line;
    int y = 0;
    while (std::getline(mapFile, line)) {
      this->map.emplace_back();
      int x = 0;
      for (char c : line) {
        if (c == WALL_TKN) {
          this->map[y].push_back(WALL_SPACE);
          auto w = create_wall(x, y);
          this->displayMap.push_back(w);
        } else if (c == EMPTY_TKN) {
          this->map[y].push_back(BLANK_SPACE);
          // no display info
        } else if (c == ENEMY_TKN) {
          auto e = create_enemy(x, y);
          this->enemyPositions.push_back(
              {.id = this->nextId(), .x = x, .y = y, .sprite = e});
          this->map[y].push_back(BLANK_SPACE);
        } else if (c == PLAYER_TKN) {
          auto p = create_player(x, y);
          this->player = {.id = playerId, .x = x, .y = y, .sprite = p};
          this->map[y].push_back(BLANK_SPACE);
        } else if (c == TREASURE_TKN) {
          auto t = create_treasure(x, y);
          this->treasurePositions.push_back(
              {.id = this->nextId(), .x = x, .y = y, .sprite = t});
          this->map[y].push_back(BLANK_SPACE);
        } else {
          continue;
        }
        ++x;
      }
      ++y;
    }

    unsigned long max_width = 0;
    for (auto &rows : this->map) {
      if (max_width < rows.size()) {
        max_width = rows.size();
      }
    }
    this->width = static_cast<int>(max_width);
    this->height = static_cast<int>(this->map.size());
  }
  mapFile.close();
}

void Level::reset() {
  this->map.clear();
  this->treasurePositions.clear();
  this->displayMap.clear();
  this->enemyPositions.clear();
  this->loadLevelFromFile(this->file);
}

bool Level::isEmpty(int x, int y) { return map[y][x] == BLANK_SPACE; }

bool Level::playerCanStep(int dx, int dy) {
  return canStep(player, dx, dy, map);
}

void Level::print() {
  int x = 0;
  int y = 0;
  for (auto &row : map) {
    for (auto &tile : row) {
      bool printed = false;
      if (player.x == x && player.y == y) {
        std::cout << "p";
        printed = true;
      }
      for (auto pos : enemyPositions) {
        if (pos.x == x && pos.y == y) {
          std::cout << "e";
          printed = true;
        }
      }
      for (auto pos : treasurePositions) {
        if (pos.x == x && pos.y == y) {
          std::cout << "t";
          printed = true;
        }
      }
      if (tile == WALL_SPACE) {
        std::cout << tile;
        printed = true;
      }
      if (!printed) {
        std::cout << " ";
      }
      std::cout << " ";
      ++x;
    }
    std::cout << "\n";
    ++y;
    x = 0;
  }
}

int Level::nextId() { return idCounter++; }

int Level::getEnemyIndex(int id) {
  for (int i = 0; i < enemyPositions.size(); i++) {
    if (enemyPositions[i].id == id) {
      return i;
    }
  }

  return -1;
}
bool Level::enemyCanStep(Pos pos, int dx, int dy) {
  return canStep(pos, dx, dy, map);
}
int Level::getWidth() { return this->width; }
int Level::getHeight() { return this->height; }
