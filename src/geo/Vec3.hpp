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

#include <string>
#include <cmath>
#include <algorithm>
#include <X3D/X3D.h>
#include <tuple>

struct Vec3 {
    float x, y, z;
    
     Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) { }
     Vec3() : x(0), y(0), z(0) { }
     Vec3(X3D_Vex3D& v) : x(v.x), y(v.y), z(v.z) { }
    
     Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    
     Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    
     float lengthSquared() const {
        return x * x + y * y + z * z;
    }
    
     float length() const {
        return sqrt(lengthSquared());
    }
    
     Vec3 normalize() const {
        float len = length();
        
        return Vec3(x / len, y / len, z / len);
    }
    
     Vec3 operator*(float f) const {
        return Vec3(x * f, y * f, z * f);
     }
     
     Vec3 operator/(float f) const {
         return Vec3(x / f, y / f, z / f);
     }
    
     Vec3 cross(const Vec3& v) const {
        const Vec3& u = *this;
        
        return Vec3(
            u.y * v.z - u.z * v.y,
            u.z * v.x - u.x * v.z,
            u.x * v.y - u.y * v.x
        );
    }
    
     Vec3 neg() const {
        return Vec3(-x, -y, -z);
    }
    
     Vec3 multiplyEach(const Vec3& v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    
     Vec3 maxValue(float maxValueAllowed) {
        return Vec3(std::min(maxValueAllowed, x), std::min(maxValueAllowed, y), std::min(maxValueAllowed, z));
    }
    
     Vec3 reflectAboutNormal(const Vec3& normal) const {
        float ndot = -normal.dot(*this);
        return (*this + (normal * 2 * ndot)).normalize();
    }
    
     float dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    
     float distanceBetween(Vec3& v) {
        return (*this - v).length();
    }
    
    std::string toString() const {
        char str[128];
        sprintf(str, "{ %f, %f, %f }", x, y, z);
        
        return std::string(str);
    }
    
    X3D_Vex3D toX3DVex3D() const {
        return x3d_vex3d_make(x, y, z);
    }
    
    std::tuple<float, float, float> toTuple() const {
        return std::make_tuple(x, y, z);
    }
    
    std::tuple<int, int, int> toIntTuple() const {
        return std::make_tuple((int)x, (int)y, (int)z);
    }
};

