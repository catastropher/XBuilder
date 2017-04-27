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

#include <X3D/X3D.h>
#include <SDL2/SDL.h>
#include <vector>

#include "gui/MainWindowManager.hpp"
#include "XBuilderContext.hpp"

using namespace std;

void xbuilder_handle_keys(void);

void initX3d() {
    X3D_InitSettings init = {
        .screen_w = 640,
        .screen_h = 480,
        .screen_scale = 1,
        .fullscreen = X3D_TRUE,
        .fov = ANG_60,
        .flags = X3D_INIT_VIRTUAL_SCREEN
    };
    
    x3d_init(&init);
    
    x3d_rendermanager_set_render_mode(X3D_RENDER_NONE);
    x3d_keymanager_set_callback(xbuilder_handle_keys);
    
    x3d_camera_init();
    X3D_CameraObject* cam = x3d_playermanager_get()->player[0].cam;
    cam->base.angle = (X3D_Vex3D_angle256) { 0, 0, 0 };
    x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
    
    cam->base.base.pos.z = -500 * 256;
    cam->base.base.pos.x = 0;
    cam->base.base.pos.y = -50 * 256;
}

// For dumb reasons this needs to be here
extern "C" void test_render_callback(X3D_CameraObject* cam) { }

// Again, for reasons that are even more dumb, these two globals need to be here as well
X3D_Texture checkerboard;
X3D_LightMapContext lightmap_context;

int main() {    
    initX3d();
    
    MainWindowManager mainWindowManager;
    mainWindowManager.openMainWindow();
    
    XBuilderContext context(mainWindowManager);
    context.enterMainLoop();
    
    mainWindowManager.closeMainWindow();
        
    x3d_cleanup();
}

