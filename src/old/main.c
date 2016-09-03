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
#include "mouse.h"

#include <SDL/SDL.h>
#include <agar/core.h>
#include <agar/gui.h>

AG_Pixmap* x3d_pixmap;
AG_Surface* x3d_surface = NULL;


void xbuilder_update_x3d_pixmap() {
  static int surface = -1;
  
  if(x3d_surface != NULL) {
    AG_SurfaceFree(x3d_surface);
  }
  
  x3d_screen_clear(0);
  x3d_render(x3d_playermanager_get()->player[0].cam);
  x3d_surface = AG_SurfaceFromSDL(x3d_screen_get_internal());
  
  if(surface == -1) {
    surface = AG_PixmapAddSurface(x3d_pixmap, x3d_surface);
  }
  else {
    AG_PixmapReplaceSurface(x3d_pixmap, surface, x3d_surface);
  }
  
  AG_PixmapSetSurface(x3d_pixmap, surface);
}

void MouseButtonUp(AG_Event* ev) {
  x3d_log(X3D_INFO, "Clicked!");
}

void init() {
  X3D_InitSettings init = {
    .screen_w = 640,
    .screen_h = 480,
    .screen_scale = 1,
    .fullscreen = X3D_TRUE,
    .fov = ANG_60,
    .flags = X3D_INIT_VIRTUAL_SCREEN
  };
  
  x3d_init(&init);
  
  if(AG_InitCore(NULL, 0) == -1 || AG_InitGraphics(0) == -1) {
    x3d_log(X3D_INFO, "Failed to init agar: %s\n", AG_GetError());
    return (1);
  }
  
  AG_Window* win = AG_WindowNew(0);
  AG_LabelNew(win, 0, "Hello, world!");
  
  x3d_pixmap = AG_PixmapNew(win, 0, 640, 480);
  
  AG_SetEvent(x3d_pixmap, "mouse-button-down", MouseButtonUp, NULL);
  
  
  AG_WindowShow(win);
}

X3D_LineTexture3D logo;
X3D_LineTexture3D aperture;

void xbuilder_linetexture_editor();


enum {
  KEY_ENTER = X3D_KEY_0,
  KEY_W = X3D_KEY_1,
  KEY_S = X3D_KEY_2,
  KEY_A = X3D_KEY_3,
  KEY_D = X3D_KEY_4,
  KEY_ESCAPE = X3D_KEY_5,
  KEY_Q = X3D_KEY_6,
  KEY_E = X3D_KEY_7,
  KEY_UP = X3D_KEY_8,
  KEY_DOWN = X3D_KEY_9,
  KEY_1 = X3D_KEY_10,
  KEY_2 = X3D_KEY_11,
  KEY_9 = X3D_KEY_12,
  KEY_0 = X3D_KEY_13,
  KEY_RECORD = X3D_KEY_14
};


void setup_key_map(void) {
  x3d_key_map_pc(KEY_ENTER, SDLK_RETURN);
  x3d_key_map_pc(KEY_ESCAPE, SDLK_ESCAPE);
  x3d_key_map_pc(KEY_W, 'w');
  x3d_key_map_pc(KEY_A, 'a');
  x3d_key_map_pc(KEY_S, 's');
  x3d_key_map_pc(KEY_D, 'd');
  x3d_key_map_pc(KEY_Q, SDLK_DOWN);
  x3d_key_map_pc(KEY_E, SDLK_UP);
  x3d_key_map_pc(KEY_UP, '[');
  x3d_key_map_pc(KEY_DOWN, ']');
  x3d_key_map_pc(KEY_1, '1');
  x3d_key_map_pc(KEY_2, '2');
  x3d_key_map_pc(KEY_0, '0');
  x3d_key_map_pc(KEY_9, '9');
  x3d_key_map_pc(KEY_RECORD, 'm');
  x3d_key_map_pc(X3D_KEY_15, 'p');
}

extern X3D_Level* global_level;
extern int16 creation_plane_y;

