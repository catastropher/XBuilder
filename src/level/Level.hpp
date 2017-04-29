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
#include <cassert>

#include "geo/geo.hpp"
#include "texture.hpp"
#include "gui/ViewRenderer.hpp"
#include "json/json.hpp"
#include "pack/fileutils.hpp"

class Level;

using nlohmann::json;

struct Vertex {
    Vec3 v;
    int id;
    
    Vertex(Vec3 v_, int id_) : v(v_), id(id_) { }
    Vertex(Vec3 v_) : v(v_) { }
    
    bool operator<(const Vertex& vertex) const {
        return v.toIntTuple() < vertex.v.toIntTuple();
    }
};

struct LevelPrism {
public:
    LevelPrism(std::vector<Vertex*> vertices_) : vertices(vertices_) { }
    
    int baseVertices() const { return vertices.size() / 2; }
    int totalFaces() const { return baseVertices() + 2; }
    
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
    
    json saveToJsonObject() {
        json vertexIds = json::array();
        
        for(int i = 0; i < (int)vertices.size(); ++i) {
            vertexIds.push_back(vertices[i]->id);
        }
        
        return vertexIds;
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
        
        int newVertexId = vertices.size();
        Vertex* newVertex = new Vertex(v, newVertexId);
        vertices[vertex] = newVertex;
        verticesById.push_back(newVertex);
        
        return newVertex;
    }
    
    LevelPrism addLevelPrism(Prism3D& geometry) {
        std::vector<Vertex*> v;
        
        for(int i = 0; i < geometry.totalVertices(); ++i)
            v.push_back(addVertex(geometry.getVertex(i)));
        
        return LevelPrism(v);
    }
    
    void addVerticesToLevelJson(json& levelJson) {
        json verticesJson = json::array();
        
        for(Vertex* v : verticesById) {
            verticesJson.push_back(
                {
                    { "x", v->v.x },
                    { "y", v->v.y },
                    { "z", v->v.z }
                }
            );
        }
        
        levelJson["vertices"] = verticesJson;
    }
    
    void loadVerticesFromLevelJson(json& levelJson) {
        assert(vertices.size() == 0);
        
        int vertexId = 0;
        for(auto vertex : levelJson["vertices"]) {
            Vec3 v = Vec3(vertex["x"], vertex["y"], vertex["z"]);
            
            if(vertices.count(Vertex(v)) != 0)
                throw "Vertex #" + std::to_string(vertexId) + " is a duplicate of vertex #" + std::to_string(vertices[Vertex(v)]->id);
            
            addVertex(v);
            ++vertexId;
        }
    }
    
    Vertex* getVertexById(int id) {
        assert(id < (int)verticesById.size());
        
        return verticesById[id];
    }
    
    ~VertexManager() {
        for(auto v : vertices) {
            delete v.second;
        }
        
        vertices.clear();
    }
    
private:
    std::map<Vertex, Vertex*> vertices;
    std::vector<Vertex*> verticesById;
};

struct Segment;
struct Level;

class LevelSurface {
public:
    LevelSurface(Polygon3D geometry, Level& level_) : level(level_), hasPrimaryTexture_(false) {
        surface.surface.texels = nullptr;
        
        if(geometry.totalVertices() == 0) {
            return;
        }
        
        updateSurfaceGeometry(geometry);
    }
    
    void updateSurfaceGeometry(Polygon3D geometry) {
        X3D_Vex3D v[X3D_MAX_POINTS_IN_POLY];
        X3D_Polygon3D poly;
        poly.v = v;
        
        geometry.toX3DPolygon3D(poly);
        
        if(surfaceHasAlreadyBeenCreated())
            x3d_surface_update_geometry(&surface, &poly);
        else
            x3d_surface_init(&surface, &poly);
        
        rebuildSurface();
    }
    
    bool hasPrimaryTexture() const {
        return hasPrimaryTexture_;
    }
    
    X3D_SurfaceTexture& getPrimaryTexture() {
        return primaryTexture;
    }
    
    int totalDecals() const {
        return decals.size();
    }
    
    X3D_SurfaceTexture& getDecal(int id) {
        return *decals[id];
    }
    
    X3D_SurfaceTexture& addNewDecalTexture(X3D_SurfaceTexture tex) {
        X3D_SurfaceTexture* decal = new X3D_SurfaceTexture(tex);
        decals.push_back(decal);
        
        return *decal;
    }
    
    void setPrimaryTexture(X3D_SurfaceTexture tex) {
        primaryTexture = tex;
        hasPrimaryTexture_ = true;
    }
    
    X3D_Surface& getSurface() {
        return surface;
    }
    
    void rebuildSurface() {
        rebuildSurfaceTextures();
        x3d_surface_force_entire_rebuild(&surface);
    }
    
