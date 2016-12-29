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

class Level;
    
struct Vertex {
    Vec3 v;
    
    Vertex(Vec3 v_) : v(v_) { }
    
    bool operator<(const Vertex& vertex) const {
        return v.toTuple() < vertex.v.toTuple();
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

class LevelSegmentFace {
public:
    LevelSegmentFace(Segment& seg_, int id_) : seg(seg_), id(id_) { }
    
    Polygon3D getGeometry() const;
    LevelSegmentFace& operator=(const LevelSegmentFace& face);
    Segment& extrude(float dist);
    Segment& extrude(Prism3D& newGeometry);
    
    
    Segment& getSeg() const {
        return seg;
    }
    
    int getId() const {
        return id;
    }
    
private:
    Segment& seg;
    int id;
};

class Segment {
public:
    Segment(const Segment& seg) = default;
    Segment(Level& level_, LevelPrism& geometry_, int id_) : level(level_), geometry(geometry_), id(id_), deleted(false) {
        for(int i = 0; i < geometry_.totalFaces(); ++i)
            faces.push_back(new LevelSegmentFace(*this, i));
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
    
    LevelSegmentFace& getFace(int faceId) {
        return *faces[faceId];
    }
    
    ~Segment() {
        for(int i = 0; i < (int)geometry.totalFaces(); ++i)
            delete faces[i];
    }
    
private:
    Level& level;
    std::vector<LevelSegmentFace*> faces;
    LevelPrism geometry;
    int id;
    bool deleted;
};

class Level {
public:
    class SegmentIterator {
    public:
        SegmentIterator(Segment** seg_, Segment** end_) : seg(seg_), end(end_)  {
            if(shouldSkipDeletedSegment(seg))
                *this = next();
        }
        
        SegmentIterator next() const {
            Segment** s = seg + 1;
            
            while(shouldSkipDeletedSegment(s))
                ++s;
            
            return SegmentIterator(s, end);
        }
        
        SegmentIterator& operator=(const SegmentIterator& segIterator) {
            seg = segIterator.seg;
            end = segIterator.end;
            return *this;
        }
        
        bool operator==(const SegmentIterator& segIterator) const {
            return seg == segIterator.seg;
        }
        
        bool operator!=(const SegmentIterator& segIterator) const {
            return !(*this == segIterator);
        }
        
        SegmentIterator& operator++() {
            *this = this->next();
            return *this;
        }
        
        Segment& operator*() const {
            return **seg;
        }
        
        Segment* operator->() const {
            return *seg;
        }
        
    private:
        bool shouldSkipDeletedSegment(Segment** s) const {
            return s < end && (*s)->isDeleted();
        }
        
        Segment** seg;
        Segment** end;
    };
    
    SegmentIterator segmentBegin() {
        return SegmentIterator(&segments[0], &segments[segments.size()]);
    }
    
    SegmentIterator segmentEnd() {
        return SegmentIterator(&segments[segments.size()], &segments[segments.size()]);
    }
    
    
    Segment& addSegment(Prism3D& geometry) {
        LevelPrism prism = vertexManager.addLevelPrism(geometry);
        
        Segment* newSegment = new Segment(*this, prism, segments.size());
        segments.push_back(newSegment);
        
        return *newSegment;
    }
    
    Segment& getSegment(int id) const {
        return *segments[id];
    }
    
    ~Level() {
        for(int i = 0; i < (int)segments.size(); ++i)
            delete segments[i];
    }
    
private:
    VertexManager vertexManager;
    std::vector<Segment*> segments;
};


