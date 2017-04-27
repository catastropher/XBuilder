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

#include "level/Level.hpp"
#include "windows/windows.hpp"
#include "XBuilderContext.hpp"

struct GuiManager {
public:
    GuiManager(XBuilderContext& context_)
        : context(context_),
        toolWindow(context),
        viewWindow(context),
        consoleWindow(context)
        {
        }
    
    void render() {
        viewWindow.beginRender();
        viewWindow.renderSegmentsInLevel();
        
        MouseState mouseState = viewWindow.getMouseStateRelativeToWindow();
        
        toolWindow.handleMouse(mouseState);
        toolWindow.render();
        
        viewWindow.renderWindow();
        consoleWindow.render();
    }
    
private:
    WindowContext context;
    ToolWindow toolWindow;
    ViewWindow viewWindow;
    ConsoleWindow consoleWindow;
};

