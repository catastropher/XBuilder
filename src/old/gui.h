// This file is part of XBuilder.
//
// XBuilder is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// XBuilder is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with XBuilder. If not, see <http://www.gnu.org/licenses/>.

#include "X3D.h"

#include <SDL/SDL.h>
#include <agar/core.h>
#include <agar/gui.h>

typedef struct XBuilder_Window {
  AG_Window* window;
  const char* name;
  const char* parent;
  void (*open_window)(XBuilder_Window* window);
  void (*close_window)(XBuilder_Window* window);
  _Bool is_open;
  int flags;
} XBuilder_Window;

void xbuilder_window_create(const char* name, const char* parent_window, void (*open_window)(XBuilder_Window* window), void (*close_window)(XBuilder_Window* window));

