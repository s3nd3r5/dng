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

#ifndef DNG_MACRESOURCES_H
#define DNG_MACRESOURCES_H
#include "../Resources.h"

namespace fs = std::experimental::filesystem;

class MacResources : public Resources {
public:
  MacResources();

protected:
public:
  const char *convert_to_str(fs::path &path) override;

protected:
  fs::path exeDir;
  fs::path workingDir;
  std::vector<fs::path> levelSearchDirs() override;
  std::vector<fs::path> defaultsSearchDirs() override;
  std::vector<fs::path> fontSearchDirs() override;
};
#endif // DNG_MACRESOURCES_H
