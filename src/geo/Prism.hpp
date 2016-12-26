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

#include <vector>

#include "Vec3.hpp"
#include "Polygon.hpp"

struct Prism3D {
    enum {
        BASE_A = 0,
        BASE_B = 1
    };
    
    int baseVertices() const {
        return vertices.size() / 2;
    }
    
    Polygon3D getFace(int faceId) const {
        if(faceId == BASE_A)
            return getFace(faceId);
        
        if(faceId == BASE_B)
            return getBaseBReversed();
        
        return getSideFace(faceId);
    }
    
    void setFace(int faceId, Polygon3D& poly) {
        if(faceId == BASE_A)
            setBaseA(poly);
        
        if(faceId == BASE_B)
            setBaseBReversed(poly);
        
        setSideFace(faceId, poly);
    }
    
private:
    Polygon3D getBaseA() const {
        Polygon3D poly;
        for(int i = 0; i < baseVertices(); ++i)
            poly.addPoint(vertices[i]);
        
        return poly;
    }
    
    void setBaseA(Polygon3D& poly) {
        for(int i = 0; i < baseVertices(); ++i)
            vertices[i] = poly.vertices[i];
    }
    
    Polygon3D getBaseBReversed() const {
        Polygon3D poly;
        for(int i = baseVertices() - 1; i >= 0; --i)
            poly.addPoint(vertices[i + baseVertices()]);
        
        return poly;
    }
    
    void setBaseBReversed(Polygon3D& poly) {
        for(int i = baseVertices() - 1; i >= 0; --i)
            vertices[i + baseVertices()] = poly.vertices[baseVertices() - i - 1];
    }
    
    void getSideFaceVertexIds(int faceId, int idDest[4]) const {
        uint16 sideId = faceId - 2;
        uint16 nextId = x3d_int16_increment_wrap(sideId, baseVertices());
        
        idDest[0] = sideId;
        idDest[1] = sideId + baseVertices();
        idDest[2] = nextId + baseVertices();
        idDest[3] = nextId;
    }
    
    Polygon3D getSideFace(int faceId) const {
        Polygon3D poly;
        int vertexIds[4];
        getSideFaceVertexIds(faceId, vertexIds);
        
        for(int i = 0; i < 4; ++i)
            poly.addPoint(vertices[vertexIds[i]]);
        
        return poly;
    }
    
    void setSideFace(int faceId, Polygon3D& poly) {
        int vertexIds[4];
        getSideFaceVertexIds(faceId, vertexIds);
        
        for(int i = 0; i < 4; ++i)
            vertices[vertexIds[i]] = poly.vertices[i];
    }
    
    std::vector<Vec3> vertices;
};


