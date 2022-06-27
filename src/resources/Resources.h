//========================================================================
// dng
//------------------------------------------------------------------------
// Copyright (c) 2022 Steph Enders <steph@senders.io>
// Copyright (c) 2022 Dan Enders
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

#ifndef DNG_RESOURCES_H
#define DNG_RESOURCES_H

#ifdef __APPLE__
#include "boost/filesystem.hpp"
#include "boost/optional.hpp"
using namespace boost;
#else
#include <filesystem>
#include <optional>
using namespace std;
#endif
#include <memory>
#include <vector>

// struct LevelInfo {
//   filesystem::path dir;
//   filesystem::path dngMap;
//   optional<filesystem::path> procLua;
// };

static const std::string FONT_TYPES[] = {".otf", ".ttf"};
static const filesystem::path DEFAULT_LUA{"defaults.lua"};
static const filesystem::path DNG_MAP{"dng.map"};
static const filesystem::path PROC_LUA{"proc.lua"};
static const char *DEFAULT_FONT = "PressStart2P-vaV7.ttf";

class Resources {

protected:
  std::shared_ptr<filesystem::path> font;        // chosen font
  std::shared_ptr<filesystem::path> defaultsLua; // defaults lua file
  std::vector<filesystem::path> fonts;           // all found maps
  std::vector<filesystem::path> levels;          // all found maps
  virtual std::vector<filesystem::path> levelSearchDirs() = 0;
  virtual std::vector<filesystem::path> defaultsSearchDirs() = 0;
  virtual std::vector<filesystem::path> fontSearchDirs() = 0;

public:
  Resources();
  void loadLevels();
  void loadFontFiles();
  void loadDefaultLuaFile();
  std::vector<filesystem::path> getFontFiles();
  std::vector<filesystem::path> getLevels();
  std::shared_ptr<filesystem::path> updateFont(int idx);
  std::shared_ptr<filesystem::path> getFontFile();
  std::shared_ptr<filesystem::path> getDefaultsLuaFile();
  std::shared_ptr<filesystem::path> getLevelMap(int idx);
  optional<std::shared_ptr<filesystem::path>> getLevelProcLua(int idx);
  /* Helper method to convert any path to a const char *
   * Windows uses wchar_t so this can help provide a common way to
   * interact with files
   */
  virtual const char *convert_to_str(filesystem::path &path) = 0;
};

#endif // DNG_RESOURCES_H
