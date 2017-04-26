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

#include <vector>

#include "Plane.hpp"
#include "Polygon.hpp"

struct Frustum {
    std::vector<Plane> planes;
    
    void addPlane(Plane p) {
        planes.push_back(p);
    }
    
    static Frustum constructFromPointAndPolygon(Vec3 point, Polygon3D& poly) {
        Frustum f;
        
        for(int i = 0; i < poly.totalVertices(); ++i) {
            int nextVertex = (i + 1) % poly.totalVertices();
            f.addPlane(Plane(point, poly.vertices[nextVertex], poly.vertices[i]));
        }
        
        Vec3 center = poly.calculateCenter();
        
        if(!f.pointIsInsideOf(center)) {
            for(int i = 0; i < poly.totalVertices(); ++i) {
                f.planes[i].flipDirection();
            }
        }
        
        return f;
    }
    
    bool pointIsInsideOf(Vec3 point) {
        for(int i = 0; i < (int)planes.size(); ++i) {
            if(!planes[i].pointIsOnNormalFacingSide(point))
                return false;
        }
        
        return true;
    }
};

