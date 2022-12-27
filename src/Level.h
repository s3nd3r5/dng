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

#ifndef DNG_LEVEL_H
#define DNG_LEVEL_H

#include "SFML/Graphics/RectangleShape.hpp"
#include <memory>
#include <vector>

// tokens from map file
static const char PLAYER_TKN = 'p';
static const char WALL_TKN = 'w';
static const char EMPTY_TKN = '0';
static const char TREASURE_TKN = 't';
static const char ENEMY_TKN = 'e';
static const char BLANK_SPACE = '\0';
static const char WALL_SPACE = '#';
static const char KEY_TKN_START = '1';  // inclusive (1, 2, 3, 4)
static const char KEY_TKN_END = '4';    // inclusive (1, 2, 3, 4)
static const char DOOR_TKN_START = 'a'; // inclusive (a, b, c, d)
static const char DOOR_TKN_END = 'd';   // inclusive (a, b, c, d)
static const char KEY_DOOR_MAPPING[4] = {'a', 'b', 'c', 'd'};
static const char DIRT_TKN = '#';

struct Pos {
  char token;
  int id;
  int x;
  int y;
  sf::RectangleShape sprite;
};

class Level {

public:
  explicit Level(const char *filePath);
  ~Level() = default;
  void load();
  bool playerCanStep(int dx, int dy) const;
  bool tryDoor(int x, int y) const;
  bool isDoor(int x, int y) const;
  bool isDirt(int x, int y) const;
  int getEnemyIndex(int id);
  bool enemyCanStep(const Pos &pos, int dx, int dy) const;
  void reset();
  int nextId();
  int getWidth() const;
  int getHeight() const;

  std::vector<std::vector<char>> map; // source copy of map
  std::vector<sf::RectangleShape> displayMap;
  Pos player;
  std::vector<char> heldKeys;
  std::vector<Pos> enemyPositions;
  std::vector<Pos> treasurePositions;
  std::vector<Pos> doorPositions;
  std::vector<Pos> keyPositions;
  std::vector<Pos> dirtPositions;

private:
  int idCounter = 1; // defaults at 1 (player always 0)
  int width{};
  int height{};
  std::unique_ptr<std::string> file;
};

#endif // DNG_LEVEL_H
