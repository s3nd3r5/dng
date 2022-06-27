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

#include "Resources.h"
#include <algorithm>
#include <cassert>
#include <iostream>

Resources::Resources() {
  this->font = std::make_shared<filesystem::path>();
  this->defaultsLua = std::make_shared<filesystem::path>();
}

void Resources::loadFontFiles() {
  // We will search 1 level deep
  for (auto &base : this->fontSearchDirs()) {
    if (filesystem::exists(base) && filesystem::is_directory(base)) {
      for (auto &item : filesystem::directory_iterator(base)) {
        // only 1 deep
        if (filesystem::exists(item) && filesystem::is_directory(item)) {
          for (auto &file : filesystem::directory_iterator(item)) {
            if (filesystem::exists(file) && filesystem::is_regular_file(file)) {
              auto ext = filesystem::path(file).extension();
              bool matched =
                  std::any_of(std::begin(FONT_TYPES), std::end(FONT_TYPES),
                              [ext](auto &supported) {
                                return supported == ext.generic_string();
                              });
              if (matched) {
                fonts.emplace_back(file);
                // set default if not set
                if ((!filesystem::exists(*this->font) ||
                     !filesystem::is_regular_file(*this->font)) &&
                    filesystem::path(file).filename() == DEFAULT_FONT) {
                  *this->font = filesystem::path(file);
                }
              }
            }
          }
        } else if (filesystem::is_regular_file(item)) {
          auto ext = filesystem::path(item).extension();
          bool matched =
              std::any_of(std::begin(FONT_TYPES), std::end(FONT_TYPES),
                          [ext](auto &supported) {
                            return supported == ext.generic_string();
                          });
          if (matched) {
            fonts.emplace_back(item);
            // set default if not set
            if ((!filesystem::exists(*this->font) ||
                 !filesystem::is_regular_file(*this->font)) &&
                filesystem::path(item).filename() == DEFAULT_FONT) {
              *this->font = filesystem::path(item);
            }
          }
        }
      }
    }
  }
}

void Resources::loadLevels() {
  for (auto &base : this->levelSearchDirs()) {
    if (filesystem::exists(base) && filesystem::is_directory(base)) {
      for (auto &dir : filesystem::directory_iterator(base)) {
        if (filesystem::exists(dir) && filesystem::is_directory(dir)) {
          if (filesystem::exists(dir / DNG_MAP)) {
            this->levels.emplace_back(dir);
          }
        }
      }
    }
  }
  std::sort(this->levels.begin(), this->levels.end());
}
void Resources::loadDefaultLuaFile() {
  for (auto &base : this->defaultsSearchDirs()) {
    auto f = base / DEFAULT_LUA;
    if (filesystem::exists(f)) {
      *this->defaultsLua = f;
      break;
    }
  }
}
std::vector<filesystem::path> Resources::getFontFiles() { return this->fonts; }
std::vector<filesystem::path> Resources::getLevels() { return this->levels; }
std::shared_ptr<filesystem::path> Resources::getFontFile() {
  return this->font;
}
std::shared_ptr<filesystem::path> Resources::updateFont(int idx) {
  auto f = this->fonts[idx];
  *this->font = f;
  return getFontFile();
}
std::shared_ptr<filesystem::path> Resources::getDefaultsLuaFile() {
  return this->defaultsLua;
}
std::shared_ptr<filesystem::path> Resources::getLevelMap(int idx) {
  auto lvlBase = this->levels[idx];
  filesystem::path dngMap = lvlBase / DNG_MAP;
  // existence of the level dng.map is asserted in the initializer
  assert(filesystem::exists(dngMap));
  return std::make_shared<filesystem::path>(dngMap);
}
optional<std::shared_ptr<filesystem::path>>
Resources::getLevelProcLua(int idx) {
  auto lvlBase = this->levels[idx];
  auto procLua = lvlBase / PROC_LUA;
  if (exists(procLua)) {
    return std::make_shared<filesystem::path>(procLua);
  } else {
#ifdef __APPLE__
    return boost::optional<std::shared_ptr<filesystem::path>>();
#else
    return nullopt;
#endif
  }
}