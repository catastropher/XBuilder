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

void ToolManager::renderLevel(X3D_CameraObject* cam) {
    X3D_Vex3D v[32];
    X3D_Prism3D prism;
    prism.v = v;
    
    X3D_ColorIndex red = x3d_color_to_colorindex(x3d_rgb_to_color(255, 0, 0));
    X3D_ColorIndex white = x3d_color_to_colorindex(x3d_rgb_to_color(255, 255, 255));
    X3D_ColorIndex blue = x3d_color_to_colorindex(x3d_rgb_to_color(0, 0, 255));
    
    X3D_ColorIndex faceColors[] = { blue, white };
    
    for(int segId = 0; segId < x3d_level_total_segs(level); ++segId) {
        X3D_LevelSegment* seg = x3d_level_get_segmentptr(level, segId);
        x3d_levelsegment_get_geometry(level, seg, &prism);
        
        Prism3DRenderer renderer(prism);
        renderer.colorEntirePrism(red);
        
        for(int faceIndex = 0; faceIndex < 2; ++faceIndex) {
            if(faceIsSelected(faceIndex) && selectedFaceParentSeg(faceIndex) == segId) {
                renderer.colorFace(x3d_segfaceid_face(getSelectedFace(faceIndex)), faceColors[faceIndex]);
            }
        }
        
        
        renderer.render(cam);
    }
}


