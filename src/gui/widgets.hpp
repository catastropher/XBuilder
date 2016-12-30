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
#include <X3D/X3D.h>

#include "imgui/imgui.h"
#include "geo/Distance.hpp"

struct DropDownItem {
    std::string value;
    std::string displayName;
    
    DropDownItem(std::string value_, std::string displayName_) : value(value_), displayName(displayName_) { }
};

class Widget {

protected:
    static std::string generateUniqueId() {
        return "uniqueid" + std::to_string(nextUniqueId++);
    }
    
private:
    static int nextUniqueId;
};

// TODO: This should be a class
struct DropDownWidget : Widget {
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
    
    std::string getSelectedDisplayName() const {
        if(selectedItem == -1)
            return "";
        
        return items[selectedItem].displayName;
    }
    
    bool valueChanged() {
        bool changed = selectedItem != lastSelectedItem;
        lastSelectedItem = selectedItem;
        
        return changed;
    }
};

class FloatSliderInputWidget : Widget {
public:
    FloatSliderInputWidget(std::string label_, float defaultValue, float minValue_, float maxValue_, std::string format_ = "%.3f")
        : label(label_),
        value(defaultValue),
        minValue(minValue_),
        maxValue(maxValue_),
        selectedInputType(0),
        format(format_),
        selectSliderId("Slider ##" + Widget::generateUniqueId()),
        selectInputId("Input ##" + Widget::generateUniqueId())
        {
            
        }
    
    void render() {
        ImGui::RadioButton(selectSliderId.c_str(), &selectedInputType, 0);
        
        ImGui::SameLine();
        ImGui::RadioButton(selectInputId.c_str(), &selectedInputType, 1);
        
        if(selectedInputType == 0)
            ImGui::SliderFloat(label.c_str(), &value, minValue, maxValue, format.c_str());
        else
            ImGui::InputFloat(label.c_str(), &value);
    }
    
    float getValue() const {
        return value;
    }
    
protected:
    FloatSliderInputWidget(std::string label_)
    : label(label_),
    selectedInputType(0),
    selectSliderId("Slider ##" + Widget::generateUniqueId()),
    selectInputId("Input ##" + Widget::generateUniqueId())
    {
        
    }
    
    void setRange(float minValue_, float maxValue_) {
        minValue = minValue_;
        maxValue = maxValue_;
    }
    
    void setValue(float val) {
        value = val;
    }
    
    void setFormat(std::string format_) {
        format = format_;
    }
    
private:
    std::string label;
    float value;
    float minValue;
    float maxValue;
    int selectedInputType;
    std::string format;
    std::string selectSliderId;
    std::string selectInputId;
};

struct DistanceRange {
    DistanceRange(float defaultValue, float minValue_, float maxValue_, Distance::Unit unit)
        : minValue(minValue_, unit), maxValue(maxValue_, unit), defaultValue(defaultValue, unit) { }
    
    Distance minValue;
    Distance maxValue;
    Distance defaultValue;
};

class DistanceUnitSelectorWidget {
public:
    DistanceUnitSelectorWidget(Distance::Unit defaultUnit) : currentUnit(defaultUnit), previousUnit(defaultUnit) { }
    
    void render() {
        previousUnit = currentUnit;
        
        ImGui::RadioButton("ft", (int *)&currentUnit, Distance::Unit::FEET);
        ImGui::SameLine();
        ImGui::RadioButton("m", (int *)&currentUnit, Distance::Unit::METERS);
        ImGui::SameLine();
        ImGui::RadioButton("X3D units", (int *)&currentUnit, Distance::Unit::X3D_UNITS);
    }
    
    Distance::Unit getCurrentUnit() const {
        return currentUnit;
    }
    
    bool unitChanged() const {
        return currentUnit != previousUnit;
    }
    
    Distance::Unit getPreviousUnit() const {
        return previousUnit;
    }
    
private:
    Distance::Unit currentUnit;
    Distance::Unit previousUnit;
};

class DistanceSliderInputWidget : private FloatSliderInputWidget {
public:
    DistanceSliderInputWidget(std::string label_, DistanceRange range_, DistanceUnitSelectorWidget& unitSelector) :
        FloatSliderInputWidget(label_),
        range(range_),
        unitSelectorWidget(unitSelector) {
            setValue(range_.defaultValue.toUnit(unitSelector.getCurrentUnit()).dist);
            updateUnit();
        }
        
    void render() {
        if(unitSelectorWidget.unitChanged())
            updateUnit();
        
        FloatSliderInputWidget::render();
    }
    
    Distance getDistance() const {
        return Distance(getValue(), currentUnit());
    }
    
    void setDistance(Distance distance) {
        setValue(distance.toUnit(currentUnit()).dist);
    }
    
    float getDistanceInUnits() const {
        return Distance(getValue(), currentUnit()).toUnit(Distance::X3D_UNITS).dist;
    }
    
private:
    Distance::Unit currentUnit() const {
        return unitSelectorWidget.getCurrentUnit();
    }
    
    Distance::Unit previousUnit() const {
        return unitSelectorWidget.getPreviousUnit();
    }
    
    void updateUnit() {
        setRange(
            range.minValue.toUnit(currentUnit()).dist,
            range.maxValue.toUnit(currentUnit()).dist
        );
        
        setValue(Distance(getValue(), previousUnit()).toUnit(currentUnit()).dist);
        setFormat("%.3f " + Distance::getUnitShortName(currentUnit()));
    }
    
    DistanceRange range;
    DistanceUnitSelectorWidget& unitSelectorWidget;
};

class IntSliderWidget : Widget {
public:
    IntSliderWidget(std::string label_, int defaultValue, int minValue_, int maxValue_, std::string format_ = "%.0f")
        : label(label_), value(defaultValue), minValue(minValue_), maxValue(maxValue_), format(format_) { }
    
    void render() {
        ImGui::SliderInt(label.c_str(), &value, minValue, maxValue, format.c_str());
    }
    
    int getValue() const {
        return value;
    }
    
private:
    std::string label;
    int value;
    int minValue;
    int maxValue;
    std::string format;
};

