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

#include "Vec3.hpp"
#include "Plane.hpp"
#include "Ray.hpp"

struct Polygon3D {
    std::vector<Vec3> vertices;
    
    Polygon3D() { }
    Polygon3D(int totalVertices_) : vertices(totalVertices_) { }
    Polygon3D(std::vector<Vec3>& v) : vertices(v) { }
    
    void addPoint(Vec3 v) {
        vertices.push_back(v);
    }
    
    int totalVertices() const {
        return vertices.size();
    }
    
    Plane calculatePlane() const {
        return Plane(vertices[0], vertices[1], vertices[2]);
    }
    
    void reverse() {
        for(int i = 0; i < totalVertices() / 2; ++i)
            std::swap(vertices[i], vertices[totalVertices() - i - 1]);
    }
    
    void translate(Vec3 v) {
        for(int i = 0; i < totalVertices(); ++i)
            vertices[i] = vertices[i] + v;
    }
    
    void translateAlongNormal(float dist) {
        translate(calculatePlane().normal * dist);
    }
    
    Vec3 calculateCenter() const {
        Vec3 center(0, 0, 0);
        
        for(int i = 0; i < totalVertices(); ++i)
            center = center + vertices[i];
        
        return center / totalVertices();
    }
    
    bool rayIntersectsPolygon(Ray& ray, PlaneIntersection& result);
    
    void scaleRelativeToCenter(float scale) {
        Vec3 center = calculateCenter();
        
        for(int i = 0; i < totalVertices(); ++i) {
            vertices[i] = (vertices[i] - center) * scale + center;
        }
    }
};

