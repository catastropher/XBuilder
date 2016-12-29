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

struct FaceTool : Tool {
    Raytracer::FaceIntersection selectedFace;
    bool requireSelectedFace;
    
    FaceTool(ToolContext& context_, bool requireSelectedFace_ = true) : Tool(context_), requireSelectedFace(requireSelectedFace_) { }
    
    virtual void updateSelectedFace() { }
    
    virtual void viewWindowHandleMouse(MouseState& state) {
        X3D_CameraObject* viewCamera = x3d_playermanager_get()->player[0].cam;
        
        if(state.leftPressed) {
            Raytracer raytracer(context.level, viewCamera, state.pos);
            
            if(raytracer.findClosestIntersectedFace(selectedFace))
                updateSelectedFace();
        }
    }
    
    bool faceIsSelected() const {
        return selectedFace.validIntersection();
    }
};

struct FaceToolGroup : ToolGroup {
    FaceToolGroup(ToolContext& context_) : ToolGroup(context_) {
        toolDropDown.addItem("extrude", "Extrude");
        toolDropDown.addItem("scale", "Scale");
        toolDropDown.addItem("connect", "Connect");
    }
    
    void setSelectedTool(std::string name);
    
    void renderToolWindow() {
        if(!selectedTool)
            return;
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Face -> %s", toolDropDown.getSelectedDisplayName().c_str());
        ImGui::Separator();
        
        if(FaceTool* tool = dynamic_cast<FaceTool*>(selectedTool)) {
            if(tool->requireSelectedFace && !tool->faceIsSelected()) {
                ImGui::Text("Please select a face :(");
                return;
            }
        }
        
        ToolGroup::renderToolWindow();
    }
};


