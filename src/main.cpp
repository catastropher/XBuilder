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

#include <iostream>

#include "X3D.h"
#include "levelbuilder.hpp"

#include "x3dwrapper.hpp"

using namespace std;

extern "C" {
void test_render_callback() {
    
}

}

int main() {
    X3D_InitSettings init = {
        .screen_w = 640,
        .screen_h = 480,
        .screen_scale = 1,
        .fullscreen = X3D_TRUE,
        .fov = ANG_60,
        .flags = X3D_INIT_VIRTUAL_SCREEN
    };
    
    x3d_init(&init);
}
