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
    ViewRenderPrismFlags flags;
    
    for(int i = 0; i < seg.getGeometry().totalFaces(); ++i) {
        if(seg.getFace(i).isConnectedToSegmentFace())
            flags = flags.disableFace(i);
    }
    
    Prism3D prism = seg.getGeometry();
    renderPrism3D(prism, color, flags);
}

void ViewRenderer::renderPrism3D(Prism3D& prism, X3D_ColorIndex color, ViewRenderer::ViewRenderPrismFlags prismFlags) {
//     X3D_Vex3D v[32];
//     X3D_Prism3D x3dPrism;
//     x3dPrism.v = v;
//     
     X3D_CameraObject* cam = getCameraObject();
//     
//     prism.toX3DPrism3D(&x3dPrism);
//     x3d_prism3d_render_wireframe(&x3dPrism, cam, color);
    
    for(int i = 0; i < prism.totalFaces(); ++i) {
        if(!prismFlags.faceIsEnabled(i))
            continue;
        
        Polygon3D poly = prism.getFace(i);
        X3D_Polygon3D x3dPoly;
        X3D_Vex3D v[32];
        x3dPoly.v = v;
        
        poly.toX3DPolygon3D(x3dPoly);
        
        Vec3 lightDir = Vec3(0, 0, 1).normalize();
        
        Plane plane = poly.calculatePlane();
        
        uint8 shade = std::max(0.0f, plane.normal.dot(lightDir)) * 15 + 3;
        
        //shade = (shade + 1) % 15;
        
//         for(int i = 0; i < 256; ++i) {
//             for(int j = 0; j < 16; ++j) {
//                 x3d_screen_draw_pix(i, j, x3d_colormap_get_index(i, j));
//             }
//         }
        
        //printf("\nRender %d\n", i);
        for(int j = 0; j < poly.totalVertices(); ++j) {
          //  printf("\t%d %d %d\n", x3dPoly.v[j].x, x3dPoly.v[j].y, x3dPoly.v[j].z);
        }
        
        x3d_render_flat_shaded_polygon(&x3dPoly, cam, x3d_colormap_get_index(color, shade));
    }
    
    //printf("Shade: %d\n", (int)shade);
    
    //x3d_render_flat_shaded_polygon(&x3dPoly, cam, colorx3d_colormap_get_index(color, shade));
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



