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

#include "Vec3.hpp"
#include "Ray.hpp"

struct PlaneIntersection {
    Vec3 intersection;
    float t;
    
    PlaneIntersection() : t(100000000) { }
    
    bool operator<(const PlaneIntersection& inter) const {
        return t < inter.t;
    }
};

struct Plane {
    Vec3 normal;
    float d;
    
    Plane(Vec3 p1, Vec3 p2, Vec3 p3) {        
        Vec3 u = p1 - p2;
        Vec3 v = p3 - p2;
        
        normal = u.cross(v).normalize();
        d = -p1.dot(normal);
    }
    
    Plane(Vec3 normal_, Vec3 pointOnPlane) : normal(normal_), d(-normal_.dot(pointOnPlane)) { }
    
    Plane(float a, float b, float c, float d_) {
        normal.x = a;
        normal.y = b;
        normal.z = c;
        d = d_;
    }
    
    Plane() { }
    
    bool rayIntersectsPlane(const Ray& ray, PlaneIntersection& result, bool allowNegativeT = false) const {
        float den = ray.dir.dot(normal);
        
        if(den == 0)
            return false;
        
        result.t = -(ray.v[0].dot(normal) + d) / den;
        
        if(result.t < 0 && !allowNegativeT)
            return false;
        
        result.intersection = ray.v[0] + ray.dir * result.t;
        
        return true;
    }
    
    float pointDistance(Vec3 point) const {
        return normal.dot(point) + d;
    }
    
    bool pointIsOnNormalFacingSide(Vec3 point) const {
        return pointDistance(point) < 0;
    }
    
    void flipDirection() {
        normal = normal.neg();
        d = -d;
    }
    
    static Plane constructHorizontal(float y, bool normalPointsUp) {
        Plane plane;
        plane.normal = Vec3(0, (normalPointsUp ? -1 : 1), 0);
        plane.d = -plane.normal.dot(Vec3(0, y, 0));
        
        return plane;
    }
    
    static Plane constructParallelToCameraView(X3D_CameraObject* cam, Vec3 pointOnPlane) {
        X3D_Vex3D dir;
        x3d_dynamicobject_forward_vector(&cam->base, &dir);
        
        Vec3 normal = Vec3(dir.x / 32768.0, dir.y / 32768.0, dir.z / 32768.0);
        
        return Plane(normal, pointOnPlane);
    }
};

