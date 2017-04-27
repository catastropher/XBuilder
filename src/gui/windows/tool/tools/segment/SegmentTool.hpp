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
#include <iostream>
#include <string>

#include "level/Level.hpp"
#include "gui/widgets.hpp"
#include "../Tool.hpp"
#include "level/Raytracer.hpp"
#include "gui/ViewRenderer.hpp"

class SegmentTool : public Tool {
public:
    SegmentTool(ToolContext& context_, bool requireSelectedSeg_ = true) : Tool(context_), requireSelectedSeg(requireSelectedSeg_) { }
    
    virtual void updateSelectedSeg() { }
    
    virtual void viewWindowHandleMouse(MouseState& state) {
        X3D_CameraObject* viewCamera = x3d_playermanager_get()->player[0].cam;
        
        if(state.leftPressed) {
            Raytracer raytracer(context.project.getLevel(), viewCamera, state.pos);
            
            if(raytracer.findClosestIntersectedFace(selectedFace))
                updateSelectedSeg();
        }
    }
    
    bool segIsSelected() const {
        return selectedFace.validIntersection();
    }
    
    void renderSelectedSeg() {
        if(segIsSelected()) {
            Prism3D geo = selectedFace.face->getSeg().getGeometry();
            ViewRenderer::renderPrism3D(geo, context.colorPalette.primarySelectColor);
        }
    }
    
    Segment& getSelectedSeg() const {
        return selectedFace.face->getSeg();
    }
    
    bool requiresSelectedSeg() const {
        return requireSelectedSeg;
    }
    
    void deselectSeg() {
        selectedFace.face = nullptr;
    }

protected:
    Raytracer::FaceIntersection selectedFace;
    bool requireSelectedSeg;
};

struct SegmentToolGroup : ToolGroup {
    SegmentToolGroup(ToolContext& context_) : ToolGroup(context_) {
        toolDropDown.addItem("create", "Create");
    }
    
    void setSelectedTool(std::string name);
    
    void renderToolWindow() {
        if(!selectedTool)
            return;
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Segment -> %s", toolDropDown.getSelectedDisplayName().c_str());
        ImGui::Separator();
        
        if(SegmentTool* tool = dynamic_cast<SegmentTool*>(selectedTool)) {
            if(tool->requiresSelectedSeg() && !tool->segIsSelected()) {
                ImGui::Text("Please select a segment :(");
                return;
            }
        }
        
        ToolGroup::renderToolWindow();
    }
};


