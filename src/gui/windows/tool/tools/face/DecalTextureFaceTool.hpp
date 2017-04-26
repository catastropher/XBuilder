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

class DecalTextureFaceTool : public FaceTool {
public:
    DecalTextureFaceTool(ToolContext& context_)
    : FaceTool(context_),
    texturePickerWidget(TextureManager::getTextures()),
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
    
    virtual void viewWindowHandleMouse(MouseState& state) {
        if(!faceIsSelected()) {
            FaceTool::viewWindowHandleMouse(state);
        }
        else if(state.leftPressed && texturePickerWidget.getSelectedTexture()) {
            Raytracer::FaceIntersection intersection = findFaceUnderMouse(state);
            
            if(intersection.validIntersection() && intersection.face == getSelectedFace().face) {
                X3D_Vex2D offset = calculatePositionInSurfaceCoordinateSystemFrom3DPoint(intersection.intersection.intersection);
                addDecalTexture(offset);
            }
        }
    }
    
    X3D_SurfaceTexture createTextureFromGui() {
        X3D_SurfaceTexture tex;
        tex.offset.x = offsetX;
        tex.offset.y = offsetY;
        tex.tex = &texturePickerWidget.getSelectedTexture()->getX3DTexture();
        tex.angle = angle;
        tex.scale = scaleWidget.getValue() / 100 * 256;
        tex.flags = 0;
        
        return tex;
    }
    
    X3D_Vex2D calculatePositionInSurfaceCoordinateSystemFrom3DPoint(Vec3 point) {
        return getSurface().projectPointInSurfaceCoordinateSystem(point);
    }
    
    void addDecalTexture(X3D_Vex2D offset) {
        X3D_SurfaceTexture decalTex = createTextureFromGui();
        
        offset.x -= x3d_surfacetexture_w(&decalTex) / 2;
        offset.y -= x3d_surfacetexture_h(&decalTex) / 2;
        
        decalTex.flags |= X3D_SURFACETEXTURE_IS_DECAL;
        decalTex.offset = offset;
        
        LevelSurface& surface = getSurface();
        
        surface.addNewDecalTexture(decalTex);
        surface.rebuildSurface();
    }
    
    LevelSurface& getSurface() {
        return getSelectedFace().face->getSurface();
    }
    
    void renderToolWindow() {
        texturePickerWidget.render();
        texturePickerWidget.renderOpenModalButton();
        
        //if(needToUpdateDecalTexture())
        //    updatePrimaryTexture();
        
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
    
    bool needToUpdateDecalTexture() {
        return offsetX != lastOffsetX ||
            offsetY != lastOffsetY ||
            angle != lastAngle ||
            scaleWidget.getValue() != lastScale ||
            texturePickerWidget.selectedTextureChanged();
    }
    
//     void updatePrimaryTexture() {
//         X3D_SurfaceTexture primaryTexture;
//         primaryTexture.offset.x = offsetX;
//         primaryTexture.offset.y = offsetY;
//         primaryTexture.tex = &texturePickerWidget.getSelectedTexture()->getX3DTexture();
//         primaryTexture.angle = angle;
//         primaryTexture.scale = scaleWidget.getValue() / 100 * 256;
//         primaryTexture.flags = 0;
//         
//         LevelSurface& surface = getSurface();
//         surface.setPrimaryTexture(primaryTexture);
//         surface.rebuildSurface();
//         
//         lastAngle = angle;
//         lastOffsetX = offsetX;
//         lastOffsetY = offsetY;
//         lastScale = scaleWidget.getValue();
//     }
    
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

