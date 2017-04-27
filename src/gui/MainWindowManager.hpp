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
#include <X3D/X3D.h>
#include <GLES3/gl3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

class MainWindowManager {
public:
    MainWindowManager() : userQuit(false), windowInitialized(false) { }
    
    void beginFrame() {
        if(!windowInitialized)
            return;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSdlGL3_ProcessEvent(&event);
            x3d_pc_send_event(&event);
            
            if(event.type == SDL_QUIT)
                userQuit = true;
        }
        
        ImGui_ImplSdlGL3_NewFrame(window);
    }
    
    void endFrame() {
        if(!windowInitialized)
            return;
        
        ImVec4 backgroundColor = ImColor(114, 144, 154);
        
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window);
    }
    
    bool userWantsToQuit() const {
        return userQuit;
    }
    
    void openMainWindow() {
        if(windowInitialized)
            return;
        
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
        
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        
        SDL_DisplayMode current;
        SDL_GetCurrentDisplayMode(0, &current);
        window = SDL_CreateWindow("XBuilder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        glcontext = SDL_GL_CreateContext(window);
        
        SDL_GL_MakeCurrent(window, glcontext);
        
        ImGui_ImplSdlGL3_Init(window);
        
        windowInitialized = true;
    }
    
    void closeMainWindow() {
        if(!windowInitialized)
            return;
        
        ImGui_ImplSdlGL3_Shutdown();
        SDL_GL_DeleteContext(glcontext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        
        windowInitialized = false;
    }
    
    ~MainWindowManager() {
        closeMainWindow();
    }
    
private:
    bool userQuit;
    bool windowInitialized;
    SDL_Window* window;
    SDL_GLContext glcontext;
};

