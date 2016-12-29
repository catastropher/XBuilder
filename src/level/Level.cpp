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

#include "Level.hpp"

Polygon3D LevelSegmentFace::getGeometry() const {
    return seg.getGeometry().getFace(id);
}

Segment& Segment::operator=(const Segment& seg) {
    level = seg.level;
    faces = seg.faces;
    geometry = seg.geometry;
    id = seg.id;
    deleted = seg.deleted;
    
    return *this;
}

LevelSegmentFace& LevelSegmentFace::operator=(const LevelSegmentFace& face) {
    seg = face.seg;
    id = face.id;
    
    return *this;
}

Segment& LevelSegmentFace::extrude(float dist) {
    Prism3D geo = seg.getGeometry();
    Prism3D newSegGeometry = geo.createPrism3DFromExtrudedFace(geo, id, dist);
    
    return seg.getLevel().addSegment(newSegGeometry);
}

