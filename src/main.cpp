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

#include "CApi.h"
#include "Level.h"
#include "LuaApi.h"
#include "MessageBox.h"
#include "SfmlUtils.h"
#ifdef __linux__
#include "linux/res.h"
#endif
#ifdef _WIN32
#include "windows/res.h"
#endif
#include "lua.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <filesystem>
#include <iostream>

std::shared_ptr<Level> lvl;

Scene scene;
const int MAX_WIDTH = static_cast<int>(SPRITE_SIZE) * 20 * 4;
const int MAX_HEIGHT = static_cast<int>(SPRITE_SIZE) * 20 * 3;

int main(int argc, char **argv) {

  if (argc <= 1) {
    std::cerr << "Must pass in path to level directory" << std::endl;
    return -1;
  }

  auto res = get_resources();

  std::string lvl_pfx = argv[1];

  std::filesystem::path mapFile = lvl_pfx / std::filesystem::path{"dng.map"};
  std::filesystem::path luaFile = lvl_pfx / std::filesystem::path{"proc.lua"};

  lvl = std::make_shared<Level>();
  scene = Scene::INTRO;

  lvl->loadLevelFromFile(to_str(mapFile));
  lua_State *L_lvl = luaL_newstate();
  luaL_openlibs(L_lvl);
  init_c_api(L_lvl);

  lua_State *L_default = luaL_newstate();
  luaL_openlibs(L_default);
  init_c_api(L_default);

  if (std::filesystem::exists(res.defaultsFile) &&
      luaL_dofile(L_default, to_str(res.defaultsFile)) != LUA_OK) {
    std::cout << "Failed to load default proc" << std::endl;
    luaL_error(L_default, "Error: %s", lua_tostring(L_default, -1));
    return EXIT_FAILURE;
  }

  // Initialize to default
  LState *l_state = init_default(L_default);

  if (std::filesystem::exists(luaFile) &&
      luaL_dofile(L_lvl, to_str(luaFile)) == LUA_OK) {
    override_file_fns(L_lvl, l_state);
  } else if (std::filesystem::exists(luaFile)) {
    std::cout << "[C] No Good" << std::endl;
    luaL_error(L_lvl, "Error: %s\n", lua_tostring(L_lvl, -1));
    return EXIT_FAILURE;
  }

  float ZOOM = 0.5f;
  sf::Vector2f mapBounds = {static_cast<float>(lvl->getWidth()) * SPRITE_SIZE,
                            static_cast<float>(lvl->getHeight()) * SPRITE_SIZE};
  int width = static_cast<int>(static_cast<float>(lvl->getWidth()) / ZOOM *
                               SPRITE_SIZE / ZOOM);
  int height = static_cast<int>(static_cast<float>(lvl->getHeight()) / ZOOM *
                                SPRITE_SIZE / ZOOM);
  bool useViewport = false;
  if (width > MAX_WIDTH) {
    width = MAX_WIDTH;
    useViewport = true;
  }
  if (height > MAX_HEIGHT) {
    height = MAX_HEIGHT;
    useViewport = true;
  }

  sf::RenderWindow window(sf::VideoMode(width, height), "dng");
  window.setMouseCursorVisible(false);
  window.setFramerateLimit(30);
  sf::View view(window.getDefaultView());
  view.zoom(ZOOM);
  view.setSize(view.getSize() * ZOOM);
  view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
  if (useViewport) {
    view.setCenter(to_position(lvl->player) +
                   sf::Vector2f(SPRITE_SIZE / 2.f, SPRITE_SIZE / 2.f));
  } else {
    view.setCenter(view.getSize() / 2.f);
  }

  window.setView(view);
  sf::Clock deltaClock;
  sf::Font font;
  font.loadFromFile(to_str(res.fontFile));

  MessageBox intro;
  MessageBox win;
  MessageBox loss;

  do {
    sf::Event event{};
    float dt = deltaClock.restart().asSeconds();

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        window.close();
      }
      if (!lua_onkeypress(l_state->onkeypress, get_key(event))) {
        window.close();
      }
    }

    if (scene == Scene::INTRO) {
      if (!lua_onintro(l_state->onintro)) {
        window.close();
      }
    } else if (scene == Scene::LEVEL) {
      if (!lua_onupdate(l_state->onupdate, dt)) {
        window.close();
      }
    } else if (scene == Scene::WIN) {
      lua_onwin(l_state->onwin);
      // window.close();
    } else if (scene == Scene::LOSS) {
      lua_onloss(l_state->onloss);
    }

    // Render
    if (useViewport) {
      // Reinitialize center view
      if (scene == INTRO) {
        if (useViewport) {
          view.setCenter(to_position(lvl->player) +
                         sf::Vector2f(SPRITE_SIZE / 2.f, SPRITE_SIZE / 2.f));
        } else {
          view.setCenter(view.getSize() / 2.f);
        }
      }
      sf::Vector2f newPos = to_position(lvl->player) + // center
                            sf::Vector2f(SPRITE_SIZE / 2.f, SPRITE_SIZE / 2.f);
      sf::Vector2f diff =
          newPos - (lvl->player.sprite.getPosition() +
                    sf::Vector2f(SPRITE_SIZE / 2.f, SPRITE_SIZE / 2.f));

      if (diff.x > 0.f && newPos.x > view.getSize().x / 2.f) {
        view.move({diff.x, 0.f});
      }
      if (diff.y > 0.f && newPos.y > view.getSize().y / 2.f) {
        view.move({0.f, diff.y});
      }
      if (diff.x < 0.f && newPos.x < view.getCenter().x) {
        view.move({diff.x, 0.f});
      }
      if (diff.y < 0.f && newPos.y < view.getCenter().y) {
        view.move({0.f, diff.y});
      }
      // readjust for OB
      if (view.getCenter().x - view.getSize().x / 2.f < 0.f) {
        view.move({(view.getCenter().x - view.getSize().x / 2.f) * -1.f, 0.f});
      }
      if (view.getCenter().y - view.getSize().y / 2.f < 0.f) {
        view.move({0.f, (view.getCenter().y - view.getSize().y / 2.f) * -1.f});
      }
      if (view.getCenter().x + view.getSize().x / 2.f > mapBounds.x) {
        view.move(
            {-1 * ((view.getCenter().x + view.getSize().x / 2.f) - mapBounds.x),
             0.f});
      }
      if (view.getCenter().y + view.getSize().y / 2.f > mapBounds.y) {
        view.move({0.f, -1 * ((view.getCenter().y + view.getSize().y / 2.f) -
                              mapBounds.y)});
      }
    }

    window.clear(BLANK_COLOR);
    window.setView(view);
    for (auto &rect : lvl->displayMap) {
      window.draw(rect);
    }
    if (scene == Scene::LEVEL) {
      for (auto &enemy : lvl->enemyPositions) {
        enemy.sprite.setPosition(to_position(enemy));
        window.draw(enemy.sprite);
      }
      for (auto &treasure : lvl->treasurePositions) {
        treasure.sprite.setPosition(to_position(treasure));
        window.draw(treasure.sprite);
      }
    }

    if (scene != Scene::LOSS) {
      lvl->player.sprite.setPosition(to_position(lvl->player));
      window.draw(lvl->player.sprite);
    }
    if (scene == Scene::WIN) {
      window.setView(window.getDefaultView());
      if (win.msgs.empty()) {
        win = initializeMessageBox({{"You Win!", LARGE_TEXT},
                                    {"press [space] to restart", SMALL_TEXT}},
                                   font, window.getSize());
      }

      window.draw(win.box);
      for (auto &msg : win.msgs) {
        window.draw(msg);
      }
      window.setView(view);
    }
    if (scene == Scene::LOSS) {
      window.setView(window.getDefaultView());
      if (loss.msgs.empty()) {
        loss = initializeMessageBox({{"You Lose!", LARGE_TEXT},
                                     {"press [space] to restart", SMALL_TEXT}},
                                    font, window.getSize());
      }

      window.draw(loss.box);
      for (auto &msg : loss.msgs) {
        window.draw(msg);
      }
      window.setView(view);
    }
    if (scene == INTRO) {
      window.setView(window.getDefaultView());
      if (intro.msgs.empty()) {
        intro = initializeMessageBox(
            {{"Start!", LARGE_TEXT}, {"press [space]", SMALL_TEXT}}, font,
            window.getSize());
      }

      window.draw(intro.box);
      for (auto &msg : intro.msgs) {
        window.draw(msg);
      }
      window.setView(view);
    }
    window.setView(view);
    window.display();
  } while (window.isOpen());

  std::cout << "[C] Quit" << std::endl;

  return EXIT_SUCCESS;
}
