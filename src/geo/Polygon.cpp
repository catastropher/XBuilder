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

#include "Polygon.hpp"
#include "Plane.hpp"
#include "Frustum.hpp"

bool Polygon3D::rayIntersectsPolygon(Ray& ray, PlaneIntersection& result) {
    Plane plane = calculatePlane();
    
    if(!plane.rayIntersectsPlane(ray, result))
        return false;
    
    return Frustum::constructFromPointAndPolygon(ray.v[0], *this).pointIsInsideOf(result.intersection);
}

