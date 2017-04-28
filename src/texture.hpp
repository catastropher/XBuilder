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
#include <vector>
#include <string>
#include <GLES3/gl3.h>
#include <map>

class OpenGLTextureManager {    
public:
    static GLuint addX3DTexture(X3D_Texture* tex);
    static void updateX3DTexture(GLuint id, X3D_Texture* updatedTexture);
    void purgeAllTextures();
    
    ~OpenGLTextureManager();
    
private:
    static std::vector<GLuint> loadedTextures;
    static std::vector<unsigned char> convertX3DTextureToByteArray(X3D_Texture* tex);
    static GLuint uploadByteArrayTexture(std::vector<unsigned char>& texturePixels, int texWidth, int texHeight);
};

class LevelTexture {
public:
    LevelTexture(int id_, std::string name_, X3D_Texture tex) : id(id_), name(name_), texture(tex) {
        glTextureId = OpenGLTextureManager::addX3DTexture(&texture);
    }
    
    int getWidth() const {
        return texture.w;
    }
    
    int getHeight() const {
        return texture.h;
    }
    
    int totalTexels() const {
        return texture.w * texture.h;
    }
    
    std::string getName() const {
        return name;
    }
    
    X3D_Texture& getX3DTexture() {
        return texture;
    }
   
    void* getImguiId() const {
        return (void *)(size_t)glTextureId;
    }
    
    void saveToXtexFile(std::string fileName);
    
    ~LevelTexture() {
        x3d_texture_cleanup(&texture);
    }
    
private:
    int id;
    std::string name;
    X3D_Texture texture;
    GLuint glTextureId;
};

class TextureManager {
public:
    LevelTexture* loadTextureFromFile(std::string fileName);
    
    LevelTexture* addTexture(X3D_Texture tex, std::string fileName);
    
    const std::vector<LevelTexture*>& getTextures() {
        return textures;
    }
    
    LevelTexture* getTextureByName(std::string name) {
        for(int i = 0; i < (int)textures.size(); ++i) {
            if(textures[i]->getName() == name)
                return textures[i];
        }
        
        return nullptr;
    }
    
    LevelTexture* getTextureByTextureAddress(X3D_Texture* tex) {
        if(levelTexturesByX3dAddress.count(tex->texels) == 0)
            return nullptr;
        
        return levelTexturesByX3dAddress[tex->texels];
    }
    
    void purgeAllTextures() {
        for(int i = 0; i < (int)textures.size(); ++i)
            delete textures[i];
        
        textures.clear();        
    }
    
    void exportAllTexturesToDirectory(std::string directory) {
        for(LevelTexture* tex : textures)
            tex->saveToXtexFile(directory + "/" + tex->getName() + ".xtex");
    }
    
    void recursivelyImportTexturesFromDirectory(std::string directory);
    
    ~TextureManager() {
        purgeAllTextures();
    }
    
private:
    std::vector<LevelTexture*> textures;
    std::map<X3D_ColorIndex*, LevelTexture*> levelTexturesByX3dAddress;
};



