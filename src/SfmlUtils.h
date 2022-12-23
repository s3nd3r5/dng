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

#ifndef DNG_SFML_UTILS_H
#define DNG_SFML_UTILS_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <cmath>
#include <iostream>

const float SPRITE_SIZE = 16.f; // squares
const sf::Color WALL_COLOR = sf::Color(150, 150, 150, 255);
const sf::Color BLANK_COLOR = sf::Color(216, 216, 216, 255);
const sf::Color A_DOOR_COLOR = sf::Color(255, 0, 0, 255);
const sf::Color B_DOOR_COLOR = sf::Color(0, 255, 0, 255);
const sf::Color C_DOOR_COLOR = sf::Color(0, 0, 255, 255);
const sf::Color D_DOOR_COLOR = sf::Color(229, 163, 78, 255);

inline sf::Keyboard::Key get_key(sf::Event event) {
  if (event.type == sf::Event::KeyPressed ||
      sf::Keyboard::isKeyPressed(event.key.code)) {
    return event.key.code;
  }
  return sf::Keyboard::Unknown;
}

inline sf::Vector2f to_position_xy(int x, int y) {
  return {static_cast<float>(x) * SPRITE_SIZE,
          static_cast<float>(y) * SPRITE_SIZE};
}
inline sf::Vector2f to_position(const Pos &pos) {
  return to_position_xy(pos.x, pos.y);
}

inline sf::RectangleShape create_square(sf::Color color, int x, int y) {
  sf::RectangleShape rect({SPRITE_SIZE - 1.f, SPRITE_SIZE - 1.f});
  rect.setFillColor(color);
  rect.setOutlineColor(sf::Color::Black);
  rect.setOutlineThickness(1.f);
  rect.setPosition(to_position_xy(x, y));
  return rect;
}

inline sf::RectangleShape create_small_square(sf::Color color, int x, int y) {
  sf::RectangleShape rect({SPRITE_SIZE - 4.f, SPRITE_SIZE - 4.f});
  rect.setFillColor(color);
  auto pos = to_position_xy(x, y);
  rect.setPosition({pos.x + 2.f, pos.y + 2.f});
  return rect;
}

inline sf::RectangleShape create_wall(int x, int y) {
  return create_square(WALL_COLOR, x, y);
}

inline sf::RectangleShape create_enemy(int x, int y) {
  return create_square(sf::Color::Magenta, x, y);
}

inline sf::RectangleShape create_player(int x, int y) {
  return create_square(sf::Color::Cyan, x, y);
}

inline sf::RectangleShape create_treasure(int x, int y) {
  return create_square(sf::Color::Yellow, x, y);
}

inline sf::RectangleShape create_key(char t, int x, int y) {
  switch (t) {
  case '1':
    return create_small_square(A_DOOR_COLOR, x, y);
  case '2':
    return create_small_square(B_DOOR_COLOR, x, y);
  case '3':
    return create_small_square(C_DOOR_COLOR, x, y);
  case '4':
  default:
    return create_small_square(D_DOOR_COLOR, x, y);
  }
}

inline sf::RectangleShape create_door(char t, int x, int y) {
  switch (t) {
  case 'a':
    return create_square(A_DOOR_COLOR, x, y);
  case 'b':
    return create_square(B_DOOR_COLOR, x, y);
  case 'c':
    return create_square(C_DOOR_COLOR, x, y);
  case 'd':
  default:
    return create_square(D_DOOR_COLOR, x, y);
  }
}

inline sf::Vector2f round(const sf::Vector2f vector) {
  return sf::Vector2f{std::round(vector.x), std::round(vector.y)};
}

inline sf::Text write_text(const char *msg, unsigned int fontSize,
                           float lineSpacing, const sf::Font &font,
                           const sf::Vector2u windowSize) {
  sf::Text text(msg, font, fontSize);
  text.setOutlineThickness(4.f);
  text.setOrigin(round(sf::Vector2f{text.getLocalBounds().width / 2,
                                    text.getLocalBounds().height / 2}));
  text.setPosition(sf::Vector2f(windowSize / 2u));
  text.setLineSpacing(lineSpacing);
  while (text.getGlobalBounds().top < 0 || text.getGlobalBounds().left < 0 ||
         static_cast<unsigned int>(
             (text.getGlobalBounds().width + text.getGlobalBounds().left) *
             text.getLineSpacing()) > windowSize.x ||
         static_cast<unsigned int>(
             (text.getGlobalBounds().height + text.getGlobalBounds().top) *
             text.getLineSpacing()) > windowSize.y) {
    text.setCharacterSize(text.getCharacterSize() - 1);
    text.setOrigin(round(sf::Vector2f{text.getLocalBounds().width / 2,
                                      text.getLocalBounds().height / 2}));
    text.setPosition(sf::Vector2f(windowSize / 2u));
  }
  return text;
}

#endif // DNG_SFML_UTILS_H
