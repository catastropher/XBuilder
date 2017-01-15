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
    texturePickerWidget(TextureManager::getTextures()),
    offsetX(0),
    offsetY(0),
    lastOffsetX(0),
    lastOffsetY(0),
    angle(0),
    lastAngle(0)
    {
        
    }
    
    LevelSurface& getSurface() {
        return getSelectedFace().face->getSurface();
    }
    
    void renderToolWindow() {
        texturePickerWidget.render();
        texturePickerWidget.renderOpenModalButton();
        
        if(texturePickerWidget.selectedTextureChanged()) {
            LevelSurface& surface = getSurface();
            
            if(!surface.hasPrimaryTexture()) {
                X3D_SurfaceTexture tex;
                tex.angle = 0;
                tex.flags = 0;
                tex.offset = x3d_vex2d_make(0, 0);
                tex.scale = 256;
                tex.tex = &texturePickerWidget.getSelectedTexture()->getX3DTexture();
                
                surface.setPrimaryTexture(tex);
            }
            else {
                X3D_SurfaceTexture tex = surface.getPrimaryTexture();
                tex.tex = &texturePickerWidget.getSelectedTexture()->getX3DTexture();
                surface.setPrimaryTexture(tex);
            }
            
            surface.rebuildSurface();
        }
        
        if(lastOffsetX != offsetX || lastOffsetY != offsetY) {
            lastOffsetX = offsetX;
            lastOffsetY = offsetY;
            
            LevelSurface& surface = getSurface();
            X3D_SurfaceTexture tex = surface.getPrimaryTexture();
            tex.offset = x3d_vex2d_make(offsetX, offsetY);
            surface.setPrimaryTexture(tex);
            surface.rebuildSurface();
        }
        
        if(angle != lastAngle) {
            lastAngle = angle;
            
            LevelSurface& surface = getSurface();
            X3D_SurfaceTexture tex = surface.getPrimaryTexture();
            tex.angle = angle;
            surface.setPrimaryTexture(tex);
            surface.rebuildSurface();
        }
        
        ImGui::DragInt("Offset X", &offsetX, .1);
        ImGui::DragInt("Offset Y", &offsetY, .1);
        ImGui::DragInt("Angle", &angle, .1);
    }
    
private:
    TexturePickerWidget texturePickerWidget;
    int offsetX;
    int offsetY;
    int lastOffsetX;
    int lastOffsetY;
    int angle;
    int lastAngle;
};

