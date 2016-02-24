#include "X3D.h"

#include <SDL/SDL.h>

void init() {
  X3D_InitSettings init = {
    .screen_w = 320,
    .screen_h = 240,
    .screen_scale = 1,
    .fullscreen = X3D_FALSE,
    .fov = ANG_60
  };
  
  x3d_init(&init);
}


int main(int argc, char *argv[]) {
  init();
  
  SDL_Delay(5000);
  
  x3d_cleanup();
  
}