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

#pragma once

#include <X3D/X3D.h>

struct MouseState {
    bool leftPressed;
    bool rightPressed;
    X3D_Vex2D pos;
    bool hoverInWindow;
    
    MouseState(bool leftPressed_, bool rightPressed_, X3D_Vex2D pos_, bool hoverInWindow_) :
        leftPressed(leftPressed_), rightPressed(rightPressed_), pos(pos_), hoverInWindow(hoverInWindow_) { }
        
    MouseState() : leftPressed(false), rightPressed(false), pos({ 0, 0 }), hoverInWindow(false) { }
};

