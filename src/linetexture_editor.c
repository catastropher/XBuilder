#include "X3D.h"

#include <SDL/SDL.h>

static int16 cell_width;
static int16 cell_height;
static int16 line_spacing;
static X3D_Vex2D cursor;
static int16 grid_width;
static int16 grid_height;
static int16 cursor_mode;
static X3D_Vex2D line_start;

static _Bool quit;

static X3D_LineTexture2D texture;

static void render_grid(X3D_Color light, X3D_Color dark) {
  X3D_ScreenManager* screenman = x3d_screenmanager_get();
  
  int16 x, y;
  for(y = 0; y < screenman->h; y += cell_height) {
    for(x = 0; x < screenman->w; x += line_spacing) {
      X3D_Color c = ((y / cell_height) % 4 == 0 ? light : dark);
      
      x3d_screen_draw_pix(x, y, c);
    }
  }
  
  for(x = 0; x < screenman->w; x += cell_width ) {
    for(y = 0; y < screenman->h; y += line_spacing) {
      X3D_Color c = ((x / cell_height) % 4 == 0 ? light : dark);
      
      x3d_screen_draw_pix(x, y, c);
    }
  }
}

static void render_cursor(int16 x, int16 y, X3D_Color color) {
  x3d_screen_draw_circle(x, y, 4, color);
}

static void render() {
  x3d_screen_clear(0);
  render_grid(x3d_rgb_to_color(192, 192, 192), x3d_rgb_to_color(128, 128, 128));
  render_cursor(cursor.x * cell_width, cursor.y * cell_height, x3d_rgb_to_color(255, 255, 255));
  
  if(cursor_mode == 1) {
    render_cursor(line_start.x, line_start.y, 31);
  }
  
  x3d_linetexture2d_render_2d(&texture, (X3D_Vex2D) { 0, 0 }, 31);
  
  x3d_screen_flip();
}

enum {
  KEY_UP = X3D_KEY_0,
  KEY_DOWN = X3D_KEY_1,
  KEY_LEFT = X3D_KEY_2,
  KEY_RIGHT = X3D_KEY_3,
  KEY_ESC = X3D_KEY_4,
  KEY_ENTER = X3D_KEY_5
};

void handle_keys() {
  x3d_read_keys();
  
  if(x3d_key_down(KEY_UP)) {
    if(cursor.y > 0) --cursor.y;
    
    while(x3d_key_down(KEY_UP)) x3d_read_keys();
  }
  
  if(x3d_key_down(KEY_DOWN)) {
    if(cursor.y < grid_height - 1) ++cursor.y;
    
    while(x3d_key_down(KEY_DOWN)) x3d_read_keys();
  }
  
  if(x3d_key_down(KEY_LEFT)) {
    if(cursor.x > 0) --cursor.x;
    
    while(x3d_key_down(KEY_LEFT)) x3d_read_keys();
  }
  
  if(x3d_key_down(KEY_RIGHT)) {
    if(cursor.x < grid_width - 1) ++cursor.x;
    
    while(x3d_key_down(KEY_RIGHT)) x3d_read_keys();
  }
  
  if(x3d_key_down(KEY_ENTER)) {
    X3D_Vex2D pos = { cursor.x * cell_width, cursor.y * cell_height };
    
    if(cursor_mode == 0) {
      line_start = pos;
      cursor_mode = 1;
    }
    else if(cursor_mode == 1) {
      x3d_linetexture2d_add_edge(&texture, line_start, pos);
      cursor_mode = 0;
    }
    
    while(x3d_key_down(KEY_ENTER)) x3d_read_keys();
  }
  
  if(x3d_key_down(KEY_ESC)) quit = X3D_TRUE;
}

void run() {
  do {
    render();
    handle_keys();
  } while(!quit);
}

void xbuilder_linetexture_editor() {
  x3d_key_map_pc(KEY_UP, SDLK_UP);
  x3d_key_map_pc(KEY_DOWN, SDLK_DOWN);
  x3d_key_map_pc(KEY_LEFT, SDLK_LEFT);
  x3d_key_map_pc(KEY_RIGHT, SDLK_RIGHT);
  x3d_key_map_pc(KEY_ESC, SDLK_ESCAPE);
  x3d_key_map_pc(KEY_ENTER, SDLK_RETURN);
  
  cell_width = 16;
  cell_height = 16;
  line_spacing = 4;
  grid_width = x3d_screenmanager_get()->w / cell_width;
  grid_height = x3d_screenmanager_get()->h / cell_height;
  
  cursor.x = 10;
  cursor.y = 10;
  
  cursor_mode = 0;
  
  quit = X3D_FALSE;

  x3d_linetexture2d_create_dynamically_allocated_texture(&texture, 100, 100);
  
  run();
}


