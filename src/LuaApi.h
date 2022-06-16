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

#ifndef DNG_LUA_API_H
#define DNG_LUA_API_H

#include "lua.hpp"
#include <iostream>

struct LState {
  lua_State *onkeypress;
  lua_State *onupdate;
  lua_State *onintro;
  lua_State *onwin;
  lua_State *onloss;
} typedef LState;

const char *ON_KEYPRESS = "onKeyPress";
const char *ON_UPDATE = "onUpdate";
const char *ON_INTRO = "onIntro";
const char *ON_WIN = "onWin";
const char *ON_LOSS = "onLoss";

LState *init_default(lua_State *L) {
  auto *state = static_cast<LState *>(malloc(sizeof(LState)));

  state->onkeypress = L;
  state->onupdate = L;
  state->onintro = L;
  state->onwin = L;
  state->onloss = L;

  return state;
}

bool check_fn(lua_State *L, const char *fn) {
  lua_getglobal(L, fn);
  return lua_isfunction(L, -1);
}

void override_file_fns(lua_State *L, LState *state) {
  if (check_fn(L, ON_KEYPRESS)) {
    state->onkeypress = L;
  }
  if (check_fn(L, ON_UPDATE)) {
    state->onupdate = L;
  }
  if (check_fn(L, ON_INTRO)) {
    state->onintro = L;
  }
  if (check_fn(L, ON_WIN)) {
    state->onwin = L;
  }
  if (check_fn(L, ON_LOSS)) {
    state->onloss = L;
  }
}

bool lua_dofn(lua_State *L, const char *fn) {
  lua_getglobal(L, fn);
  if (!lua_isfunction(L, -1)) {
    std::cout << "[C] Error " << fn << " not function | not found" << std::endl;
    return false;
  }
  lua_pcall(L, 0, 1, 0);
  return true;
}

bool lua_dofn_with_number(lua_State *L, const char *fn, lua_Number num) {
  lua_getglobal(L, fn);
  if (!lua_isfunction(L, -1)) {
    std::cout << "[C] Error " << fn << " not function | not found" << std::endl;
    return false;
  }
  lua_pushnumber(L, num);
  lua_pcall(L, 1, 1, 0);
  return true;
}

bool lua_onkeypress(lua_State *L, int pressedKey) {
  if (pressedKey == -1) {
    return true;
  }
  return lua_dofn_with_number(L, "onKeyPress", pressedKey);
}

bool lua_onupdate(lua_State *L, float dt) {
  return lua_dofn_with_number(L, "onUpdate", dt);
}

bool lua_onintro(lua_State *L) { return lua_dofn(L, "onIntro"); }

bool lua_onwin(lua_State *L) { return lua_dofn(L, "onWin"); }

bool lua_onloss(lua_State *L) { return lua_dofn(L, "onLoss"); }

#endif // DNG_LUA_API_H
