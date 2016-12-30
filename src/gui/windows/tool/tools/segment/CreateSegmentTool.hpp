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

#include "imgui/imgui.h"
#include "SegmentTool.hpp"
#include "gui/widgets.hpp"

class CreateSegmentTool : public SegmentTool {
public:
    CreateSegmentTool(ToolContext& context_) :
        SegmentTool(context_, false),
        unitSelectorWidget(context_.defaultDistanceUnit),
        sidesWidget("Sides in Base", 4, 3, 10, "%.0f sides"),
        radiusWidget("Radius",
            Distance(10, Distance::FEET),
            Distance(1, Distance::FEET), 
            Distance(100, Distance::FEET),
            unitSelectorWidget),
        heightWidget("Height",
            Distance(10, Distance::FEET),
            Distance(1, Distance::FEET), 
            Distance(100, Distance::FEET),
            unitSelectorWidget)
        { }
    
    void renderToolWindow() {
        sidesWidget.render();
        unitSelectorWidget.render();
        radiusWidget.render();
        heightWidget.render();
        
        if(segIsSelected())
            renderSelectedSeg();
    }
    
private:
    void showGeometryPreview() const {

    }
    
    DistanceUnitSelectorWidget unitSelectorWidget;
    IntSliderWidget sidesWidget;
    DistanceSliderInputWidget radiusWidget;
    DistanceSliderInputWidget heightWidget;
};

