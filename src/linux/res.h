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

#ifndef DNG_RES_H
#define DNG_RES_H

#include <filesystem>

const std::filesystem::path DEFAULT_PROC{"dnglib/defaults.lua"};
const std::filesystem::path DEFAULT_FONT{"res/PressStart2P-vaV7.ttf"};
// TODO setup to allow switching to monospace instead of game font
// const std::filesystem::path DEFAULT_MONOSPACE_FONT{
//    "res/LiberationMono-Regular.ttf"};

struct Res {

  std::filesystem::path defaultsFile;
  std::filesystem::path fontFile;

} typedef Res;

inline Res get_resources() {
  using namespace std::filesystem;

  auto current_dir = current_path();
  auto exe_dir = canonical("/proc/self/exe").remove_filename();
  auto install_dir = path{"/usr/local/share/dng/"};

  Res res;
  if (exists(current_dir / DEFAULT_PROC)) {
    res.defaultsFile = current_dir / DEFAULT_PROC;
  } else if (exists(exe_dir / DEFAULT_PROC)) {
    res.defaultsFile = exe_dir / DEFAULT_PROC;
  } else if (exists(install_dir / DEFAULT_PROC)) {
    res.defaultsFile = install_dir / DEFAULT_PROC;
  } else {
    res.defaultsFile = DEFAULT_PROC; // just return w/ no path info
  }
  // TODO make configurable
  path fontp = DEFAULT_FONT;
  if (exists(current_dir / fontp)) {
    res.fontFile = current_dir / fontp;
  } else if (exists(exe_dir / fontp)) {
    res.fontFile = exe_dir / fontp;
  } else if (exists(install_dir / fontp)) {
    res.fontFile = install_dir / fontp;
  } else {
    res.fontFile = fontp; // just return w/ no path info
  }

  return res;
}

#endif // DNG_RES_H