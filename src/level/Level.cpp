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

#include <cassert>

#include "Level.hpp"

Polygon3D LevelSegmentFace::getGeometry() const {
    return seg.getGeometry().getFace(id);
}

Segment& Segment::operator=(const Segment& seg) {
    assert(&seg.level == &level);
    
    faces = seg.faces;
    geometry = seg.geometry;
    id = seg.id;
    deleted = seg.deleted;
    
    return *this;
}

json LevelSurface::saveX3dSurfaceTextureToJson(X3D_SurfaceTexture* tex) {
    TextureManager& textureManager = level.getTextureManager();
    json textureJson = json::object();
    LevelTexture* levelTex = textureManager.getTextureByTextureAddress(tex->tex);
    
    if(!levelTex)
        throw "Missing texture";
    
    textureJson["name"] = levelTex->getName();
    textureJson["offsetx"] = tex->offset.x;
    textureJson["offsety"] = tex->offset.y;
    textureJson["angle"] = tex->angle;
    textureJson["scale"] = tex->scale;
    textureJson["flags"] = tex->flags;
    
    return textureJson;
}

X3D_SurfaceTexture LevelSurface::loadX3dSurfaceTextureFromJson(json& texJson) {
    X3D_SurfaceTexture tex;
    
    if(texJson == nullptr) {
        tex.tex = nullptr;
        return tex;
    }
    
    std::string texName = texJson["name"];
    LevelTexture* levelTexture = level.getTextureManager().getTextureByName(texName);
    
    if(!levelTexture)
        throw "Missing texture '" + texName + "'";
    
    tex.tex = &levelTexture->getX3DTexture();
    
    tex.offset.x = texJson["offsetx"];
    tex.offset.y = texJson["offsety"];
    tex.angle = texJson["angle"];
    tex.scale = texJson["scale"];
    tex.flags = texJson["flags"];
    
    return tex;
}



LevelSegmentFace::LevelSegmentFace(Segment& seg_, int id_)
    : seg(seg_), id(id_), connectedFace(nullptr), surface(getGeometry(), seg_.getLevel())
{
    
}

LevelSegmentFace& LevelSegmentFace::operator=(const LevelSegmentFace& face) {
    seg = face.seg;
    id = face.id;
    
    return *this;
}

Segment& LevelSegmentFace::extrude(float dist) {
    Prism3D geo = seg.getGeometry();
    Prism3D newSegGeometry = geo.createPrism3DFromExtrudedFace(geo, id, dist);
    
    return extrude(newSegGeometry);
}

Segment& LevelSegmentFace::extrude(Prism3D& newGeometry) {
    Segment& newSeg = seg.getLevel().addSegment(newGeometry);
    connectToSegmentFace(newSeg.getFace(Prism3D::BASE_A));
    return newSeg;
}

json LevelSegmentFace::saveToJsonObject() {
    json faceJson = json::object();
    
    if(!isConnectedToSegmentFace()) {
        faceJson["connectedSegmentId"] = nullptr;
        faceJson["connectedFaceId"] = nullptr;
    }
    else {
        faceJson["connectedSegmentId"] = connectedFace->seg.getId();
        faceJson["connectedFaceId"] = connectedFace->id;
    }
    
    faceJson["surface"] = surface.saveToJsonObject();
    
    return faceJson;
}

void LevelSegmentFace::loadFromJsonObject(json& faceJson) {
    surface.loadFromJsonObject(faceJson["surface"]);
    
    // Note: we can't load the connections here because it's possible we reference
    // a segment that hasn't been loaded yet :(
}



void Segment::updateGeometry(Prism3D& updatedGeometry) {
    geometry.updateGeometry(updatedGeometry);
    
    // TODO: in future we shouldn't rebuild all surfaces
    level.rebuildAllSurfaces();
}

void Segment::loadFromJsonObject(json& segmentJson) {
    assert(faces.size() == 0);
    
    id = segmentJson["id"];
    deleted = segmentJson["isDeleted"];
    
    std::vector<Vertex*> vertices;
    for(auto vertexId : segmentJson["prism"]) {
        vertices.push_back(level.getVertexManager().getVertexById(vertexId));
    }
    
    geometry = LevelPrism(vertices);
    
    int faceId = 0;
    for(auto face : segmentJson["faces"]) {
        LevelSegmentFace* levelFace = new LevelSegmentFace(*this, faceId++);
        levelFace->loadFromJsonObject(face);
        faces.push_back(levelFace);
    }
}



