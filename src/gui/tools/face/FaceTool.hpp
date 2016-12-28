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
#include "gui/widgets.hpp"
#include "gui/tools/Tool.hpp"

struct SelectedFace {
    Level::Segment& seg;
    int faceId;
};

struct FaceTool : Tool {
    SelectedFace selectedFace;
    
    virtual void updateSelectedFace() { }
};

struct FaceToolGroup : ToolGroup {
    FaceToolGroup() {
        toolDropDown.addItem("extrude", "Extrude");
        toolDropDown.addItem("scale", "Scale");
        toolDropDown.addItem("connect", "Connect");
    }
};


