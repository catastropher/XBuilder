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

#include <string>
#include <vector>
#include <X3D/X3D.h>
#include <boost/filesystem.hpp>

#include "texture.hpp"
#include "imgui/imgui.h"

using namespace std;
using namespace boost::filesystem;

vector<GLuint> OpenGLTextureManager::loadedTextures;

vector<unsigned char> OpenGLTextureManager::convertX3DTextureToByteArray(X3D_Texture* tex) {
    vector<unsigned char> texturePixels;
    
    for(int i = 0; i < (int)x3d_texture_total_texels(tex); ++i) {
        uint8 r, g, b;
        X3D_ColorIndex texel = tex->texels[i];
        x3d_color_to_rgb(x3d_colorindex_to_color(texel), &r, &g, &b);
        
        texturePixels.push_back(r);
        texturePixels.push_back(g);
        texturePixels.push_back(b);
        texturePixels.push_back(255);
    }
    
    return texturePixels;
}

void OpenGLTextureManager::updateX3DTexture(GLuint id, X3D_Texture* updatedTexture) {
    vector<unsigned char> texturePixels = convertX3DTextureToByteArray(updatedTexture);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, updatedTexture->w, updatedTexture->h, GL_RGBA, GL_UNSIGNED_BYTE, &texturePixels[0]);
}

void OpenGLTextureManager::purgeAllTextures() {
    glDeleteTextures(loadedTextures.size(), &loadedTextures[0]);
    loadedTextures.clear();
}


OpenGLTextureManager::~OpenGLTextureManager() {
    purgeAllTextures();
}

GLuint OpenGLTextureManager::uploadByteArrayTexture(vector<unsigned char>& texturePixels, int texWidth, int texHeight) {
    GLuint texId;
    
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    //glTexParameteri(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texturePixels[0]);
    
    loadedTextures.push_back(texId);
    
    return texId;
}

GLuint OpenGLTextureManager::addX3DTexture(X3D_Texture* tex) {
    vector<unsigned char> texturePixels = convertX3DTextureToByteArray(tex);
    return uploadByteArrayTexture(texturePixels, tex->w, tex->h);
}

LevelTexture* TextureManager::addTexture(X3D_Texture tex, string fileName) {
    int newTexId = textures.size();
    LevelTexture* levelTex = new LevelTexture(newTexId, fileName, tex);
    
    textures.push_back(levelTex);
    levelTexturesByX3dAddress[levelTex->getX3DTexture().texels] = levelTex;
    
    return levelTex;
}


LevelTexture* TextureManager::loadTextureFromFile(string fileName) {
    X3D_Texture x3dTex;
    if(!x3d_texture_load_from_bmp_file(&x3dTex, fileName.c_str())) {
        return NULL;
    }
    
    string name = path(fileName).filename().string();
    
    int lastDotIndex = name.find_last_of("."); 
    
    if(lastDotIndex != string::npos) {
        name = name.substr(0, lastDotIndex); 
    }
    
    return addTexture(x3dTex, name);
}

