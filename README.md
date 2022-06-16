# dng

dng is a maze/puzzle game + engine! Design and work your way through your own maze avoiding enemies and finding
treasure!

## How to run

You can run the game by `dng path/to/map/lvl`

To run the first level:

`dng maps/lvl1`

## Create your own level!

Levels use the following directory structure:

```shell
level-name/
  dng.map # required -the map file
  proc.lua # optional - custom lua overrides
```

### Map Format

The map format is just a text file with the following key tokens:

| Token | Description              |
|:------|:-------------------------|
| w     | Wall                     |
| p     | The player (must have 1) |
| e     | Enemies (0 or more)      |
| t     | Treasure (0 or more)     |
| 0     | Empty space              |

#### Tips

Space your map out using whitespace between every token (`w w w w` instead of `wwww`) for better readability.
Use a monospace font to help things align

### Custom Lua Code

Create a `proc.lua` in your level folder.
You can override anything found in `dnglib` using standard lua scoping rules.
If you wish to call the original defaults you can include `dnglib.defaults` in your file:
`local defaults = require('dnglib.defaults')` and call the functions:

```lua
package.path = '['
function onKeyPress(key)
    -- Set your own overrides
    defaults.onKeyPress(key)
end
```

## Develop

dng uses git-submodules:

```shell
git clone https://github.com/s3nd3r5/dng
git submodules init
git submodules update
```

### Dependencies

Current build process is setup for a *nix like environment.

| Dependency | Version |
|:-----------|:--------|
| c++        | 20+     |
| g++        | 8+      |
| CMake      | 3.16+   |
| Lua        | 5.4.*   |
| SFML       | 2.5.*   |

_Note: with some CMake modifications we could probably leverage lower versions_

### Build

dng uses CMake

```shell
# use a build dir to ensure we ignore build props
cmake -B cmake-build
```

_Developed with CLion using CMake and g++ on Linux_

### Enforcing local lua files

To ensure you're always loading the local files make sure you include:

`package.path = "./?.lua;" .. package.path` at the top of your `proc.lua`

If you're experiencing issues loading the proper files you can always set in your local development environment:

`export LUA_PATH=/path/to/dng/?.lua;` (replacing `/path/to/dng` with your actual local development path)

this will allow you to run `dng` from anywhere

### Licensing in files

You can find a copy of the notice in `.copyright_headers`.

Note `dnglib/constants.lua` and `src/lua.hpp` which have custom licensing information.

## Run

Once you build the project you can execute it by:

```shell
# use your build dir and select a map!
./cmake-build/dng ./path/to/map/lvl
```

To launch level 1 for example:

```shell
./cmake-build/dng ./maps/lvl1
```

Note: You need to use the working directory containing the `include` folder!

## External Libraries and Resources used by dng

* [SFML](https://github.com/SFML/SFML) licensed
  under [zlib/png license](https://www.sfml-dev.org/license.php)
* [Lua](https://github.com/lua/luat) licensed under [MIT license](https://www.lua.org/license.html)
* [Press Start 2P Font](https://fonts.google.com/specimen/Press+Start+2P#glyphs) licensed
  under [SIL Open Font License](https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL)
* [Liberation Fonts](https://github.com/liberationfonts/liberation-fonts) licensed
  under [SIL Open Font License](https://github.com/liberationfonts/liberation-fonts/blob/main/LICENSE)
