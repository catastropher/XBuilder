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

struct MouseState {
    bool leftPressed;
    bool rightPressed;
    X3D_Vex2D pos;
    bool hoverInWindow;
    
    MouseState(bool leftPressed_, bool rightPressed_, X3D_Vex2D pos_, bool hoverInWindow_) :
        leftPressed(leftPressed_), rightPressed(rightPressed_), pos(pos_), hoverInWindow(hoverInWindow_) { }
};

struct Tool {
    virtual void viewWindowHandleMouse() { }
    virtual void viewWindowHandleKeys() { }
    virtual void renderToolWindow() { }
};

struct SelectedFace {
    Level::Segment& seg;
    int faceId;
};

struct FaceTool : Tool {
    SelectedFace selectedFace;
    
    virtual void updateSelectedFace() { }
};

struct ExtrudeFaceTool : FaceTool {
    
};

struct DropDownItem {
    std::string value;
    std::string displayName;
    
    DropDownItem(std::string value_, std::string displayName_) : value(value_), displayName(displayName_) { }
};

struct DropDownWidget {
    std::vector<DropDownItem> items;
    int selectedItem;
    int lastSelectedItem;
    std::string labelText;
    
    DropDownWidget(std::string labelText_) : selectedItem(-1), lastSelectedItem(-1), labelText(labelText_) { }
    
    void addItem(std::string name, std::string displayName) {
        items.push_back(DropDownItem(name, displayName));
    }
    
    void renderDropDown() {
        std::vector<const char*> dropDownNames = buildDropDownNames();
        ImGui::Combo(labelText.c_str(), &selectedItem, &dropDownNames[0], dropDownNames.size());
    }
    
    std::vector<const char*> buildDropDownNames() {
        std::vector<const char*> dropDownNames;
        
        for(int i = 0; i < items.size(); ++i)
            dropDownNames.push_back(&items[i].displayName[0]);
        
        return dropDownNames;
    }
    
    std::string getSelectedValue() const {
        if(selectedItem == -1)
            return "";
        
        return items[selectedItem].value;
    }
    
    bool valueChanged() {
        bool changed = selectedItem != lastSelectedItem;
        lastSelectedItem = selectedItem;
        
        return changed;
    }
};

struct ToolGroup {
    DropDownWidget toolDropDown;
    
    ToolGroup() : toolDropDown("Tool") { }
    
    void renderToolDropDown() {
        toolDropDown.renderDropDown();
    }
};

struct FaceToolGroup : ToolGroup {
    FaceToolGroup() {
        toolDropDown.addItem("extrude", "Extrude");
        toolDropDown.addItem("scale", "Scale");
        toolDropDown.addItem("connect", "Connect");
    }
};

struct ToolManager {
    FaceToolGroup* selectedToolGroup;
    DropDownWidget toolGroupDropdown;
    
    ToolManager() : selectedToolGroup(nullptr), toolGroupDropdown("Tool Group") {
        toolGroupDropdown.addItem("face", "Face");
        toolGroupDropdown.addItem("segment", "Segment");
        toolGroupDropdown.addItem("edge", "Edge");
    }
    
    void setSelectedToolGroup(std::string name) {
        if(selectedToolGroup)
            delete selectedToolGroup;
        
        if(name == "face")
            selectedToolGroup = new FaceToolGroup();
        else
            selectedToolGroup = nullptr;
    }
    
    void renderToolWindow() {
        ImGui::Begin("Tools");
        toolGroupDropdown.renderDropDown();
        
        if(toolGroupDropdown.valueChanged()) {
            setSelectedToolGroup(toolGroupDropdown.getSelectedValue());
        }
        
        if(selectedToolGroup) {
            ImGui::Separator();
            selectedToolGroup->renderToolDropDown();
        }
        
        ImGui::End();
    }
};

