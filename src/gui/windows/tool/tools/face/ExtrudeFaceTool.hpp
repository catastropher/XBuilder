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

#include "imgui/imgui.h"
#include "FaceTool.hpp"

class ExtrudeFaceTool : public FaceTool {
public:
    ExtrudeFaceTool(ToolContext& context_) : FaceTool(context_),
        unitSelectorWidget(context_.defaultDistanceUnit),
        extrudeDistanceWidget("Extrude Distance",
            Distance(10, Distance::FEET),
            Distance(0, Distance::FEET), 
            Distance(100, Distance::FEET),
            unitSelectorWidget),
            extrudeType(EXTRUDE_NORMAL) { }
    
    void renderToolWindow() {
        unitSelectorWidget.render();
        extrudeDistanceWidget.render();
        
        float extrudeDist = getExtrudeDistance();
        
        bool deselect = false;
        
        renderExtrudeType();
        
        if(ImGui::Button("Extrude!")) {
            Prism3D geo = getExtrudedGeometry();
            selectedFace.face->extrude(geo);
            deselect = true;
        }
        
        if(extrudeDist >= 0)
            showGeometryPreview();
        
        if(deselect)
            deselectFace();
        
        renderSelectedFace();
    }
    
private:
    enum ExtrudeType {
        EXTRUDE_NORMAL,
        EXTRUDE_KEEP_HORIZTONAL,
        EXTRUDE_KEEP_VERTICAL
    };
    
    void renderExtrudeType() {
        ImGui::RadioButton("Normal", (int *)&extrudeType, EXTRUDE_NORMAL);
        ImGui::SameLine();
        ImGui::RadioButton("Keep horizontal", (int *)&extrudeType, EXTRUDE_KEEP_HORIZTONAL);
        ImGui::SameLine();
        ImGui::RadioButton("Keep vertical", (int *)&extrudeType, EXTRUDE_KEEP_VERTICAL);
    }
    
    Prism3D getExtrudedGeometry() const {
        Prism3D geo = selectedFace.face->getSeg().getGeometry().createPrism3DFromExtrudedFace(geo, selectedFace.face->getId(), getExtrudeDistance());
        
        applyExtrudeType(geo);
        
        return geo;
    }
    
    void applyExtrudeType(Prism3D& prism) const {
        if(extrudeType == EXTRUDE_NORMAL)
            return;
        
        Polygon3D baseA = prism.getFace(Prism3D::BASE_A);
        Polygon3D baseB = prism.getFace(Prism3D::BASE_B);
        
        Plane plane;
        
        if(extrudeType == EXTRUDE_KEEP_HORIZTONAL) {
            plane = calculateHorizontalExtrudePlaneForPolygon(baseB, baseA);
            
            baseB.reverse();
            
            for(int i = 0; i < baseB.totalVertices(); ++i)
                baseB.vertices[i].y = baseA.vertices[i].y; 
            
            baseB.reverse();
        }
        else if(extrudeType == EXTRUDE_KEEP_VERTICAL) {
            plane = calculateVerticalExtrudePlaneForPolygon(baseB, baseA);
            
            baseB.reverse();
            
            for(int i = 0; i < baseB.totalVertices(); ++i) {
                baseB.vertices[i].x = baseA.vertices[i].x;
                baseB.vertices[i].z = baseA.vertices[i].z;
            }
            
            baseB.reverse();
        }
        
        Polygon3D newBaseB = projectPolygonOntoPlane(baseB, plane);

        newBaseB.translateAlongNormal(-getExtrudeDistance());
        
        prism.setFace(Prism3D::BASE_B, newBaseB);
    }
    
    Plane calculateHorizontalExtrudePlaneForPolygon(Polygon3D& poly, Polygon3D& other) const {
        Plane plane = poly.calculatePlane();
        
        plane.normal.y = 0;
        plane.normal = plane.normal.normalize();
        
        float minDist = 1000000;
        
        for(int i = 0; i < other.totalVertices(); ++i) {
            float dist = plane.normal.dot(other.vertices[i]);
            
            if(dist < minDist) {
                minDist = dist;
                plane.d = -dist;
            }
        }
        
        return plane;
    }
    
    Plane calculateVerticalExtrudePlaneForPolygon(Polygon3D& poly, Polygon3D& other) const {
        Plane plane = poly.calculatePlane();
        
        plane.normal.x = 0;
        plane.normal.z = 0;
        plane.normal = plane.normal.normalize();
        
        float minDist = 1000000;
        
        for(int i = 0; i < other.totalVertices(); ++i) {
            float dist = plane.normal.dot(other.vertices[i]);
            
            if(dist < minDist) {
                minDist = dist;
                plane.d = -dist;
            }
        }
        
        return plane;
    }
    
    Polygon3D projectPolygonOntoPlane(Polygon3D poly, Plane plane) const {
        for(int i = 0; i < poly.totalVertices(); ++i)
            poly.vertices[i] = projectPointOntoPlane(poly.vertices[i], plane);
        
        return poly;
    }
    
    Vec3 projectPointOntoPlane(Vec3 v, Plane plane) const {
        Ray ray(v, v - plane.normal);
        PlaneIntersection inter;
        plane.rayIntersectsPlane(ray, inter, true);
        
        return inter.intersection;
    }
    
    void showGeometryPreview() const {
        Prism3D newSegGeometry = getExtrudedGeometry();
        ViewRenderer::renderPrism3D(newSegGeometry, context.colorPalette.geometryPreviewColor);
    }
    
    float getExtrudeDistance() const {
        return extrudeDistanceWidget.getDistance().toUnit(Distance::X3D_UNITS).dist;
    }
    
    DistanceUnitSelectorWidget unitSelectorWidget;
    DistanceSliderInputWidget extrudeDistanceWidget;
    ExtrudeType extrudeType;
};

