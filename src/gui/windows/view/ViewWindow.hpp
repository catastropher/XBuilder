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

#include <GLES3/gl3.h>
#include <X3D/X3D.h>

#include "imgui/imgui.h"
#include "../Window.hpp"
#include "gui/ViewRenderer.hpp"

#include "texture.hpp"

extern "C" void test_render_callback(X3D_CameraObject*);

class ViewWindow {
private:
    GLuint renderTextureId;
    WindowContext& context;
    MouseState currentMouseState;
    
public:
    ViewWindow(WindowContext& context_) : context(context_) {
        X3D_Texture tex = createTextureForRenderOuput();
        renderTextureId = OpenGLTextureManager::addX3DTexture(&tex);
    }
    
    void beginRender() {
        x3d_screen_clear(0);
    }
    
    void renderSegmentsInLevel() {
        X3D_ColorIndex red = x3d_color_to_colorindex(x3d_rgb_to_color(255, 0, 0));
        ViewRenderer::renderAllSegmentsInLevel(context.level, red);
    }
    
    void renderWindow() {
        x3d_read_keys();
        
        x3d_keymanager_get()->key_handler();
        updateRenderOutputTexture();
        
        ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiSetCond_Always);
        bool show_another_window = true;
        ImGui::Begin("X3D", &show_another_window, ImVec2(640, 480), -1.0f);
        
        X3D_Texture tex = createTextureForRenderOuput();
        ImGui::Image((void*)(size_t)renderTextureId, ImVec2(tex.w, tex.h));
        
        updateMouseState();
        
        ImGui::End();
    }
    
    MouseState getMouseStateRelativeToWindow() {
        return currentMouseState;
    }
    
private:
    X3D_Texture createTextureForRenderOuput() {
        X3D_ScreenManager* screenman = x3d_screenmanager_get();
        X3D_Texture tex;
        
        tex.texels = screenman->buf;
        tex.w = screenman->w;
        tex.h = screenman->h;
        
        return tex;
    }
    
    void updateRenderOutputTexture() {
        X3D_Texture tex = createTextureForRenderOuput();
        OpenGLTextureManager::updateX3DTexture(renderTextureId, &tex);
    }
    
    void updateMouseState() {
        ImVec2 mousePos = ImGui::GetMousePos();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 relativePos = ImVec2(mousePos.x - windowPos.x, mousePos.y - windowPos.y);    
        
        X3D_Vex2D pos = { (int)relativePos.x - 10, (int)relativePos.y - 30 };
        
        currentMouseState = MouseState(ImGui::IsItemClicked(0), ImGui::IsItemClicked(1), pos, ImGui::IsItemHovered());
    }
};

    