    X3D_Vex2D projectPointInSurfaceCoordinateSystem(Vec3 v) const {
        X3D_Vex3D x3dV = v.toX3DVex3D();
        return x3d_orientation3d_transform_point(&surface.orientation, x3dV);
    }
    
    bool validPointInSurfaceCoordinateSystem(X3D_Vex2D v) const {
        return v.x >= 0 && v.x < x3d_surface_w(&surface) && v.y >= 0 && v.y < x3d_surface_h(&surface);
    }
    
    json saveToJsonObject() {
        json surfaceJson = json::object();
        
        if(!hasPrimaryTexture()) {
            surfaceJson["primaryTexture"] = nullptr;
        }
        else {
            surfaceJson["primaryTexture"] = saveX3dSurfaceTextureToJson(&primaryTexture);
        }
        
        json decalTextures = json::array();
        
        for(int i = 0; i < (int)decals.size(); ++i)
            decalTextures.push_back(saveX3dSurfaceTextureToJson(decals[i]));
        
        surfaceJson["decalTextures"] = decalTextures;
        
        return surfaceJson;
    }
    
    void loadFromJsonObject(json& surfaceJson) {
        assert(!hasPrimaryTexture_ && totalDecals() == 0);
        
        hasPrimaryTexture_ = surfaceJson["primaryTexture"] != nullptr;
        
        if(hasPrimaryTexture()) {
            primaryTexture = loadX3dSurfaceTextureFromJson(surfaceJson["primaryTexture"]);
        }
        
        for(auto decalTexture : surfaceJson["decalTextures"]) {
            addNewDecalTexture(loadX3dSurfaceTextureFromJson(decalTexture));
        }
    }
    
    ~LevelSurface() {
        if(surface.surface.texels)
            x3d_surface_cleanup(&surface);
    }
    
private:
    void rebuildSurfaceTextures() {
        surfaceTextures.clear();
        
        if(hasPrimaryTexture_) {
            surfaceTextures.push_back(primaryTexture);
        }
        
        for(int i = 0; i < (int)decals.size(); ++i) {
            if(decals[i]) {
                surfaceTextures.push_back(*decals[i]);
            }
        }
        
        surface.total_textures = surfaceTextures.size();
        surface.textures = &surfaceTextures[0];
    }
    
    bool surfaceHasAlreadyBeenCreated() const {
        return surface.surface.texels != nullptr;
    }
    
    json saveX3dSurfaceTextureToJson(X3D_SurfaceTexture* tex);
    X3D_SurfaceTexture loadX3dSurfaceTextureFromJson(json& texJson);
    
    Level& level;
    X3D_Surface surface;
    X3D_SurfaceTexture primaryTexture;
    bool hasPrimaryTexture_;
    std::vector<X3D_SurfaceTexture*> decals;
    std::vector<X3D_SurfaceTexture> surfaceTextures;
};

class LevelSegmentFace {
public:
    LevelSegmentFace(Segment& seg_, int id_);
    
    Polygon3D getGeometry() const;
    LevelSegmentFace& operator=(const LevelSegmentFace& face);
    Segment& extrude(float dist);
    Segment& extrude(Prism3D& newGeometry);
    LevelSurface& getSurface() { return surface; }
    Segment& getSeg() const { return seg; }
    
    int getId() const { return id; }
    
    bool isConnectedToSegmentFace() const { return connectedFace != nullptr; }
    
    void connectToSegmentFace(LevelSegmentFace& face) {
        connectedFace = &face;
        face.connectedFace = this;
    }
    
    void renderWithSurface() {
        ViewRenderer::renderPolygonSurface(&surface.getSurface(), getGeometry());
    }
    
    void rebuildSurface() {
        surface.updateSurfaceGeometry(getGeometry());
    }
    
    json saveToJsonObject();
    void loadFromJsonObject(json& faceJson);
    
private:
    Segment& seg;
    int id;
    LevelSegmentFace* connectedFace;
    LevelSurface surface;
};

class Segment {
public:
    Segment(const Segment& seg) = default;
    Segment(Level& level_, LevelPrism& geometry_, int id_)
        : level(level_), geometry(geometry_), id(id_), deleted(false)
        {
            if(geometry.baseVertices() == 0)
                return;
            
            for(int i = 0; i < geometry_.totalFaces(); ++i)
                faces.push_back(new LevelSegmentFace(*this, i));
        }
    
    Prism3D getGeometry() const {
        return geometry.getGeometry();
    }
    
    void updateGeometry(Prism3D& updatedGeometry);
    
    bool isDeleted() const { return deleted; }
    int totalFaces() const { return geometry.totalFaces(); }
    
    LevelSegmentFace& getFace(int faceId) {
        if(faceId >= totalFaces()) {
            printf("Requested face %d, has %d faces (id %d)\n", faceId, totalFaces(), id);
        }
        
        assert(faceId < totalFaces());
        return *faces[faceId];
        
    }
    Level& getLevel() const { return level; }
    
