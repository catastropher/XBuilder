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

#include <SDL2/SDL.h>

#include "imgui/imgui.h"
#include "SegmentTool.hpp"
#include "gui/widgets.hpp"

class CreateSegmentTool : public SegmentTool {
public:
    CreateSegmentTool(ToolContext& context_) :
        SegmentTool(context_, false),
        unitSelectorWidget(context_.defaultDistanceUnit),
        sidesWidget("Sides in Base", 4, 3, 10, "%.0f sides"),
        lengthWidget("Side Length", DistanceRange(10, 1, 100, Distance::FEET), unitSelectorWidget),
        heightWidget("Height", DistanceRange(10, 1, 100, Distance::FEET), unitSelectorWidget),
        elevationWidget("Center Elevation", DistanceRange(0, -1000, 1000, Distance::FEET), unitSelectorWidget),
        showPreview(false)
        { }
    
    void renderToolWindow() {
        sidesWidget.render();
        unitSelectorWidget.render();
        lengthWidget.render();
        heightWidget.render();
        elevationWidget.render();
        
        showGeometryPreview();
    }
    
private:
    
    Prism3D getNewGeometry() const {
        Prism3D geo = Prism3D::construct(sidesWidget.getValue(), lengthWidget.getDistanceInUnits(), heightWidget.getDistanceInUnits(), Vec3(), newPrismPosition);
        return geo;
    }
    
    void showGeometryPreview() const {
        if(!showPreview)
            return;
        
        Prism3D geo = getNewGeometry();
        ViewRenderer::renderPrism3D(geo, context.colorPalette.geometryPreviewColor);
    }
    
    void viewWindowHandleMouse(MouseState& state) {
        calculatePositionFromMouse(state);
        
        if(state.leftPressed) {
            Prism3D geo = getNewGeometry();
            context.level.addSegment(geo);
        }
    }
    
    void calculatePositionFromMouse(MouseState& state) {
        if(!state.hoverInWindow) {
            showPreview = false;
            return;
        }
        
        X3D_CameraObject* cam = x3d_playermanager_get()->player[0].cam;
        Ray ray = Ray::constructThroughPointOnScreen(state.pos, cam);
        PlaneIntersection intersection;
        
        if(ImGui::IsMouseDown(1)) {
            Plane viewPlane = Plane::constructParallelToCameraView(cam, newPrismPosition);
            
            if(!viewPlane.rayIntersectsPlane(ray, intersection, true)) {
                showPreview = false;
                return;
            }
            
            elevationWidget.setDistance(Distance(intersection.intersection.y, Distance::X3D_UNITS));
            newPrismPosition.y = intersection.intersection.y;
        }
        else {
            Plane plane = Plane::constructHorizontal(elevationWidget.getDistanceInUnits(), true);
            
            if(!plane.rayIntersectsPlane(ray, intersection, true)) {
                showPreview = false;
                return;
            }
            
            newPrismPosition = intersection.intersection;
        }
        
        showPreview = true;
    }
    
    DistanceUnitSelectorWidget unitSelectorWidget;
    IntSliderWidget sidesWidget;
    DistanceSliderInputWidget lengthWidget;
    DistanceSliderInputWidget heightWidget;
    DistanceSliderInputWidget elevationWidget;
    
    bool showPreview;
    Vec3 newPrismPosition;
};

