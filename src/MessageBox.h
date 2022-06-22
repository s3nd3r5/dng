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

#ifndef DNG_MESSAGE_BOX_H
#define DNG_MESSAGE_BOX_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SfmlUtils.h"
#include <vector>
#include <limits>

const int LARGE_TEXT = 54;
const int MEDIUM_TEXT = 36;
const int SMALL_TEXT = 24;
const float PADDING = 16.f;
const float LINE_HEIGHT = 1.2f;
struct MessageBox {

  std::vector<sf::Text> msgs;
  sf::RectangleShape box;

} typedef MessageBox;

struct DisplayText {
  std::string msg;
  int textSize;
} typedef DisplayText;

inline MessageBox initializeMessageBox(const std::vector<DisplayText> &strs,
                                       const sf::Font &font,
                                       sf::Vector2u windowSize) {

  std::vector<sf::Text> messages;
  sf::RectangleShape box;
  float width = 0.f;
  float height = 0.f;
  float left = std::numeric_limits<float>::max();
  sf::Text prev;
  for (const auto &str : strs) {
    sf::Text text = write_text(str.msg.c_str(), str.textSize, LINE_HEIGHT, font,
                               windowSize);
    // move below previous
    if (!prev.getString().isEmpty()) {
      text.move(0.f, prev.getLocalBounds().height * prev.getLineSpacing());
    }
    // add to vector
    messages.push_back(text);

    // find widest
    if (text.getGlobalBounds().width > width) {
      width = text.getGlobalBounds().width;
    }
    // find leftmost
    if (text.getGlobalBounds().left < left) {
      left = text.getGlobalBounds().left;
    }
    // heights added together + line height
    height += (text.getLocalBounds().height * text.getLineSpacing());
    prev = text;
  }
  box = sf::RectangleShape(sf::Vector2f(width + PADDING, height + PADDING));
  box.setOutlineColor(sf::Color::Black);
  box.setOutlineThickness(4.f);
  box.setFillColor(sf::Color(128, 128, 128, 128));
  box.setPosition(left - (PADDING / 2.f),
                  // Top will always be the first message
                  messages[0].getGlobalBounds().top - (PADDING / 2.f));

  return {
      .msgs = messages,
      .box = box,
  };
}

#endif // DNG_MESSAGE_BOX_H