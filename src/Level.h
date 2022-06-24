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

struct Pos {
  int id;
  int x;
  int y;
  sf::RectangleShape sprite;
};

class Level {

public:
  void loadLevelFromFile(const char *filePath);

  bool isEmpty(int x, int y);

  bool playerCanStep(int dx, int dy);

  int getEnemyIndex(int id);

  bool enemyCanStep(Pos pos, int dx, int dy);

  void reset();
  /* deprecate*/
  void print();

  int nextId();
  int getWidth();
  int getHeight();

  std::vector<std::vector<char>> map; // source copy of map
  std::vector<sf::RectangleShape> displayMap;
  Pos player;
  std::vector<Pos> enemyPositions;
  std::vector<Pos> treasurePositions;

private:
  int idCounter = 1; // defaults at 1 (player always 0)
  int width;
  int height;
  const char *file;
};

#endif // DNG_LEVEL_H
