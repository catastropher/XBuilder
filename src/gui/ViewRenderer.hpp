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

#include "geo/Polygon.hpp"
#include "geo/Prism.hpp"
#include "render/X3D_surface.h"

class Segment;
class Level;

class ViewRenderer {
public:
    struct ViewRenderPrismFlags {
        unsigned int renderFaceBitArray;
        
        ViewRenderPrismFlags(unsigned int renderFaceBitArray_) : renderFaceBitArray(renderFaceBitArray_) { }
        ViewRenderPrismFlags() : renderFaceBitArray(0xFFFFFFFF) { }
        
        static ViewRenderPrismFlags renderNone() {
            return ViewRenderPrismFlags(0);
        }
        
        static ViewRenderPrismFlags renderAll() {
            return ViewRenderPrismFlags();
        }
        
        ViewRenderPrismFlags disableFace(int faceId) {
            return ViewRenderPrismFlags(renderFaceBitArray & (~(1 << faceId)));
        }
        
        ViewRenderPrismFlags enableFace(int faceId) {
            return ViewRenderPrismFlags(renderFaceBitArray | (1 << faceId));
        }
        
        bool faceIsEnabled(int faceId) const {
            return (renderFaceBitArray & (1 << faceId)) != 0;
        }
    };
    
    static void renderSegment(Segment& seg, X3D_ColorIndex color);
    static void renderPrism3D(Prism3D& prism, X3D_ColorIndex color, ViewRenderPrismFlags flags = ViewRenderPrismFlags::renderAll());
    static void renderAllSegmentsInLevel(Level& level, X3D_ColorIndex color);
    static void renderPolygon(Polygon3D& poly, X3D_ColorIndex color);
    static void renderRay(Ray ray, X3D_ColorIndex color);
    static void renderPolygonSurface(X3D_Surface* surface, Polygon3D poly);
    
private:
    static X3D_CameraObject* getCameraObject() {
        return x3d_playermanager_get()->player[0].cam;
    }
};

