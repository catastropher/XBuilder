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

#include <algorithm>

#include "Raytracer.hpp"

namespace Level {

    bool Raytracer::findClosestIntersectedFace(Raytracer::FaceIntersection& result) {
        FaceIntersection closestIntersection;
        
        for(Level::SegmentIterator seg = level.segmentBegin(); seg != level.segmentEnd(); ++seg) {
            printf("\tVisit seg\n");
            closestIntersection = std::min(closestIntersection, findClosestIntersectionForSegment(*seg));
        }
        
        result = closestIntersection;
        
        return result.seg != nullptr;
    }
    
    Raytracer::FaceIntersection Raytracer::findClosestIntersectionForSegment(Segment& seg) {
        FaceIntersection closestIntersection;
        Prism3D geometry = seg.getGeometry();
        
        for(int i = 0; i < geometry.totalFaces(); ++i) {
            Polygon3D face = geometry.getFace(i);
            PlaneIntersection planeIntersection;
            
            if(face.rayIntersectsPolygon(ray, planeIntersection)) {
                printf("HitX! %f\n", planeIntersection.t);
                closestIntersection = std::min(closestIntersection, FaceIntersection(planeIntersection, &seg, i));
            }
        }
        
        return closestIntersection;
    }
}




