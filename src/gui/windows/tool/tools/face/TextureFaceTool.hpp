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

class TextureFaceTool : public FaceTool {
public:
    TextureFaceTool(ToolContext& context_)
    : FaceTool(context_),
    texturePickerWidget(context_.project.getTextureManager().getTextures()),
    offsetX(0),
    offsetY(0),
    lastOffsetX(0),
    lastOffsetY(0),
    angle(0),
    lastAngle(0),
    lastScale(100.0),
    scaleWidget("Scale %", 100, 10.0, 400, "%.3f %%")
    {
        
    }
    
    void updateSelectedFace() {
        LevelSurface& surface = getSurface();
        
        if(surface.hasPrimaryTexture()) {
            X3D_SurfaceTexture primaryTexture = surface.getPrimaryTexture();
            
            angle = primaryTexture.angle;
            offsetX = primaryTexture.offset.x;
            offsetY = primaryTexture.offset.y;
            scaleWidget.setValue(primaryTexture.scale / 256.0 * 100);
            
            texturePickerWidget.setSelectedTexture(context.project.getTextureManager().getTextureByTextureAddress(primaryTexture.tex));
            
            updatePrimaryTexture();
        }
        else {
            texturePickerWidget.setSelectedTexture(nullptr);
            resetPrimaryTexture();
        }
    }
    
    LevelSurface& getSurface() {
        return getSelectedFace().face->getSurface();
    }
    
    void renderToolWindow() {
        texturePickerWidget.render();
        texturePickerWidget.renderOpenModalButton();
        
        if(needToUpdatePrimaryTexture())
            updatePrimaryTexture();
        
        ImGui::DragInt("Offset X", &offsetX, .1);
        ImGui::DragInt("Offset Y", &offsetY, .1);
        ImGui::DragInt("Angle", &angle, .1);
        scaleWidget.render();
    }
    
    void resetPrimaryTexture() {
        offsetX = 0;
        lastOffsetX = 0;
        
        offsetY = 0;
        lastOffsetY = 0;
        
        angle = 0;
        lastAngle = 0;
        
        scaleWidget.setValue(100.0);
        lastScale = 100.0;
    }
    
    bool needToUpdatePrimaryTexture() {
        return offsetX != lastOffsetX ||
            offsetY != lastOffsetY ||
            angle != lastAngle ||
            scaleWidget.getValue() != lastScale ||
            texturePickerWidget.selectedTextureChanged();
    }
    
    void updatePrimaryTexture() {
        X3D_SurfaceTexture primaryTexture;
        primaryTexture.offset.x = offsetX;
        primaryTexture.offset.y = offsetY;
        primaryTexture.tex = &texturePickerWidget.getSelectedTexture()->getX3DTexture();
        primaryTexture.angle = angle;
        primaryTexture.scale = scaleWidget.getValue() / 100 * 256;
        primaryTexture.flags = 0;
        
        LevelSurface& surface = getSurface();
        surface.setPrimaryTexture(primaryTexture);
        surface.rebuildSurface();
        
        lastAngle = angle;
        lastOffsetX = offsetX;
        lastOffsetY = offsetY;
        lastScale = scaleWidget.getValue();
    }
    
private:
    TexturePickerWidget texturePickerWidget;
    int offsetX;
    int offsetY;
    int lastOffsetX;
    int lastOffsetY;
    int angle;
    int lastAngle;
    float lastScale;
    FloatSliderInputWidget scaleWidget;
};

