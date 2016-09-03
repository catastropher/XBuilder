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

#include "levelbuilder.hpp"

LevelBuilder::~LevelBuilder() {
    for(LevelVertex* v : vertices)
        delete v;
}

LevelVertex& LevelBuilder::addVertex(XVex3D vertex) {
    for(LevelVertex* v : vertices) {
        if(x3d_vex3d_equal(&vertex, v))
            return *v;
    }

    LevelVertex* new_v = new LevelVertex(vertex);
    vertices.push_back(new_v);
    
    return *new_v;
}

LevelSegment& LevelBuilder::addSegment(X3D_Prism3D geometry) {
    LevelSegment& seg = newSegment(geometry.base_v);
    
    for(int i = 0; i < geometry.base_v * 2; ++i)
        seg.geometry.v[i] = &addVertex(geometry.v[i]);
    
    return seg;
}

LevelSegment& LevelBuilder::extrudeSegment(LevelSegment& segment, int16 face, int16 extrude_dist) {
    XPrism3D seg_geo = segment.getGeometry();
    
    XPolygon3D seg_face = seg_geo.getFace(face);
    seg_face.reverse();
    
    XPrism3D new_seg_geo(seg_face.total_v);
    new_seg_geo.setFace(X3D_BASE_A, seg_face);
    
    XPlane plane = seg_face.calculatePlane();
    seg_face.translateNormal(&plane.normal, extrude_dist);
    seg_face.reverse();
    new_seg_geo.setFace(X3D_BASE_B, seg_face);
    
    return newSegment(new_seg_geo);
}



