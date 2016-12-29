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

#include "gui/MouseState.hpp"
#include "level/Level.hpp"

struct WindowContext {
    Level& level;
    
    WindowContext(Level& level_) : level(level_) { }
};

class Window {
public:
    virtual void render() = 0;
    
    Window(WindowContext& context_) : context(context_) { }
    
private:
    WindowContext context;
};

