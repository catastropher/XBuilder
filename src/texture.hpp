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


#include <X3D/X3D.h>
#include <vector>
#include <string>
#include <GLES3/gl3.h>

class OpenGLTextureManager {
private:
    static std::vector<GLuint> loadedTextures;
    
    static std::vector<unsigned char> convertX3DTextureToByteArray(X3D_Texture* tex);
    static GLuint uploadByteArrayTexture(std::vector<unsigned char>& texturePixels, int texWidth, int texHeight);
    
public:
    static GLuint addX3DTexture(X3D_Texture* tex);
    
    ~OpenGLTextureManager();
};

class LevelTexture {
private:
    int id;
    std::string name;
    X3D_Texture texture;
    GLuint glTextureId;
    
    void createOpenGLTexture();

    
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
    
    std::string getName() const {
        return name;
    }
    
    const X3D_Texture getX3DTexture() const {
        return texture;
    }
   
    void* getImguiId() const {
        return (void *)(GLuint)glTextureId;
    }
   
    void renderInGUI();
    
    ~LevelTexture() {
        x3d_texture_cleanup(&texture);
    }
};

class TextureManager {
private:
    static std::vector<LevelTexture*> textures;
    
public:
    static LevelTexture* loadTexture(std::string fileName);
    static LevelTexture* addTexture(X3D_Texture tex, std::string fileName);
    
    static const std::vector<LevelTexture*>& getTextures() {
        return textures;
    }
};



