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

#include <vector>
#include <string>

#include "imgui/imgui.h"

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
        
        for(int i = 0; i < (int)items.size(); ++i)
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