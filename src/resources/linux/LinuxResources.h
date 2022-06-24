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

#ifndef DNG_LINUXRESOURCES_H
#define DNG_LINUXRESOURCES_H
#include "../Resources.h"
class LinuxResources : public Resources {
public:
  LinuxResources();

protected:
public:
  const char *convert_to_str(std::filesystem::path &path) override;

protected:
  std::filesystem::path exeDir;
  std::filesystem::path workingDir;
  std::vector<std::filesystem::path> levelSearchDirs() override;
  std::vector<std::filesystem::path> defaultsSearchDirs() override;
  std::vector<std::filesystem::path> fontSearchDirs() override;
};
#endif // DNG_LINUXRESOURCES_H
