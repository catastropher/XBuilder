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

struct MouseState {
    X3D_Vex2D pos;
    bool leftPressed;
    bool rightPressed;
    
    static MouseState getCurrentMouseState() {
        MouseState state;
        
        int16 x, y;
        x3d_pc_mouse_state(&state.leftPressed, &state.rightPressed, &x, &y);
        
        state.pos.x = x;
        state.pos.y = y;
        
        return state;
    }
};

struct Tool;

struct ToolManager {
    X3D_CameraObject* cam;
    X3D_Level* level;
    
    Tool* activeTool;
    
    X3D_SegFaceID selectedFaces[2];
    
    ToolManager(X3D_CameraObject* cam_, X3D_Level* level_) : cam(cam_), level(level_), activeTool(NULL) {
        resetSelectedFaces();
    }
    
    void setActiveTool(Tool* tool);
    
    void handleMouse(MouseState& mouseState);
    void handleKeys();
    
    void renderLevel(X3D_CameraObject* cam);
    
    void resetSelectedFaces() {
        selectedFaces[0] = X3D_FACE_NONE;
        selectedFaces[1] = X3D_FACE_NONE;
    }
    
    X3D_SegFaceID getSelectedFace(int faceIndex) {
        return selectedFaces[faceIndex];
    }
    
    bool faceIsSelected(int faceIndex) {
        return selectedFaces[faceIndex] != X3D_FACE_NONE;
    }
    
    int selectedFaceParentSeg(int faceIndex) {
        return x3d_segfaceid_seg(selectedFaces[faceIndex]);
    }

private:
    bool handleChangeToolKeys();
};

struct Tool {
    ToolManager& toolState;
    
    Tool(ToolManager& toolState_) : toolState(toolState_) { }
    
    virtual void handleMouse(MouseState& state) { }
    virtual void handleKeys() { }
    
    bool mouseIsHoveringOverLevelGeometry(MouseState& mouseState, X3D_SegFaceID& hoverGeometryId) {
        X3D_RayTracer raytracer;
        x3d_raytracer_init_from_point_on_screen(&raytracer, toolState.level, toolState.cam, mouseState.pos);
        bool hitSomething = x3d_raytracer_find_closest_hit_wall(&raytracer);
        
        if(hitSomething)
            hoverGeometryId = x3d_segfaceid_create(raytracer.hit_seg, raytracer.hit_seg_face);
        
        return hitSomething;
    }
    
    virtual ~Tool() { }
};

struct SelectFaceTool : Tool {
    int selectFaceIndex;
    
    SelectFaceTool(ToolManager& toolState_, int selectFaceIndex_) : Tool(toolState_), selectFaceIndex(selectFaceIndex_) { }
    
    void handleMouse(MouseState& mouseState) {
        if(!mouseState.leftPressed)
            return;
        
        X3D_SegFaceID mouseHover;
        if(!mouseIsHoveringOverLevelGeometry(mouseState, mouseHover))
            return;
        
        toolState.selectedFaces[selectFaceIndex] = mouseHover;
    }
};

