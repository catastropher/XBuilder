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

#include "gui.h"

static XBuilder_Window* windows = NULL;
static int total_windows = 0;

void xbuilder_window_create(const char* name, const char* parent_window, void (*open_window)(XBuilder_Window* window), void (*close_window)(XBuilder_Window* window)) {
  ++total_windows;
  windows = realloc(windows, sizeof(XBuilder_Window) * total_windows);
  
  XBuilder_Window* new_window = windows + total_windows - 1;
  
  new_window->name = name;
  new_window->open_window = open_window;
  new_window->close_window = close_window;
  new_window->is_open = X3D_FALSE;
  new_window->window = NULL;
  new_window->parent = parent_window;
  new_window->flags = 0;
  
  if(parent_window == NULL)
    new_window->flags |= AG_WINDOW_MAIN;
}

XBuilder_Window* xbuilder_window_getptr(const char* name) {
  int i;
  for(i = 0; i < total_windows; ++i) {
    if(strcmp(windows[i].name, name) == 0)
      return windows + i;
  }
  
  return NULL;
}

void xbuilder_window_open(const char* name) {
  XBuilder_Window* window = xbuilder_window_getptr(name);

  if(window->is_open)
    return;
  
  window->window = AG_WindowNew(window->flags);
  
  if(window->parent != NULL) {
    XBuilder_Window* parent_window = xbuilder_window_getptr(window->parent);
    AG_WindowAttach(parent_window->window, window->window);
  }
  
  if(window->open_window)
    window->open_window(window);
  
  AG_WindowShow(window->window);
  window->is_open = X3D_TRUE;
}

void xbuilder_window_close(const char* name) {
  XBuilder_Window* window = xbuilder_window_getptr(name);
  
  if(!window->is_open)
    return;
  
  if(window->close_window)
    window->close_window(window);
  
  if(window->window) {
    AG_WindowHide(window);
    AG_WindowDetach(window);
    window->window = NULL;
  }
  
  window->is_open = X3D_FALSE;
}

void xbuilder_create_main_window(XBuilder_Window* xwindow) {
  AG_Window* window = xwindow->window;
  
  AG_Toolbar* toolbar = AG_ToolbarNew(window, AG_TOOLBAR_HORIZ, 1, AG_TOOLBAR_STICKY);
  
  AG_ToolbarButton(toolbar, "Add Segment", 1, NULL, NULL);
  AG_ToolbarButton(toolbar, "Select Segment", 1, NULL, NULL);
}

void xbuilder_init_gui(void) {
  xbuilder_window_create("main", NULL, );
}
