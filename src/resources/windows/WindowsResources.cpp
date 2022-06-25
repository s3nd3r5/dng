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
#include "WindowsResources.h"
#include <libloaderapi.h>
#define MAX_BUF_SIZE 1024
std::filesystem::path exeDir;
std::filesystem::path workingDir;

std::vector<std::filesystem::path> WindowsResources::levelSearchDirs() {
  return {workingDir / "maps", exeDir / "maps"};
}
std::vector<std::filesystem::path> WindowsResources::defaultsSearchDirs() {
  return {workingDir / "dnglib", exeDir / "dnglib"};
}
std::vector<std::filesystem::path> WindowsResources::fontSearchDirs() {
  return {workingDir / "res", exeDir / "res"};
}
WindowsResources::WindowsResources() : Resources() {
  this->workingDir = std::filesystem::current_path();
  this->font = std::make_shared<std::filesystem::path>(workingDir / "res" /
                                                           DEFAULT_FONT);

  char exe_dir_str[255];
  GetModuleFileNameA(nullptr, exe_dir_str, 255);
  this->exeDir = std::filesystem::path{exe_dir_str}.remove_filename();
}

const char *WindowsResources::convert_to_str(std::filesystem::path &path) {
  std::setlocale(LC_ALL, "en_US.utf8"); // TODO more support?
  const wchar_t *wstr = path.c_str();
  size_t len = std::wcslen(wstr) + 1; // gotta get that \0
  char *ret = static_cast<char *>(malloc(sizeof(char) * len)); // buffered
  std::wcstombs(ret, path.c_str(), len);
  return ret;
}
