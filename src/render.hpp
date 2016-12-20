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

#define MAX_EDGES_IN_PRISM3D 32

struct Prism3DRenderer {
    X3D_Prism3D prism;
    X3D_ColorIndex edgeColors[MAX_EDGES_IN_PRISM3D];
    
    Prism3DRenderer(X3D_Prism3D& prism_) : prism(prism_) { }
    
    void colorEntirePrism(X3D_ColorIndex color) {
        for(int i = 0; i < MAX_EDGES_IN_PRISM3D; ++i)
            edgeColors[i] = color;
    }
    
    void colorFace(int faceId, X3D_ColorIndex color) {
        uint16 edgeIndexes[MAX_EDGES_IN_PRISM3D];
        uint16 totalFaceEdges = x3d_prism_face_edge_indexes(prism.base_v, faceId, edgeIndexes);
        
        for(uint16 j = 0; j < totalFaceEdges; ++j) {
            edgeColors[edgeIndexes[j]] = color;
        }
    }
    
    void render(X3D_CameraObject* cam) {
        x3d_prism3d_render_wireframe_with_colored_edges(&prism, cam, edgeColors);
    }
};

