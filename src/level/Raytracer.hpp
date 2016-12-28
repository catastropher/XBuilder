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

#include "Level.hpp"
#include "geo/Ray.hpp"

namespace Level {
    class Raytracer {
    public:
        struct FaceIntersection {
            PlaneIntersection intersection;
            Segment* seg;
            int faceId;
            
            FaceIntersection() : seg(nullptr) { }
            FaceIntersection(PlaneIntersection& intersection_, Segment* seg_, int faceId_) : intersection(intersection_), seg(seg_), faceId(faceId_) { }
            
            bool operator<(const FaceIntersection& inter) const {
                return intersection < inter.intersection;
            }
            
            bool validIntersection() const {
                return seg != nullptr;
            }
        };
        
        
        Raytracer(Level& level_, Ray ray_) : level(level_), ray(ray_) { }
        
        Raytracer(Level& level_, X3D_CameraObject* cam, X3D_Vex2D pointOnScreen) : level(level_) {
            X3D_Line3D line;
            x3d_line3d_from_screen_point(&line, &pointOnScreen, cam, 15);
            
            Vec3 v0(line.start);
            Vec3 v1 = v0 + Vec3(line.dir);
            
            ray = Ray(v0, v1);
        }
        
        bool findClosestIntersectedFace(FaceIntersection& result);
        
    private:
        FaceIntersection findClosestIntersectionForSegment(Segment& seg);
        
        Level& level;
        Ray ray;
    };
}