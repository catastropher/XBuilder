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

#include "ViewRenderer.hpp"

void ViewRenderer::renderSegment(Segment& seg, X3D_ColorIndex color) {
    Prism3D prism = seg.getGeometry();
    renderPrism3D(prism, color);
}

void ViewRenderer::renderPrism3D(Prism3D& prism, X3D_ColorIndex color) {
    X3D_Vex3D v[32];
    X3D_Prism3D x3dPrism;
    x3dPrism.v = v;
    
    X3D_CameraObject* cam = getCameraObject();
    
    prism.toX3DPrism3D(&x3dPrism);
    x3d_prism3d_render_wireframe(&x3dPrism, cam, color);
}

void ViewRenderer::renderAllSegmentsInLevel(Level& level, X3D_ColorIndex color) {
    for(Level::SegmentIterator seg = level.segmentBegin(); seg != level.segmentEnd(); ++seg)
        renderSegment(*seg, color);
}

void ViewRenderer::renderPolygon(Polygon3D& poly, X3D_ColorIndex color) {
    X3D_CameraObject* cam = getCameraObject();
    
    for(int i = 0; i < poly.totalVertices(); ++i) {
        int next = (i + 1) % poly.totalVertices();
        
        X3D_Ray3D ray = x3d_ray3d_make(
            poly.vertices[i].toX3DVex3D(),
            poly.vertices[next].toX3DVex3D()
        );
        
        x3d_ray3d_render(&ray, cam, color);
    }
}

void ViewRenderer::renderRay(Ray ray, X3D_ColorIndex color) {
    X3D_Ray3D x3dRay = x3d_ray3d_make(
        ray.v[0].toX3DVex3D(),
        ray.v[1].toX3DVex3D()
    );
    
    x3d_ray3d_render(&x3dRay, getCameraObject(), color);
}



