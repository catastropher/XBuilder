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

#include <vector>
#include <string>

#include "level/Level.hpp"
#include "texture.hpp"
#include "imgui/imgui.h"

struct TexturePickerWidget {
public:
    TexturePickerWidget(const std::vector<LevelTexture*>& textures_, LevelTexture* selectedTexture_ = nullptr)
        : textures(textures_),
        selectedTexture(selectedTexture_),
        lastSelectedTexture(selectedTexture_),
        finalSelectedTexture(selectedTexture_),
        tileSize(128, 128),
        horizontalSpacing(10),
        isOpen(false) {
            
        }
    
    void render() {
        ImGui::SetNextWindowSize(ImVec2((tileSize.x + horizontalSpacing) * 4 + 60, tileSize.x * 3));
        if(ImGui::BeginPopupModal("Pick Texture", &isOpen)) {
            renderSearch();
            renderSelectButton();
            renderTextureButtons();
            ImGui::EndPopup();
        }
    }
    
    void show() {
        isOpen = true;
        lastSelectedTexture = finalSelectedTexture;
        ImGui::OpenPopup("Pick Texture");
    }
    
    bool selectedTextureChanged() const {
        return finalSelectedTexture != lastSelectedTexture;
    }
    
    LevelTexture* getSelectedTexture() const {
        return finalSelectedTexture;
    }
    
    void renderOpenModalButton() {
        LevelTexture* tex = getSelectedTexture();
        
        if(tex) {
            ImVec2 dim = calculateTextureButtonDimensions(tex);
            
            if(ImGui::ImageButton(tex->getImguiId(), dim)) {
                show();
            }
        }
        else {
            if(ImGui::Button("Select Texture")) {
                show();
            }
        }
    }
    
private:
    std::vector<LevelTexture*> filterTextures() {
        std::vector<LevelTexture*> matches;
        
        for(LevelTexture* tex : textures) {
            if(tex->getName().find(searchText) != std::string::npos)
                matches.push_back(tex);
        }
        
        return matches;
    }
    
    void renderSearch() {
        ImGui::InputText("Search", searchText, sizeof(searchText));
    }
    
    void renderSelectButton() {
        ImGui::SameLine();
        if(ImGui::Button("Select!")) {
            ImGui::CloseCurrentPopup();
            finalSelectedTexture = selectedTexture;
        }
    }
    
    void renderTextureButtons() {
        ImGui::BeginChild("textures");
        ImGui::Columns(4);
        
        std::vector<LevelTexture*> matches = filterTextures();
        
        for(int i = 0; i < (int)matches.size(); i += 4) {
            renderTextureButtonRow(&matches[i], &matches[matches.size()]);
        }
        
        ImGui::EndChild();
    }
    
    void renderTextureButtonRow(LevelTexture** rowStart, LevelTexture** end) {
        LevelTexture** rowEnd = rowStart + 4;
        int columnWidth = 18;
        
        for(LevelTexture** tex = rowStart; tex != rowEnd; ++tex) {
            if(tex < end)
                renderTextureButton(*tex, (int)(end - tex));
            
            ImGui::NextColumn();
        }
        
        for(LevelTexture** tex = rowStart; tex != rowEnd; ++tex) {
            if(tex < end)
                ImGui::Text("%s", centerTextInColumn((*tex)->getName(), columnWidth).c_str());
            
            ImGui::NextColumn();
        }
        
        for(LevelTexture** tex = rowStart; tex != rowEnd; ++tex) {
            if(tex < end) {
                char widthString[64];
                sprintf(widthString, "%dx%d", (*tex)->getWidth(), (*tex)->getHeight());
                
                ImGui::Text("%s", centerTextInColumn(widthString, columnWidth).c_str());
            }
                
            ImGui::NextColumn();
        }
        
        bool lastRow = rowEnd >= end;
        
        if(!lastRow)
            ImGui::Separator();
    }
    
    std::string centerTextInColumn(std::string str, int widthInChars) {
        std::string centeredString;
        
        for(int i = 0; i < (int)(widthInChars / 2 - str.length() / 2); ++i)
            centeredString += ' ';
        
        return centeredString + str;
    }
    
    ImVec2 calculateTextureButtonDimensions(LevelTexture* tex) const {
        ImVec2 size(tex->getWidth(), tex->getHeight());
        
        if(size.x < tileSize.x && size.y < tileSize.y)
            return size;
        
        float ratio = size.x / size.y;
        
        if(size.x > size.y)
            return ImVec2(tileSize.x, tileSize.x / ratio);
        
        return ImVec2(tileSize.y * ratio, tileSize.y);
    }
    
    void renderTextureButton(LevelTexture* tex, int id) {
        char buttonId[64];
        sprintf(buttonId, "texbutton%d", id);
        
        ImGui::BeginGroup();
        ImGui::PushID(buttonId);
        
        ImVec2 dim = calculateTextureButtonDimensions(tex);
        
        bool selected = (tex == selectedTexture);
        if(!selected)
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        else
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.5, .5, .5, 1));
        
        if(ImGui::ImageButton(tex->getImguiId(), dim))
            selectedTexture = tex;
        
        ImGui::PopStyleColor();
        
        ImGui::PopID();
        ImGui::EndGroup();
    }
    
    std::vector<LevelTexture*> textures;
    LevelTexture* selectedTexture;
    LevelTexture* lastSelectedTexture;
    LevelTexture* finalSelectedTexture;
    char searchText[256] = "";
    ImVec2 tileSize;
    int horizontalSpacing;
    bool isOpen;
};

