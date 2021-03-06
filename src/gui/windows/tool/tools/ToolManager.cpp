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

#include "tools.hpp"

void ToolManager::setSelectedToolGroup(std::string name) {
    if(selectedToolGroup)
        delete selectedToolGroup;
    
    if(name == "face")
        selectedToolGroup = new FaceToolGroup(context);
    else if(name == "segment")
        selectedToolGroup = new SegmentToolGroup(context);
    else
        selectedToolGroup = nullptr;
}

