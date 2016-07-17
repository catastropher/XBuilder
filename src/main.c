#include "X3D.h"

#include <SDL/SDL.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdk.h>

void init() {
  X3D_InitSettings init = {
    .screen_w = 800,
    .screen_h = 480,
    .screen_scale = 1,
    .fullscreen = X3D_FALSE,
    .fov = ANG_60,
  };
  
  x3d_init(&init);
}

X3D_LineTexture3D logo;

void xbuilder_linetexture_editor();

int main() {
  init();
  
  xbuilder_linetexture_editor();
  x3d_cleanup();
}


