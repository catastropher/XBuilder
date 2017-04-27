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

#include "geo/Distance.hpp"

#include "project/Project.hpp"

struct ToolContextColorPalette {
    X3D_ColorIndex geometryPreviewColor;
    X3D_ColorIndex primarySelectColor;
    
    ToolContextColorPalette() {
        geometryPreviewColor = getColorIndex(64, 64, 64);
        primarySelectColor = 208; //getColorIndex(0, 0, 255);
    }
    
private:
    X3D_ColorIndex getColorIndex(unsigned char r, unsigned g, unsigned char b) {
        return x3d_color_to_colorindex(x3d_rgb_to_color(r, g, b));
    }
};

struct ToolContext {
    Project& project;
    ToolContextColorPalette colorPalette;
    Distance::Unit defaultDistanceUnit;
    
    ToolContext(WindowContext& context) : project(context.project), defaultDistanceUnit(Distance::Unit::FEET) { }
};

class Tool {
public:
    Tool(ToolContext& context_) : context(context_) { }
    
    virtual void viewWindowHandleMouse(MouseState& state) { }
    virtual void viewWindowHandleKeys() { }
    virtual void renderToolWindow() { }
    
    virtual ~Tool() { }
    
protected:
    ToolContext& context;
};



class ToolGroup {
public:
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
    
    virtual void viewWindowHandleKeys() { }
    
    virtual void renderToolWindow() {
        if(selectedTool)
            selectedTool->renderToolWindow();
    }
    
    virtual ~ToolGroup() {
        if(selectedTool)
            delete selectedTool;
    }
    
protected:
    virtual void setSelectedTool(std::string name) = 0;
    
    DropDownWidget toolDropDown;
    Tool* selectedTool;
    ToolContext& context;
};


