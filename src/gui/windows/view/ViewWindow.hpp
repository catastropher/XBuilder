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
#include "geo/Distance.hpp"

#include "texture.hpp"

extern "C" void test_render_callback(X3D_CameraObject*);

class ViewWindow : public Window {
public:
    ViewWindow(WindowContext& context_) : Window(context_, "X3D", true) {
        screenTexture = *x3d_screenmanager_get_screen(x3d_screenmanager_get());
        renderTextureId = OpenGLTextureManager::addX3DTexture(&screenTexture);
    }
    
    void beginRender() {
        x3d_screen_clear(0);
        x3d_screen_zbuf_clear();
        renderAxes();
    }
    
    void renderSegmentsInLevel() {
        X3D_ColorIndex red = x3d_color_to_colorindex(x3d_rgb_to_color(255, 0, 0));
        ViewRenderer::renderAllSegmentsInLevel(context.project.getLevel(), red);
    }
    
    void render() {
        x3d_read_keys();
        x3d_keymanager_get()->key_handler();
        
        updateRenderOutputTexture();
        ImGui::Image((void*)(size_t)renderTextureId, ImVec2(screenTexture.w, screenTexture.h));        
        updateMouseState();
    }
    
    MouseState getMouseStateRelativeToWindow() {
        return currentMouseState;
    }
    
private:    
    void renderAxes() {
        float size = Distance(10, Distance::FEET).toUnit(Distance::X3D_UNITS).dist;
        
        Vec3 origin(0, 0, 0);
        Vec3 x(size, 0, 0);
        Vec3 y(0, -size, 0);
        Vec3 z(0, 0, size);
        
        X3D_ColorIndex red = 251;
        X3D_ColorIndex green = 63;
        X3D_ColorIndex blue = 208;
        
        ViewRenderer::renderRay(Ray(origin, x), red);
        ViewRenderer::renderRay(Ray(origin, y), green);
        ViewRenderer::renderRay(Ray(origin, z), blue);
    }
    
    void updateRenderOutputTexture() {
        OpenGLTextureManager::updateX3DTexture(renderTextureId, &screenTexture);
    }
    
    void updateMouseState() {
        ImVec2 mousePos = ImGui::GetMousePos();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 relativePos = ImVec2(mousePos.x - windowPos.x, mousePos.y - windowPos.y);    
        
        X3D_Vex2D pos = { (int)relativePos.x - 10, (int)relativePos.y - 30 };
        
        currentMouseState = MouseState(ImGui::IsItemClicked(0), ImGui::IsItemClicked(1), pos, ImGui::IsItemHovered(), { (int)windowPos.x, (int)windowPos.y});
    }
    
    GLuint renderTextureId;
    MouseState currentMouseState;
    X3D_Texture screenTexture;
    X3D_Surface surface;
    
    X3D_SurfaceTexture surfaceTextures[2];
    
};

    