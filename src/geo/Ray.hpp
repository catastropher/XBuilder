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

struct Ray {
    Vec3 v[2];
    Vec3 dir;
    
    Ray(Vec3 v0, Vec3 v1) {
        v[0] = v0;
        v[1] = v1;
        
        dir = (v[1] - v[0]).normalize();
    }
    
    Ray() { }
    
    Vec3 calculatePointOnLine(float t) const {
        return v[0] + (v[1] - v[0]) * t;
    }
    
    Ray reflectAboutNormal(const Vec3& normal, const Vec3& intersectionPoint) const {
        Vec3 newDir = dir.reflectAboutNormal(normal);
        return Ray(intersectionPoint, intersectionPoint + newDir);
    }
    
    static Ray constructThroughPointOnScreen(X3D_Vex2D pointOnScreen, X3D_CameraObject* cam) {
        X3D_Line3D line;
        x3d_line3d_from_screen_point(&line, &pointOnScreen, cam, 15);
        
        Vec3 v0(line.start);
        Vec3 v1 = v0 + Vec3(line.dir);
        
        return Ray(v0, v1);
    }
};

