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

#include "X3D.h"
#include "x3dwrapper.hpp"

struct LevelVertex : XVex3D {
    LevelVertex(XVex3D& v) : XVex3D(v) { }
};

struct LevelPrism {
    int base_v;
    std::vector<LevelVertex*> v;
    
    XPrism3D toXPrism3D() {
        XPrism3D prism(base_v);
        
        for(int i = 0; i < base_v * 2; ++i)
            prism.v[i] = *v[i];
        
        return prism;
    }
};

struct LevelSegment {
    LevelPrism geometry;
    std::vector<LevelSegment*> connections;
    
    LevelSegment(LevelPrism geometry_) {
        geometry = geometry_;
        
        for(int i = 0; i < geometry.base_v + 2; ++i) {
            connections.push_back(nullptr);
        }
    }
    
    XPrism3D getGeometry() {
        return geometry.toXPrism3D();
    }
};

class LevelBuilder {
private:
    std::vector<LevelVertex*> vertices;
    std::vector<LevelSegment*> segments;
    
    LevelPrism addGeometry(XPrism3D& prism) {
        LevelPrism level_prism;
        level_prism.base_v = prism.base_v;
        
        for(int i = 0; i < prism.base_v * 2; ++i)
            level_prism.v.push_back(&addVertex(prism.v[i]));
        
        return level_prism;
    }
    
    LevelSegment& newSegment(XPrism3D geometry) {
        LevelPrism level_geo = addGeometry(geometry);
        LevelSegment* seg = new LevelSegment(level_geo);
        
        segments.push_back(seg);
        
        return *seg;
    }
    
public:
    ~LevelBuilder();
    
    LevelVertex& addVertex(XVex3D vertex);
    LevelSegment& addSegment(X3D_Prism3D geometry);
    LevelSegment& extrudeSegment(LevelSegment& segment, int16 face, int16 extrude_dist);
    
    X3D_Level buildX3DLevel();
};

