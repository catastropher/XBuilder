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

class LevelTexture {
private:
    int id;
    std::string name;
    X3D_Texture texture;
    
public:
    LevelTexture(int id_, std::string name_, X3D_Texture tex) : id(id_), name(name_), texture(tex) { }
    
    int getWidth() const {
        return texture.w;
    }
    
    int getHeight() const {
        return texture.h;
    }
    
    const X3D_Texture getX3DTexture() const {
        return texture;
    }
};

class TextureManager {
private:
    std::vector<LevelTexture*> textures;
    
public:
    bool loadTexture(std::string fileName);
};

