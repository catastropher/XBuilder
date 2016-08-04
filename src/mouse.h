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

enum {
  MOUSE_HOVER_NOTHING,
  MOUSE_HOVER_FACE
};

typedef struct MouseHover {
  int16 type;
  int16 dist;
  
  X3D_SegFaceID segface;
  X3D_Vex3D creation_plane_intersection;
  X3D_Vex3D vertical_plane_intersection;
  
  X3D_Vex3D hit_point;
} MouseHover;

void determine_what_mouse_is_hovering_over(X3D_CameraObject* cam, MouseHover* dest, uint16 mode);

void set_left_click_callback(void (*callback)(MouseHover*), _Bool repeat_left);
void set_right_click_callback(void (*callback)(MouseHover*), _Bool repeat_right);
void handle_mouse_callback(X3D_CameraObject* cam);

void set_left_release_callback(void (*callback)(MouseHover* hover));
void set_right_release_callback(void (*callback)(MouseHover* hover));

void reset_mouse_callbacks(void);

