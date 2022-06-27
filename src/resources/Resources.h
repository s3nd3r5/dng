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
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif
#include <optional>
#include <vector>

// struct LevelInfo {
//   filesystem::path dir;
//   filesystem::path dngMap;
//   optional<filesystem::path> procLua;
// };
using namespace std;

static const string FONT_TYPES[] = {".otf", ".ttf"};
static const filesystem::path DEFAULT_LUA{"defaults.lua"};
static const filesystem::path DNG_MAP{"dng.map"};
static const filesystem::path PROC_LUA{"proc.lua"};
static const char *DEFAULT_FONT = "PressStart2P-vaV7.ttf";

class Resources {

protected:
  shared_ptr<fs::path> font;                // chosen font
  shared_ptr<filesystem::path> defaultsLua; // defaults lua file
  vector<filesystem::path> fonts;           // all found maps
  vector<filesystem::path> levels;          // all found maps
  virtual vector<filesystem::path> levelSearchDirs() = 0;
  virtual vector<filesystem::path> defaultsSearchDirs() = 0;
  virtual vector<filesystem::path> fontSearchDirs() = 0;

public:
  Resources();
  void loadLevels();
  void loadFontFiles();
  void loadDefaultLuaFile();
  vector<filesystem::path> getFontFiles();
  vector<filesystem::path> getLevels();
  shared_ptr<fs::path> updateFont(int idx);
  shared_ptr<fs::path> getFontFile();
  shared_ptr<fs::path> getDefaultsLuaFile();
  shared_ptr<fs::path> getLevelMap(int idx);
  optional<shared_ptr<fs::path>> getLevelProcLua(int idx);
  /* Helper method to convert any path to a const char *
   * Windows uses wchar_t so this can help provide a common way to
   * interact with files
   */
  virtual const char *convert_to_str(filesystem::path &path) = 0;
};

#endif // DNG_RESOURCES_H
