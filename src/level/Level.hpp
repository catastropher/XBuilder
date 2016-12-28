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
    
    struct LevelPrism {
    public:
        LevelPrism(std::vector<Vertex*> vertices_) : vertices(vertices_) { }
        
        int baseVertices() const {
            return vertices.size() / 2;
        }
        
        int totalFaces() const {
            return baseVertices() + 2;
        }
        
        Prism3D getGeometry() const {
            std::vector<Vec3> v;

            for(int i = 0; i < (int)vertices.size(); ++i)
                v.push_back(vertices[i]->v);
            
            return Prism3D(v);
        }
        
        void updateGeometry(Prism3D& updatedGeometry) {
            for(int i = 0; i < (int)vertices.size(); ++i)
                vertices[i]->v = updatedGeometry.getVertex(i);
        }
        
    private:
        std::vector<Vertex*> vertices;
    };
    
    class VertexManager {
    public:
        Vertex* addVertex(Vec3 v) {
            Vertex vertex(v);
            
            if(vertices.count(vertex) != 0)
                return vertices[vertex];
            
            Vertex* newVertex = new Vertex(v);
            vertices[vertex] = newVertex;
            
            return newVertex;
        }
        
        LevelPrism addLevelPrism(Prism3D& geometry) {
            std::vector<Vertex*> v;
            
            for(int i = 0; i < geometry.totalVertices(); ++i)
                v.push_back(addVertex(geometry.getVertex(i)));
            
            return LevelPrism(v);
        }
        
        ~VertexManager() {
            for(auto v : vertices) {
                delete v.second;
            }
            
            vertices.clear();
        }
        
    private:
        std::map<Vertex, Vertex*> vertices;
    };

    struct Segment;
    struct Level;
    
    class SegmentFace {
    public:
        SegmentFace(Segment& seg_, int id_) : seg(seg_), id(id_) { }
        
        Polygon3D getGeometry() const;
        SegmentFace& operator=(const SegmentFace& face);
        
    private:
        Segment& seg;
        int id;
    };
    
    class Segment {
    public:
        Segment(const Segment& seg) = default;
        Segment(Level& level_, LevelPrism& geometry_, int id_) : level(level_), geometry(geometry_), id(id_), deleted(false) {
            for(int i = 0; i < geometry_.totalFaces(); ++i)
                faces.push_back(SegmentFace(*this, i));
        }
        
        Prism3D getGeometry() const {
            return geometry.getGeometry();
        }
        
        void updateGeometry(Prism3D& updatedGeometry) {
            geometry.updateGeometry(updatedGeometry);
        }
        
        bool isDeleted() const {
            return deleted;
        }
        
        void deleteSelf() {
            deleted = true;
        }
        
        void undeleteSelf() {
            deleted = false;
        }
        
        Level& getLevel() const {
            return level;
        }
        
        Segment& operator=(const Segment& seg);
        
    private:
        Level& level;
        std::vector<SegmentFace> faces;
        LevelPrism geometry;
        int id;
        bool deleted;
    };
    
    class Level {
    public:
        Segment& addSegment(Prism3D& geometry) {
            LevelPrism prism = vertexManager.addLevelPrism(geometry);
            
            Segment* newSegment = new Segment(*this, prism, segments.size());
            segments.push_back(newSegment);
            
            return *newSegment;
        }
        
        Segment& getSegment(int id) const {
            return *segments[id];
        }
        
        std::vector<Segment*> getAllSegments() const {
            return segments;
        }
        
    private:
        VertexManager vertexManager;
        std::vector<Segment*> segments;
    };
}

