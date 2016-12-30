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

#include <X3D/X3D.h>

#include "level/Level.hpp"
#include "geo/Polygon.hpp"
#include "geo/Prism.hpp"

class ViewRenderer {
public:
    static void renderSegment(Segment& seg, X3D_ColorIndex color);
    static void renderPrism3D(Prism3D& prism, X3D_ColorIndex color);
    static void renderAllSegmentsInLevel(Level& level, X3D_ColorIndex color);
    static void renderPolygon(Polygon3D& poly, X3D_ColorIndex color);
    static void renderRay(Ray ray, X3D_ColorIndex color);
    
private:
    static X3D_CameraObject* getCameraObject() {
        return x3d_playermanager_get()->player[0].cam;
    }
};

