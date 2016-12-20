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

#include "x3dwrapper.hpp"
#include "levelbuilder.hpp"
#include "tool.hpp"

void init() {
    X3D_InitSettings init = {
        .screen_w = 640,
        .screen_h = 480,
        .screen_scale = 1,
        .fullscreen = X3D_TRUE,
        .fov = ANG_60,
        .flags = 0
    };
    
    x3d_init(&init);
}


X3D_Texture checkerboard;
X3D_LightMapContext lightmap_context;

extern X3D_Level* global_level;
int16 creation_plane_y;

void construct_horizontal_plane(X3D_Plane* plane, int16 y) {
    plane->normal.x = 0;
    plane->normal.y = -0x7FFF;
    plane->normal.z = 0;
    plane->d = y;
}


void xbuilder_handle_keys(void);

extern ToolManager* globalToolState;

extern "C" void test_render_callback(X3D_CameraObject* cam) {
    x3d_render_3d_grid(cam, x3d_vex3d_make(0, creation_plane_y, 0), 100, 20, 20);
    //handle_mouse_callback(cam);
    
    //render_level(global_level, cam);
    
    globalToolState->renderLevel(cam);
}

int main() {
    LevelBuilder builder;
    XPrism3D p = XPrism3D::construct(8, 400, 400, { 0, 0, 0 });
    LevelSegment& seg = builder.addSegment(p);
    builder.extrudeSegment(builder.extrudeSegment(seg, 3, 200), 1, 200);
    builder.extrudeSegment(seg, 1, 200);
    builder.extrudeSegment(seg, 8, 200);
    
    X3D_Level level = builder.buildX3DLevel();
    
    init();
    
    //setup_key_map();

    x3d_rendermanager_set_render_mode(X3D_RENDER_NONE);
    
    x3d_keymanager_set_callback(xbuilder_handle_keys);
    
    x3d_camera_init();
    X3D_CameraObject* cam = x3d_playermanager_get()->player[0].cam;
    global_level = &level;
    cam->base.angle = (X3D_Vex3D_angle256) { 0, 0, 0 };
    x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
    
    cam->base.base.pos.z = -500 * 256;
    cam->base.base.pos.x = 0;
    cam->base.base.pos.y = -50 * 256;
    
    ToolManager toolState(cam, global_level);
    globalToolState = &toolState;
    
    
    //command_add_segment();
    x3d_game_loop();
    
    
    //xbuilder_linetexture_editor();
    x3d_cleanup();
}


