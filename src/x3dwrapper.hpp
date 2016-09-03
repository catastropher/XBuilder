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

#include "X3D.h"

// void x3d_polygon3d_translate_normal(X3D_Polygon3D* poly, X3D_Normal3D* dir, int16 dist);
// void x3d_polygon3d_translate(X3D_Polygon3D* poly, X3D_Vex3D shift);
// 
// void x3d_polygon3d_scale(X3D_Polygon3D* poly, fp8x8 scale);
// void x3d_polygon3d_rotate(X3D_Polygon3D* poly, X3D_Vex3D_angle256 angle, X3D_Vex3D center);
// void x3d_polygon3d_copy(X3D_Polygon3D* src, X3D_Polygon3D* dest);
// void x3d_polygon3d_render(X3D_Polygon3D* poly, X3D_PolygonAttributes* att, struct X3D_CameraObject* cam, struct X3D_RasterRegion* parent);

struct XVex3D : X3D_Vex3D {
    XVex3D() { }
    XVex3D(X3D_Vex3D& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
};

struct XPlane : X3D_Plane {
};

struct XPolygon3D : X3D_Polygon3D {
    XPolygon3D(int total_v_) {
        total_v = total_v_;
        v = new XVex3D[total_v];
    }
    
    XPolygon3D(X3D_Polygon3D& poly) : XPolygon3D(poly.total_v) {
        for(int i = 0; i < poly.total_v; ++i)
            v[i] = poly.v[i];
    }
    
    void translate(XVex3D shift) { x3d_polygon3d_translate(this, shift); }
    void translateNormal(X3D_Normal3D* dir, int16 dist) { x3d_polygon3d_translate_normal(this, dir, dist); }
    void reverse() { x3d_polygon3d_reverse(this); }
    void scale(fp8x8 scale) { x3d_polygon3d_scale(this, scale); }
    
    XVex3D center() {
        XVex3D cen;
        x3d_polygon3d_center(this, &cen);
        return cen;
    }
    
    XPlane calculatePlane() {
        XPlane plane;
        x3d_polygon3d_calculate_plane(this, &plane);
        return plane;
    }
    
    ~XPolygon3D() {
        delete [] v;
    }
};

struct XPrism3D : X3D_Prism3D {
    explicit XPrism3D(int base_v_) {
        base_v = base_v_;
        v = new XVex3D[base_v * 2];
    }
    
    XPrism3D(const X3D_Prism3D& prism) : XPrism3D(prism.base_v) {
        for(int i = 0; i < prism.base_v * 2; ++i)
            v[i] = prism.v[i];
    }
    
    XPrism3D(const XPrism3D& prism) : XPrism3D(prism.base_v) {
        for(int i = 0; i < prism.base_v * 2; ++i)
            v[i] = prism.v[i];
    }
    
    static XPrism3D construct(uint16 steps, uint16 r, int16 h, X3D_Vex3D_angle256 rot_angle) {
        XPrism3D prism(steps);
        x3d_prism3d_construct(&prism, steps, r, h, rot_angle);
        return prism;
    }
    
    void setFace(uint16 face, XPolygon3D& src) { x3d_prism3d_set_face(this, face, &src); }
    
    int faceVertexCount(uint16 face) {
        if(face == X3D_BASE_A || face == X3D_BASE_B)
            return base_v;
        
        return 4;
    }
    
    XPolygon3D getFace(uint16 face) {
        XPolygon3D poly(faceVertexCount(face));
        x3d_prism3d_get_face(this, face, &poly);
        return poly;
    }
    
    ~XPrism3D() {
        delete [] v;
    }
};

