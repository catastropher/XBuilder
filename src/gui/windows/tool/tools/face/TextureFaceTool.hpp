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

class TextureFaceTool : public FaceTool {
public:
    TextureFaceTool(ToolContext& context_)
    : FaceTool(context_, false),
    texturePickerWidget(TextureManager::getTextures()),
    offsetX(0),
    offsetY(0),
    angle(0)
    {
        
    }
    
    void renderToolWindow() {
        texturePickerWidget.render();
        texturePickerWidget.renderOpenModalButton();
        
        ImGui::DragInt("Offset X", &offsetX, .1);
        ImGui::DragInt("Offset Y", &offsetY, .1);
        ImGui::DragInt("Angle", &angle, .1);
    }
    
private:
    TexturePickerWidget texturePickerWidget;
    int offsetX;
    int offsetY;
    int angle;
};

