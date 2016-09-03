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

#include "mouse.h"
#include "X3D.h"

extern X3D_Level* global_level;

int16 creation_plane_y;
int16 vertical_movement_plane_x;

void construct_horizontal_plane(int16 y, X3D_Plane* dest) {
  dest->normal.x = 0;
  dest->normal.y = -0x7FFF;
  dest->normal.z = 0;
  dest->d = -y;
}

void construct_vertical_plane(int16 x, X3D_Plane* dest) {
  dest->normal.x = 0x7FFF;
  dest->normal.y = 0;
  dest->normal.z = 0;
  dest->d = x;
}

void determine_what_mouse_is_hovering_over(X3D_CameraObject* cam, MouseHover* dest, uint16 mode) {
  _Bool left, right;
  int16 mouse_x, mouse_y;
  x3d_pc_mouse_state(&left, &right, &mouse_x, &mouse_y);
  
  X3D_Vex2D mouse_pos = { mouse_x, mouse_y };
  X3D_RayTracer raytracer;
  x3d_raytracer_init_from_point_on_screen(&raytracer, global_level, cam, mouse_pos);
  
  if(x3d_raytracer_find_closest_hit_wall(&raytracer)) {
    dest->dist = raytracer.min_hit_scale;
    dest->segface = x3d_segfaceid_create(raytracer.hit_seg, raytracer.hit_seg_face);
    dest->type = MOUSE_HOVER_FACE;
    dest->hit_point = raytracer.hit_pos;
  }
  else {
    dest->type = MOUSE_HOVER_NOTHING;
  }
  
  
  int16 dist;
  
  X3D_Plane creation_plane;
  construct_horizontal_plane(creation_plane_y, &creation_plane);
  x3d_line3d_intersect_plane(&raytracer.ray, &creation_plane, &dest->creation_plane_intersection, &dist);
  
  X3D_Plane vertical_movement_plane;
  construct_vertical_plane(vertical_movement_plane_x, &vertical_movement_plane);
  x3d_line3d_intersect_plane(&raytracer.ray, &vertical_movement_plane, &dest->vertical_plane_intersection, &dist);
}


static void (*left_click_callback)(MouseHover* hover);
static void (*right_click_callback)(MouseHover* hover);

static void (*left_release_callback)(MouseHover* hover);
static void (*right_release_callback)(MouseHover* hover);

static _Bool repeat_left_click;
static _Bool repeat_right_click;

void set_left_click_callback(void (*callback)(MouseHover* hover), _Bool repeat_left) {
  left_click_callback = callback;
  repeat_left_click = repeat_left;
}

void set_right_click_callback(void (*callback)(MouseHover* hover), _Bool repeat_right) {
  right_click_callback = callback;
  repeat_right_click = repeat_right;
}

void set_left_release_callback(void (*callback)(MouseHover* hover)) {
  left_release_callback = callback;
}

void set_right_release_callback(void (*callback)(MouseHover* hover)) {
  right_release_callback = callback;
}

void reset_mouse_callbacks(void) {
  left_click_callback = NULL;
  right_click_callback = NULL;
  left_release_callback = NULL;
  right_release_callback = NULL;
}


void handle_mouse_callback(X3D_CameraObject* cam) {
  _Bool left, right;
  static _Bool last_left, last_right;
  int16 mouse_x, mouse_y;
  x3d_pc_mouse_state(&left, &right, &mouse_x, &mouse_y);
  
  MouseHover hover;
  determine_what_mouse_is_hovering_over(cam, &hover, 0);
  
  if(left) {
    if(left_click_callback) {
      left_click_callback(&hover);
      
      if(!repeat_left_click) {
        do {
          x3d_read_keys();
          x3d_pc_mouse_state(&left, &right, &mouse_x, &mouse_y);
        } while(left);
        
        left_click_callback = NULL;
      }
    }
  }
  else if(last_left) {
    if(left_release_callback)
      left_release_callback(&hover);
  }
  
  
  if(right) {
    if(right_click_callback) {
      right_click_callback(&hover);
      
      if(!repeat_right_click) {
        do {
          x3d_read_keys();
          x3d_pc_mouse_state(&left, &right, &mouse_x, &mouse_y);
        } while(right);
        
        right_click_callback = NULL;
      }
    }
  }
  else if(last_right) {
    if(right_release_callback)
      right_release_callback(&hover);
  }
  
  last_left = left;
  last_right = right;
}

