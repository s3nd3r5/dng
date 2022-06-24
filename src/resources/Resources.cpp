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

void Resources::loadFontFiles() {
  // We will search 1 level deep
  for (auto &base : this->fontSearchDirs()) {
    if (std::filesystem::exists(base) && std::filesystem::is_directory(base)) {
      for (auto &item : std::filesystem::directory_iterator(base)) {
        // only 1 deep
        if (item.exists() && item.is_directory()) {
          for (auto &file : std::filesystem::directory_iterator(item)) {
            if (file.exists() && file.is_regular_file()) {
              auto ext = file.path().extension();
              bool matched =
                  std::any_of(std::begin(FONT_TYPES), std::end(FONT_TYPES),
                              [ext](auto &supported) {
                                return supported == ext.generic_string();
                              });
              if (matched) {
                fonts.push_back(file.path());
                // set default if not set
                if (!exists(*this->font) ||
                    !is_regular_file(*this->font) &&
                        file.path().filename() == DEFAULT_FONT) {
                  *this->font = file.path();
                }
              }
            }
          }
        } else if (item.is_regular_file()) {
          auto ext = item.path().extension();
          bool matched =
              std::any_of(std::begin(FONT_TYPES), std::end(FONT_TYPES),
                          [ext](auto &supported) {
                            return supported == ext.generic_string();
                          });
          if (matched) {
            fonts.push_back(item.path());
            // set default if not set
            if (!exists(*this->font) ||
                !is_regular_file(*this->font) &&
                    item.path().filename() == DEFAULT_FONT) {
              *this->font = item.path();
            }
          }
        }
      }
    }
  }
}

void Resources::loadLevels() {
  for (auto &base : this->levelSearchDirs()) {
    if (std::filesystem::exists(base) && std::filesystem::is_directory(base)) {
      for (auto &dir : std::filesystem::directory_iterator(base)) {
        if (dir.exists() && dir.is_directory()) {
          if (std::filesystem::exists(dir / DNG_MAP)) {
            this->levels.push_back(dir.path());
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
    if (std::filesystem::exists(f)) {
      *this->defaultsLua = f;
      break;
    }
  }
}
std::vector<std::filesystem::path> Resources::getFontFiles() {
  return this->fonts;
}
std::vector<std::filesystem::path> Resources::getLevels() {
  return this->levels;
}
shared_ptr<std::filesystem::path> Resources::getFontFile() {
  return this->font;
}
shared_ptr<std::filesystem::path> Resources::updateFont(int idx) {
  auto f = this->fonts[idx];
  *this->font = f;
  return getFontFile();
}
shared_ptr<std::filesystem::path> Resources::getDefaultsLuaFile() {
  return this->defaultsLua;
}
shared_ptr<std::filesystem::path> Resources::getLevelMap(int idx) {
  auto lvlBase = this->levels[idx];
  std::filesystem::path dngMap = lvlBase / DNG_MAP;
  // existence of the level dng.map is asserted in the initializer
  assert(std::filesystem::exists(dngMap));
  return std::make_shared<std::filesystem::path>(dngMap);
}
std::optional<shared_ptr<std::filesystem::path>>
Resources::getLevelProcLua(int idx) {
  auto lvlBase = this->levels[idx];
  auto procLua = lvlBase / PROC_LUA;
  if (exists(procLua)) {
    return std::make_shared<std::filesystem::path>(procLua);
  } else {
    return nullopt;
  }
}