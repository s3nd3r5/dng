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
#include <libloaderapi.h>
#define MAX_BUF_SIZE 1024
const std::filesystem::path DEFAULT_PROC{R"(dnglib\defaults.lua)"};
const std::filesystem::path DEFAULT_FONT{R"(res\PressStart2P-vaV7.ttf)"};
// TODO setup to allow switching to monospace instead of game font
// const std::filesystem::path DEFAULT_MONOSPACE_FONT{
//    "res/LiberationMono-Regular.ttf"};

struct Res {

  std::filesystem::path defaultsFile;
  std::filesystem::path fontFile;
};

inline const char *to_str(const std::filesystem::path &file) {
  std::setlocale(LC_ALL, "en_US.utf8"); // TODO more support?
  const wchar_t *wstr = file.c_str();
  size_t len = std::wcslen(wstr) + 1; // gotta get that \0
  char *ret = static_cast<char *>(malloc(sizeof(char) * len)); // buffered
  std::wcstombs(ret, file.c_str(), len);
  return ret;
}

inline Res get_resources() {
  using namespace std::filesystem;

  auto current_dir = current_path();
  char exe_dir_str[255];
  GetModuleFileNameA(nullptr, exe_dir_str, 255);
  auto exe_dir = path(exe_dir_str).remove_filename();
  // TODO - Get from PATH or Reg?
  auto install_dir = path{R"(C:\Program\ Files\ (x86)\dng)"};

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

#ifndef DNG_RES_H
#define DNG_RES_H

#endif // DNG_RES_H
