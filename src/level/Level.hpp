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
#include <X3D/X3D.h>
#include <map>

#include "geo/geo.hpp"
#include "texture.hpp"

namespace Level {

class Level;
struct SegmentEditor;
struct FaceEditor;
    
struct Vertex {
    Vec3 v;
    
    Vertex(Vec3 v_) : v(v_) { }
    
    bool operator<(const Vertex& vertex) const {
        float a[3] = { v.x, v.y, v.z };
        float b[3] = { vertex.v.x, vertex.v.y, vertex.v.z };
        
        for(int i = 0; i < 3; ++i) {
            if(a[i] < b[i])
                return true;
            else if(a[i] > b[i])
                return false;
        }
        
        return false;
    }
};

struct Face {
    
};

struct Segment {
    int id;
};

class SegmentEditor {
public:
    SegmentEditor(Level& level_, Segment& seg_);
    
    FaceEditor getEditorForFace(int faceId);
    
private:
    Level& level;
    Segment& seg;
};


class FaceEditor {
public:
    FaceEditor(SegmentEditor& segEditor_, int faceId_);
    
private:
    SegmentEditor segEditor;
    int faceId;
};


class Level {
public:
    SegmentEditor getEditorForSegment(int segId);
    FaceEditor getEditorForFace(int segId, int faceId);
    
private:
    Segment& getSegment(int segId) {
        return *segments[segId];
    }
    
    int addVertex(Vertex v) {
        if(vertexMap.count(v) != 0)
            return vertexMap[v];
        
        int newVertexId = vertices.size();
        
        vertices.push_back(v);
        vertexMap[v] = newVertexId;
        
        return newVertexId;
    }
    
    std::vector<Segment*> segments;
    std::vector<Vertex> vertices;
    std::map<Vertex, int> vertexMap;
};

}