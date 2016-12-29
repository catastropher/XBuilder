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
#include <iostream>
#include <string>
#include "level/Level.hpp"

#include "imgui/imgui.h"
#include "gui/widgets.hpp"
#include "gui/MouseState.hpp"

#include "gui/windows/Window.hpp"

struct ToolContext {
    Level& level;
    
    ToolContext(WindowContext& context) : level(context.level) { }
};

struct Tool {
    ToolContext& context;
    
    Tool(ToolContext& context_) : context(context_) { }
    
    virtual void viewWindowHandleMouse(MouseState& state) { }
    virtual void viewWindowHandleKeys() { }
    virtual void renderToolWindow() { }
    
    virtual ~Tool() { }
};



struct ToolGroup {
    DropDownWidget toolDropDown;
    Tool* selectedTool;
    ToolContext& context;
    
    ToolGroup(ToolContext& context_) : toolDropDown("Tool"), selectedTool(nullptr), context(context_) { }
    
    void renderToolDropDown() {
        toolDropDown.renderDropDown();
        
        if(toolDropDown.valueChanged())
            setSelectedTool(toolDropDown.getSelectedValue());
    }
    
    virtual void viewWindowHandleMouse(MouseState& state) {
        if(selectedTool)
            selectedTool->viewWindowHandleMouse(state);
    }
    
    virtual void setSelectedTool(std::string name) = 0;
    
    virtual void viewWindowHandleKeys() { }
    
    virtual void renderToolWindow() {
        if(selectedTool)
            selectedTool->renderToolWindow();
    }
    
    virtual ~ToolGroup() {
        if(selectedTool)
            delete selectedTool;
    }
};