void xbuilder_handle_keys(void) {
  X3D_CameraObject* cam = x3d_playermanager_get()->player[0].cam;
  
  if(x3d_key_down(KEY_ESCAPE)) {
    x3d_game_loop_quit();
  }
  
  if(x3d_key_down(KEY_W)) {
    X3D_Vex3D dir;
    x3d_dynamicobject_forward_vector(&cam->base, &dir);
    cam->base.base.pos.x += dir.x >> 3;
    cam->base.base.pos.y += dir.y >> 3;
    cam->base.base.pos.z += dir.z >> 3;
  }
  else if(x3d_key_down(KEY_S)) {
    X3D_Vex3D dir;
    x3d_dynamicobject_forward_vector(&cam->base, &dir);
    cam->base.base.pos.x -= dir.x >> 3;
    cam->base.base.pos.y -= dir.y >> 3;
    cam->base.base.pos.z -= dir.z >> 3;
  }

#if 1
  if(x3d_key_down(KEY_Q)) {
    cam->base.angle.x -= 4;
    x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
  }
  else if(x3d_key_down(KEY_E)) {
    cam->base.angle.x += 4;
    x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
  }
#endif

  if(x3d_key_down(KEY_A)) {
    cam->base.angle.y -= 4;
    x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
  }
  else if(x3d_key_down(KEY_D)) {
    cam->base.angle.y += 4;
    x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
  }

  if(x3d_key_down(KEY_UP)) {
    cam->base.base.pos.y -= 4L << 8;
  }
  else if(x3d_key_down(KEY_DOWN)) {
    cam->base.base.pos.y += 4L << 8;
  }
  
  _Bool left, right;
  int16 mouse_x, mouse_y;
  x3d_pc_mouse_state(&left, &right, &mouse_x, &mouse_y);
  
//   if(left) {
//     X3D_Vex2D mouse_pos = { mouse_x, mouse_y };
//     X3D_RayTracer raytracer;
//     x3d_raytracer_init_from_point_on_screen(&raytracer, global_level, cam, mouse_pos);
//     
//     X3D_Plane plane;
//     construct_horizontal_plane(0, &plane);
//     
//     X3D_Vex3D inter;
//     int16 dist;
//     
//     x3d_line3d_intersect_plane(&raytracer.ray, &plane, &inter, &dist);
//     
//     X3D_Prism3D new_seg = { .v = alloca(1000) };
//     x3d_prism3d_construct(&new_seg, 8, 200, 200, (X3D_Vex3D_angle256) { 0, 0, 0 });
//     x3d_prism3d_set_center(&new_seg, &inter);
//     
//     x3d_level_add_new_standalone_segment(global_level, &new_seg, 0);
//     
//     do {
//       x3d_read_keys();
//       x3d_pc_mouse_state(&left, &right, &mouse_x, &mouse_y);
//     } while(left);
//   }
}

void draw_3d_grid(X3D_CameraObject* cam, X3D_Vex3D center, int16 cell_size, int16 width_in_cells, int16 height_in_cells) {
  int16 x, z;
  
  int16 max_z = center.z + height_in_cells * cell_size / 2;
  int16 min_z = center.z - height_in_cells * cell_size / 2;
  
  for(x = -width_in_cells / 2; x < width_in_cells / 2 + 1; ++x) {
    X3D_Ray3D ray = {
      {
        x3d_vex3d_make(x * cell_size + center.x, center.y, min_z),
        x3d_vex3d_make(x * cell_size + center.x, center.y, max_z)
      }
    };
    
    x3d_ray3d_render(&ray, cam, x3d_rgb_to_color(128, 128, 128));
  }
  
  int16 max_x = center.x + width_in_cells * cell_size / 2;
  int16 min_x = center.x - width_in_cells * cell_size / 2;
  
  for(z = -height_in_cells / 2; z < height_in_cells / 2 + 1; ++z) {
    X3D_Ray3D ray = {
      {
        x3d_vex3d_make(min_x, center.y, z * cell_size + center.z),
        x3d_vex3d_make(max_x, center.y, z * cell_size + center.z),
      }
    };
    
    x3d_ray3d_render(&ray, cam, x3d_rgb_to_color(128, 128, 128));
  }
}



void test_render_callback(X3D_CameraObject* cam) {
  draw_3d_grid(cam, x3d_vex3d_make(0, 0, 0), 100, 20, 20);
  handle_mouse_callback(cam);
  
  if(x3d_key_down(X3D_KEY_15)) {
    command_move_segment(); 
  }
}

void command_add_segment();
void command_move_segment(void);

int main() {
  X3D_Level level;
  x3d_level_init(&level);
  
  init();
  
  setup_key_map();

  x3d_keymanager_set_callback(xbuilder_handle_keys);
  
  x3d_camera_init();
  X3D_CameraObject* cam = x3d_playermanager_get()->player[0].cam;
  cam->base.angle = (X3D_Vex3D_angle256) { 0, 0, 0 };
  x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
  
  cam->base.base.pos.z = -500 * 256;
  cam->base.base.pos.x = 0;
  cam->base.base.pos.y = -50 * 256;
  
  //command_add_segment();
  
  global_level = &level;
  //x3d_game_loop();
  
  xbuilder_update_x3d_pixmap();
  AG_EventLoop();
  
  //xbuilder_linetexture_editor();
  x3d_cleanup();
}


