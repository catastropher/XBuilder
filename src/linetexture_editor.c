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
  KEY_ENTER = X3D_KEY_5,
  KEY_S = X3D_KEY_6,
  KEY_L = X3D_KEY_7,
  KEY_TAB = X3D_KEY_8
};

X3D_Vex2D enter_point() {
  printf("Point: ");
  
  X3D_Vex2D p;
  scanf("%d %d", &p.x, &p.y);
  
  return p;
}

void enter_connected_list(void) {
  X3D_Vex2D first = enter_point();
  X3D_Vex2D prev = first; 
  X3D_Vex2D v;
  
  do {
    v = enter_point();
    
    if(v.x == -1 && v.y == -1)
      break;
    
    x3d_linetexture2d_add_edge(&texture, prev, v);
    render();
    prev = v;
  } while(1);
  
  x3d_linetexture2d_add_edge(&texture, prev, first);
}

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
  
  if(x3d_key_down(KEY_S)) {
    printf("Filename to save to: ");
    char filename[1024];
    gets(filename);
    
    if(x3d_linetexture2d_save_to_file(&texture, filename)) {
      x3d_log(X3D_INFO, "Texture saved to file");
    }
    else {
      x3d_log(X3D_ERROR, "Error saving to file");
    }
  }
  
  if(x3d_key_down(KEY_L)) {
    printf("Filename to load from: ");
    char filename[1024];
    gets(filename);
    
    if(x3d_linetexture2d_load_from_file(&texture, filename)) {
      x3d_log(X3D_INFO, "Loaded texture");
    }
    else {
      x3d_log(X3D_ERROR, "Error loading texture");
    }
    
    // We need to make more room in the texture for editing!
    texture.v = realloc(texture.v, sizeof(X3D_Vex2D) * 255);
    texture.edges = realloc(texture.edges, sizeof(X3D_Pair) * 255);
  }
  
  if(x3d_key_down(KEY_TAB)) {
    enter_connected_list();
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
  x3d_key_map_pc(KEY_S, 's');
  x3d_key_map_pc(KEY_L, 'l');
  x3d_key_map_pc(KEY_TAB, SDLK_TAB);
  
  cell_width = 16;
  cell_height = 16;
  line_spacing = 4;
  grid_width = x3d_screenmanager_get()->w / cell_width;
  grid_height = x3d_screenmanager_get()->h / cell_height;
  
  cursor.x = 10;
  cursor.y = 10;
  
  cursor_mode = 0;
  
  quit = X3D_FALSE;

  x3d_linetexture2d_create_dynamically_allocated_texture(&texture, 255, 255);
  
  run();
}


