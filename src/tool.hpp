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

struct MouseState {
    bool leftPressed;
    bool rightPressed;
    X3D_Vex2D pos;
    bool hoverInWindow;
    
    MouseState(bool leftPressed_, bool rightPressed_, X3D_Vex2D pos_, bool hoverInWindow_) :
        leftPressed(leftPressed_), rightPressed(rightPressed_), pos(pos_), hoverInWindow(hoverInWindow_) { }
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
    
    void renderSelectedFace(X3D_CameraObject* cam);
    
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
    virtual void render(X3D_CameraObject* cam) { }
    virtual void run() { }
    
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

struct AddNewSegTool : Tool {
    int creationPlaneY;
    X3D_Vex3D creationLocation;
    bool drawPrism;
    X3D_Prism3D prism;
    X3D_Vex3D prismV[32];
    
    AddNewSegTool(ToolManager& toolState_, int creationPlaneY_, int prismBaseV) : Tool(toolState_), creationPlaneY(creationPlaneY_) {
        prism.v = prismV;
        
        x3d_prism3d_construct(&prism, 8, 200, 200, (X3D_Vex3D_angle256) { 0, 0, 0 });
    }
    
    void handleMouse(MouseState& mouseState) {
        if(!mouseState.hoverInWindow)
            return;
        
        drawPrism = calculateCreationPosFromMouse(mouseState);
        
        if(drawPrism)
            x3d_prism3d_set_center(&prism, &creationLocation);
        
        if(!mouseState.leftPressed)
            return;
        
        
        x3d_level_add_new_standalone_segment(toolState.level, &prism, 0);
    }
    
    void render(X3D_CameraObject* cam) {
        if(drawPrism) {
            X3D_ColorIndex gray = x3d_color_to_colorindex(x3d_rgb_to_color(64, 64, 64));
            x3d_prism3d_render_wireframe(&prism, cam, gray);
        }
    }
    
private:
    static X3D_Plane constructHorizontalPlane(int16 y) {
        X3D_Plane plane;
        
        plane.normal.x = 0;
        plane.normal.y = -0x7FFF;
        plane.normal.z = 0;
        plane.d = y;
        
        return plane;
    }
    
    bool calculateCreationPosFromMouse(MouseState& mouseState) {
        X3D_RayTracer raytracer;
        x3d_raytracer_init_from_point_on_screen(&raytracer, toolState.level, toolState.cam, mouseState.pos);
        
        X3D_Plane plane = constructHorizontalPlane(creationPlaneY);
        int16 dist;
        
        return x3d_line3d_intersect_plane(&raytracer.ray, &plane, &creationLocation, &dist);
    }
};

struct ConnectFacesTool : Tool {
    ConnectFacesTool(ToolManager& toolState_) : Tool(toolState_) { }
    
    void run() {
        if(!toolState.faceIsSelected(0) || !toolState.faceIsSelected(1)) {
            x3d_log(X3D_ERROR, "Please select two faces to connect");
            return;
        }
        
        x3d_level_add_segment_connecting_faces(toolState.level, toolState.getSelectedFace(0), toolState.getSelectedFace(1));
    }
};

struct ExtrudeFaceTool : Tool {
    ExtrudeFaceTool(ToolManager& toolState_) : Tool(toolState_) { }
    
    void run() {
        if(!toolState.faceIsSelected(0)) {
            x3d_log(X3D_ERROR, "Please select a face to extrude");
            return;
        }
        
        x3d_level_add_extruded_segment(toolState.level, toolState.getSelectedFace(0), 200);
    }
};

struct ExpandFaceTool : Tool {
    ExpandFaceTool(ToolManager& toolState_) : Tool(toolState_) { }
    
    void run() {
        if(!toolState.faceIsSelected(0)) {
            x3d_log(X3D_ERROR, "Please select a face to expand");
            return;
        }
        
        X3D_Polygon3D poly;
        poly.v = (X3D_Vex3D *)alloca(1000);
        
        x3d_levelsegment_get_face_geometry(toolState.level, toolState.getSelectedFace(0), &poly);
        x3d_polygon3d_scale(&poly, 256 + 64);
        x3d_levelsegment_update_face_geometry(toolState.level, toolState.getSelectedFace(0), &poly);
    }
};

struct ShrinkFaceTool : Tool {
    ShrinkFaceTool(ToolManager& toolState_) : Tool(toolState_) { }
    
    void run() {
        if(!toolState.faceIsSelected(0)) {
            x3d_log(X3D_ERROR, "Please select a face to expand");
            return;
        }
        
        X3D_Polygon3D poly;
        poly.v = (X3D_Vex3D *)alloca(1000);
        
        x3d_levelsegment_get_face_geometry(toolState.level, toolState.getSelectedFace(0), &poly);
        x3d_polygon3d_scale(&poly, 256 - 64);
        x3d_levelsegment_update_face_geometry(toolState.level, toolState.getSelectedFace(0), &poly);
    }
};

struct SaveLevelTool : Tool {
    SaveLevelTool(ToolManager& toolState_) : Tool(toolState_) { }
    
    void run() {
        std::cout << "Filename to save to: ";
        
        std::string fileName;
        std::getline(std::cin, fileName);
        
        if(!x3d_level_save(toolState.level, fileName.c_str())) {
            x3d_log(X3D_ERROR, "Failed to save level");
        }
        else {
            x3d_log(X3D_INFO, "Level saved successfully");
        }
    }
};

struct LoadLevelTool : Tool {
    LoadLevelTool(ToolManager& toolState_) : Tool(toolState_) { }
    
    void run() {
        std::cout << "Filename to load from: ";
        
        std::string fileName;
        std::getline(std::cin, fileName);
        
        if(!x3d_level_load(toolState.level, fileName.c_str())) {
            x3d_log(X3D_ERROR, "Failed to load level");
        }
        else {
            x3d_log(X3D_INFO, "Level loaded successfully");
        }
    }
};

