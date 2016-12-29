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

struct ExtrudeFaceTool : FaceTool {
    FloatSliderInputWidget extrudeDistanceWidget;
    
    ExtrudeFaceTool(ToolContext& context_) : FaceTool(context_), extrudeDistanceWidget("Extrude Distance", 100, 1, 3000) { }
    
    void renderToolWindow() {
        extrudeDistanceWidget.render();
        
        float extrudeDist = extrudeDistanceWidget.getValue();
        
        if(ImGui::Button("Extrude!")) {
            selectedFace.face->extrude(extrudeDist);
        }
        
        if(extrudeDist > 0) {
            X3D_ColorIndex gray = x3d_color_to_colorindex(x3d_rgb_to_color(64, 64, 64));
            Prism3D geo = selectedFace.face->getSeg().getGeometry();
            Prism3D newSegGeometry = geo.createPrism3DFromExtrudedFace(geo, selectedFace.face->getId(), extrudeDist);
            ViewRenderer::renderPrism3D(newSegGeometry, gray);
        }
        
        renderSelectedFace();
    }
};

