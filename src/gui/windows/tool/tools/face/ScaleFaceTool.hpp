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

class ScaleFaceTool : public FaceTool {
public:
    ScaleFaceTool(ToolContext& context_) : FaceTool(context_), scaleWidget("Scale %", 100, 1, 400) { }
    
    void renderToolWindow() {
        scaleWidget.render();
        
        Prism3D scaledGeometry = getScaledGeometry();
        
        bool deselect = false;
        
        if(ImGui::Button("Scale!")) {
            selectedFace.face->getSeg().updateGeometry(scaledGeometry);
            deselect = true;
        }
        
        showGeometryPreview();
        renderSelectedFace();
        
        if(deselect)
            deselectFace();
    }
    
private:
    void showGeometryPreview() const {
        Prism3D scaledGeometry = getScaledGeometry();
        ViewRenderer::renderPrism3D(scaledGeometry, context.colorPalette.geometryPreviewColor);
    }
    
    Prism3D getScaledGeometry() const {
        Prism3D geo = selectedFace.face->getSeg().getGeometry();
        
        int faceId = selectedFace.face->getId();
        Polygon3D newFace = geo.getFace(faceId);
        newFace.scaleRelativeToCenter(getScale());
        geo.setFace(faceId, newFace);
        
        return geo;
    }
    
    float getScale() const {
        return scaleWidget.getValue() / 100.0;
    }
    
    FloatSliderInputWidget scaleWidget;
};

