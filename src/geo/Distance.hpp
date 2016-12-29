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

struct Distance {
    enum Unit {
        FEET,
        METERS,
        X3D_UNITS
    };
    
    float dist;
    Unit unit;
    
    Distance(float dist_, Unit unit_) : dist(dist_), unit(unit_) { }
    
    Distance toUnit(Unit newUnit) const {
        if(newUnit == unit)
            return *this;
        
        float x3dUnits = convertToX3DUnits();
        float newDist = convertX3DUnitsToUnit(x3dUnits, newUnit);
        
        return Distance(newDist, newUnit);
    }
    
private:
    float convertToX3DUnits() const {
        const float METERS_PER_FOOT = 3.28084;
        
        if(unit == FEET)
            return dist * X3D_UNITS_PER_FOOT;
        
        if(unit == METERS)
            return dist * X3D_UNITS_PER_FOOT * METERS_PER_FOOT;
        
        return dist;
    }
    
    float convertX3DUnitsToUnit(float x3dUnits, Unit newUnit) const {
        const float METERS_PER_FOOT = 3.28084;
        
        if(newUnit == FEET)
            return x3dUnits / X3D_UNITS_PER_FOOT;
        
        if(newUnit == METERS)
            return x3dUnits / X3D_UNITS_PER_FOOT / METERS_PER_FOOT;
        
        return x3dUnits;
    }
};