    void deleteSelf() { deleted = true; }
    void undeleteSelf() { deleted = false; }
    
    Segment& operator=(const Segment& seg);
    
    void rebuildSurfaces() {
        for(int i = 0; i < totalFaces(); ++i) {
            faces[i]->rebuildSurface();
        }        
    }
    
    json saveToJsonObject() {
        json segmentJson = json::object();
        
        segmentJson["id"] = id;
        segmentJson["isDeleted"] = isDeleted();
        segmentJson["prism"] = geometry.saveToJsonObject();
        
        json facesJson = json::array();
        
        for(int i = 0; i < totalFaces(); ++i) {
            facesJson.push_back(faces[i]->saveToJsonObject());
        }
        
        segmentJson["faces"] = facesJson;
        
        return segmentJson;
    }
    
    void loadFromJsonObject(json& segmentJson);
    
    int getId() const { return id; }
    
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
        
        bool operator==(const SegmentIterator& segIterator) const { return seg == segIterator.seg; }
        bool operator!=(const SegmentIterator& segIterator) const { return !(*this == segIterator); }
        
        SegmentIterator& operator++() {
            *this = this->next();
            return *this;
        }
        
        Segment& operator*() const { return **seg; }
        Segment* operator->() const { return *seg; }
        
    private:
        bool shouldSkipDeletedSegment(Segment** s) const { return s < end && (*s)->isDeleted(); }
        
        Segment** seg;
        Segment** end;
    };
    
    Level(TextureManager& textureManager_) : textureManager(textureManager_) { }
    
    Segment& getSegmentById(int id) const {
        assert(id < (int)segments.size());
        
        return *segments[id];
    }
    SegmentIterator segmentBegin() {return SegmentIterator(&segments[0], &segments[segments.size()]); }
    SegmentIterator segmentEnd() { return SegmentIterator(&segments[segments.size()], &segments[segments.size()]); }
    
    
    Segment& addSegment(Prism3D& geometry) {
        LevelPrism prism = vertexManager.addLevelPrism(geometry);
        
        Segment* newSegment = new Segment(*this, prism, segments.size());
        segments.push_back(newSegment);
        
        return *newSegment;
    }
    
    void rebuildAllSurfaces() {
        for(SegmentIterator i = segmentBegin(); i != segmentEnd(); ++i) {
            i->rebuildSurfaces();
        }
    }
    
    void saveLevelToJsonFile(std::string fileName) {
        json levelJson = json::object();
        
        vertexManager.addVerticesToLevelJson(levelJson);
        
        json segmentsJson = json::array();
        
        for(SegmentIterator i = segmentBegin(); i != segmentEnd(); ++i) {
            segmentsJson.push_back(i->saveToJsonObject());
        }
        
        levelJson["segments"] = segmentsJson;
        
        std::string levelJsonString = levelJson.dump(4);
        saveStringAsFile(levelJsonString, fileName);
    }
    
    void loadLevelFromFile(std::string fileName) {
        assert(segments.size() == 0);
        
        json levelJson = json::parse(loadFileIntoString(fileName));
        
        vertexManager.loadVerticesFromLevelJson(levelJson);
        
        Prism3D emptyPrism;
        
        for(auto segment : levelJson["segments"]) {
            Segment& seg = addSegment(emptyPrism);
            seg.loadFromJsonObject(segment);
        }
        
        int segId = 0;
        for(auto segment : levelJson["segments"]) {
            printf("Seg %d\n", segId);
            int faceId = 0;
            for(auto face : segment["faces"]) {
                printf("\tProcess face %d\n", faceId);
                
                if(face["connectedSegmentId"] != nullptr && face["connectedFaceId"] != nullptr) {
                
                    int connectedFace = face["connectedFaceId"];
                    int connectedSegment = face["connectedSegmentId"];
                    
                    LevelSegmentFace& us = getFaceBySegmentAndFaceId(segId, faceId);
                    LevelSegmentFace& them = getFaceBySegmentAndFaceId(connectedSegment, connectedFace);

                    us.connectToSegmentFace(them);
                }
                
                ++faceId;
            }
            
            ++segId;
        }
        
        rebuildAllSurfaces();
    }
    
    TextureManager& getTextureManager() {
        return textureManager;
    }
    
    VertexManager& getVertexManager() {
        return vertexManager;
    }
    
    LevelSegmentFace& getFaceBySegmentAndFaceId(int segmentId, int faceId) const {
        return getSegmentById(segmentId).getFace(faceId);
    }
    
    ~Level() {
        for(int i = 0; i < (int)segments.size(); ++i)
            delete segments[i];
    }
    
private:
    VertexManager vertexManager;
    std::vector<Segment*> segments;
    TextureManager& textureManager;
};

