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
#include "FaceTool.hpp"

class ExtrudeFaceTool : public FaceTool {
public:
    ExtrudeFaceTool(ToolContext& context_) : FaceTool(context_),
        extrudeDistanceWidget("Extrude Distance", Distance(10, Distance::FEET).toUnit(
            context_.defaultDistanceUnit),
            Distance(.25, Distance::FEET), 
            Distance(100, Distance::FEET)) { }
    
    void renderToolWindow() {
        extrudeDistanceWidget.render();
        
        float extrudeDist = getExtrudeDistance();
        
        if(ImGui::Button("Extrude!")) {
            selectedFace.face->extrude(extrudeDist);
        }
        
        if(extrudeDist > 0)
            showGeometryPreview();
        
        renderSelectedFace();
    }
    
private:
    void showGeometryPreview() const {
        Prism3D geo = selectedFace.face->getSeg().getGeometry();
        Prism3D newSegGeometry = geo.createPrism3DFromExtrudedFace(geo, selectedFace.face->getId(), getExtrudeDistance());
        ViewRenderer::renderPrism3D(newSegGeometry, context.colorPalette.geometryPreviewColor);
    }
    
    float getExtrudeDistance() const {
        return extrudeDistanceWidget.getDistance().toUnit(Distance::X3D_UNITS).dist;
    }
    
    DistanceSliderInputWidget extrudeDistanceWidget;
};

