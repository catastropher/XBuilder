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

#include "Tool.hpp"

struct ToolManager {
    ToolGroup* selectedToolGroup;
    DropDownWidget toolGroupDropdown;
    Level::Level& level;
    
    ToolManager(Level::Level& level_) : selectedToolGroup(nullptr), toolGroupDropdown("Tool Group"), level(level_) {
        toolGroupDropdown.addItem("face", "Face");
        toolGroupDropdown.addItem("segment", "Segment");
        toolGroupDropdown.addItem("edge", "Edge");
    }
    
    void setSelectedToolGroup(std::string name);
    
    void renderToolWindow() {
        ImGui::Begin("Tools");
        toolGroupDropdown.renderDropDown();
        
        if(toolGroupDropdown.valueChanged()) {
            setSelectedToolGroup(toolGroupDropdown.getSelectedValue());
        }
        
        if(selectedToolGroup) {
            ImGui::Separator();
            selectedToolGroup->renderToolDropDown();
            selectedToolGroup->renderToolWindow();
        }
        
        ImGui::End();
    }
    
    void viewWindowHandleMouse(MouseState& state) {
        if(selectedToolGroup)
            selectedToolGroup->viewWindowHandleMouse(state);
    }
    
    void viewWindowHandleKeys() { }
};

