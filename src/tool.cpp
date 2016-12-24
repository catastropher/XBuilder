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

#include <X3D/X3D.h>

#include "tool.hpp"
#include "render.hpp"

void ToolManager::setActiveTool(Tool* tool) {
    if(activeTool)
        delete activeTool;
    
    activeTool = tool;
}

bool ToolManager::handleChangeToolKeys() {
    if(x3d_pc_key_down('f')) {
        setActiveTool(new SelectFaceTool(*this, 0));
        return true;
    }
    else if(x3d_pc_key_down('g')) {
        setActiveTool(new SelectFaceTool(*this, 1));
        return true;
    }
    else if(x3d_pc_key_down('l')) {
        setActiveTool(new AddNewSegTool(*this, 0, 8));
        return true;
    }
    else if(x3d_pc_key_down('c')) {
        ConnectFacesTool(*this).run();
        
        while(x3d_pc_key_down('c'))
            x3d_read_keys();
    }
    else if(x3d_pc_key_down('e')) {
        ExtrudeFaceTool(*this).run();
        
        while(x3d_pc_key_down('e'))
            x3d_read_keys();
    }
    else if(x3d_pc_key_down('=')) {
        ExpandFaceTool(*this).run();
        
        while(x3d_pc_key_down('='))
            x3d_read_keys();
    }
    else if(x3d_pc_key_down('-')) {
        ShrinkFaceTool(*this).run();
        
        while(x3d_pc_key_down('-'))
            x3d_read_keys();
    }
    else if(x3d_pc_key_down('1')) {
        SaveLevelTool(*this).run();
        
        while(x3d_pc_key_down('1'))
            x3d_read_keys();
    }
    else if(x3d_pc_key_down('2')) {
        LoadLevelTool(*this).run();
        
        while(x3d_pc_key_down('2'))
            x3d_read_keys();
    }
    
    return false;
}

void ToolManager::handleKeys() {
    if(handleChangeToolKeys())
        return;
    
    if(activeTool)
        activeTool->handleKeys();
}

void ToolManager::handleMouse(MouseState& mouseState) {
    if(activeTool)
        activeTool->handleMouse(mouseState);
}

void ToolManager::renderSelectedFace(X3D_CameraObject* cam) {
    X3D_Vex3D v[32];
    X3D_Prism3D prism;
    prism.v = v;
    
    X3D_ColorIndex red = x3d_color_to_colorindex(x3d_rgb_to_color(255, 0, 0));
    X3D_ColorIndex white = x3d_color_to_colorindex(x3d_rgb_to_color(255, 255, 255));
    X3D_ColorIndex blue = x3d_color_to_colorindex(x3d_rgb_to_color(0, 0, 255));
    
    X3D_ColorIndex faceAColor = blue;
    X3D_ColorIndex faceBColor = white;
    
    X3D_ColorIndex faceColors[] = { blue, white };
    
    if(faceIsSelected(0) && faceIsSelected(1)) {
        if(selectedFaceParentSeg(0) == selectedFaceParentSeg(1)) {
            X3D_LevelSegment* seg = x3d_level_get_segmentptr(level, selectedFaceParentSeg(0));
            x3d_levelsegment_get_geometry(level, seg, &prism);
            
            Prism3DRenderer renderer(prism);
            renderer.colorEntirePrism(red);
            
            renderer.colorFace(x3d_segfaceid_face(getSelectedFace(0)), faceAColor);
            renderer.colorFace(x3d_segfaceid_face(getSelectedFace(1)), faceBColor);
            renderer.render(cam);
            
            return;
        }
    }
    
    for(int faceIndex = 0; faceIndex < 2; ++faceIndex) {
        if(faceIsSelected(faceIndex)) {
            X3D_LevelSegment* seg = x3d_level_get_segmentptr(level, selectedFaceParentSeg(faceIndex));
            x3d_levelsegment_get_geometry(level, seg, &prism);
            
            Prism3DRenderer renderer(prism);
            renderer.colorEntirePrism(red);
            
            renderer.colorFace(x3d_segfaceid_face(getSelectedFace(faceIndex)), faceColors[faceIndex]);
            renderer.render(cam);
        }
    }
}

void ToolManager::renderLevel(X3D_CameraObject* cam) {
    renderSelectedFace(cam);
    
    if(activeTool)
        activeTool->render(cam);
}


