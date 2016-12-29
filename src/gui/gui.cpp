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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include "texture.hpp"

#include <SDL2/SDL.h>
#include <X3D/X3D.h>
#include <GLES3/gl3.h>
#include <vector>

#include "gui/gui.hpp"
#include "level/Raytracer.hpp"

SDL_Window* window;
SDL_GLContext glcontext;

using namespace std;

extern "C" void test_render_callback(X3D_CameraObject* cam);

extern Level globalLevel;

ToolManager* globalToolManager;



struct TexturePicker {
    vector<LevelTexture*> textures;
    LevelTexture* selectedTexture = NULL;
    char searchText[256] = "";
    
    TexturePicker(const vector<LevelTexture*>& textures_) : textures(textures_) { }
    
    void render() {
        renderSelectedTexture();
        renderSearch();
        renderTextureButtons();
    }
    
    void renderSelectedTexture() {
        if(!selectedTexture)
            return;
        
        ImGui::Image(selectedTexture->getImguiId(), ImVec2(selectedTexture->getWidth(), selectedTexture->getHeight()));
        ImGui::Text("%s", selectedTexture->getName().c_str());
        ImGui::Separator();
    }
    
    void renderSearch() {
        ImGui::InputText("Search", searchText, sizeof(searchText));
    }
    
    void renderTextureButtons() {
        ImGui::BeginChild("textures");
        
        string search(searchText);
        for(int i = 0; i < (int)textures.size(); ++i) {
            if(textures[i]->getName().find(search) != string::npos)
                renderTextureButton(textures[i], i);
        }
        
        ImGui::EndChild();
    }
    
    void renderTextureButton(LevelTexture* tex, int id) {
        char buttonId[64];
        sprintf(buttonId, "texbutton%d", id);
        
        ImGui::BeginGroup();
        
        ImGui::PushID(buttonId);
        if(ImGui::ImageButton(tex->getImguiId(), ImVec2(tex->getWidth(), tex->getHeight()))) {
            selectedTexture = tex;
        }
        
        ImGui::PopID();
        
        ImGui::Text("%s", tex->getName().c_str());
        
        ImGui::EndGroup();
    }
};

void setupWindow() {
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
    window = SDL_CreateWindow("ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    glcontext = SDL_GL_CreateContext(window);
    
    SDL_GL_MakeCurrent(window, glcontext);
    
    ImGui_ImplSdlGL3_Init(window);
}

#include "pack/DirectoryScanner.hpp"

void initGUI() {
    setupWindow();
    
    bool done = X3D_FALSE;
    ImVec4 clear_color = ImColor(114, 144, 154);

    //TextureManager::loadTexture("checkerboard.bmp");
    //TextureManager::loadTexture("floor.bmp");
    
    DirectoryScanner scanner("tex/quaketex", false);
    
    vector<string> files = scanner.recursivelyScanFiles();
    
    for(string file : files) {
        //TextureManager::loadTexture(file);
    }
    
    TexturePicker picker(TextureManager::getTextures());
    
    GuiManager guiManager(globalLevel);

    while(!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdlGL3_ProcessEvent(&event);
            x3d_pc_send_event(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        
        ImGui_ImplSdlGL3_NewFrame(window);
        
//         ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
//         bool show_another_window;
//         ImGui::Begin("Another Window", &show_another_window);
        
        //picker.render();
        
        guiManager.render();
        
//         ImGui::Text("Hello");
//         ImGui::InputText("Extrude Distance", test, 256);
//         
//         ImGui::BeginChild("textures");
//         for(int i = 0; i < 20; ++i)
//             tex->renderInGUI();
        
        //ImGui::EndChild();
        
       // ImGui::End();
        
        
        
        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window);
    }
    
    ImGui_ImplSdlGL3_Shutdown();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

