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

#include "tool.hpp"

ToolManager* globalToolState;

void handle_movement_keys(X3D_CameraObject* cam) {
    if(x3d_pc_key_down(SDLK_ESCAPE)) {
        x3d_game_loop_quit();
    }
    
    if(x3d_pc_key_down('w')) {
        X3D_Vex3D dir;
        x3d_dynamicobject_forward_vector(&cam->base, &dir);
        cam->base.base.pos.x += dir.x >> 3;
        cam->base.base.pos.y += dir.y >> 3;
        cam->base.base.pos.z += dir.z >> 3;
    }
    else if(x3d_pc_key_down('s')) {
        X3D_Vex3D dir;
        x3d_dynamicobject_forward_vector(&cam->base, &dir);
        cam->base.base.pos.x -= dir.x >> 3;
        cam->base.base.pos.y -= dir.y >> 3;
        cam->base.base.pos.z -= dir.z >> 3;
    }
    
    if(x3d_pc_key_down(SDLK_DOWN)) {
        cam->base.angle.x -= 4;
        x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
    }
    else if(x3d_pc_key_down(SDLK_UP)) {
        cam->base.angle.x += 4;
        x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
    }
    
    if(x3d_pc_key_down('a')) {
        cam->base.angle.y -= 4;
        x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
    }
    else if(x3d_pc_key_down('d')) {
        cam->base.angle.y += 4;
        x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
    }
    
//     if(x3d_pc_key_down(SDLK_UP)) {
//         cam->base.base.pos.y -= 4L << 8;
//     }
//     else if(x3d_pc_key_down(SDLK_DOWN)) {
//         cam->base.base.pos.y += 4L << 8;
//     }
}

//void handle_geo_selector_keys(GeometrySelector& selector) {
//     if(x3d_pc_key_down('f')) {
//         selector.enableSelectFaceA();
//     }
//     else if(x3d_pc_key_down('g')) {
//         selector.enableSelectFaceB();
//     }
//     else if(x3d_pc_key_down('h')) {
//         selector.enableSelectSeg();
//     }
//}


void xbuilder_handler_record() {
    const int KEY_RECORD = '0';
    
    static _Bool rec = X3D_FALSE;
    
    if(x3d_pc_key_down(KEY_RECORD)) {
        while(x3d_pc_key_down(KEY_RECORD)) {
            x3d_read_keys();
        }
        
        if(!rec) {
            system("rm -rf ~/record");
            system("mkdir ~/record");
            
            printf("Begin recording in (hold M to abort):\n");
            
            _Bool record = X3D_TRUE;
            
            int16 i;
            for(i = 3; i >= 1; --i) {
                printf("%d\n", i);
                SDL_Delay(1000);
                
                x3d_read_keys();
                if(x3d_pc_key_down(KEY_RECORD)) {
                    printf("Recording aborted\n");
                    while(x3d_pc_key_down(KEY_RECORD)) {
                        x3d_read_keys();
                    }
                    
                    record = X3D_FALSE;
                    break;
                }
            }
            
            if(record) {
                printf("Begin!\n");
                x3d_screen_begin_record("/home/michael/record/frame");
                rec = X3D_TRUE;
            }
        }
        else {
            printf("Recording complete\n");
            x3d_screen_record_end();
            rec = X3D_FALSE;
        }
    }
}

void xbuilder_handle_keys(void) {
    X3D_CameraObject* cam = x3d_playermanager_get()->player[0].cam;
    
    handle_movement_keys(cam);
    xbuilder_handler_record();
    
    globalToolState->handleKeys();
    
    //MouseState mouseState = MouseState::getCurrentMouseState();
    //globalToolState->handleMouse(mouseState);
    
    _Bool left, right;
    int16 mouse_x, mouse_y;
    x3d_pc_mouse_state(&left, &right, &mouse_x, &mouse_y);
    
    X3D_Vex2D mouse_pos = { mouse_x, mouse_y };
    
    //handle_geo_selector_keys(geo_selector);
    //geo_selector.handleMouse(mouse_pos, left, right, cam);
    
    //   if(left) {
    //     X3D_RayTracer raytracer;
    //     x3d_raytracer_init_from_point_on_screen(&raytracer, global_level, cam, mouse_pos);
    //     
    //     X3D_Plane plane;
    //     construct_horizontal_plane(&plane, 0);
    //     
    //     X3D_Vex3D inter;
    //     int16 dist;
    //     
    //     x3d_line3d_intersect_plane(&raytracer.ray, &plane, &inter, &dist);
    //     
    //     X3D_Prism3D new_seg;
    //     new_seg.v = (X3D_Vex3D*)alloca(1000);
    //     
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