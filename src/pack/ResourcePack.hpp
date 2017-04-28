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

#include <string>
#include <X3D/X3D.h>

struct ResourcePackDataSoure {
    std::string packFilePath;
    
    ResourcePackDataSoure(std::string packFilePath_) : packFilePath(packFilePath_) { }
    
    virtual std::vector<uint8> getSourceContents() = 0;
    virtual ~ResourcePackDataSoure() { }
};

struct ResourcePackPhysicalFile : ResourcePackDataSoure {
    std::string rootDirectory;
    std::string physicalRelativePath;
    
    ResourcePackPhysicalFile(std::string rootDirectory_, std::string physicalRelativePath_, std::string packFilePath_)
        : ResourcePackDataSoure(packFilePath_),
        rootDirectory(rootDirectory_),
        physicalRelativePath(physicalRelativePath_)
        { }
    
    std::vector<uint8> getSourceContents();
};

class ResourcePackBuilder {
public:
    void addFilesFromDirectoryToPackDirectory(std::string directory, std::string packDirectory);
    void writePackFile(std::string packFileName);
    
    ~ResourcePackBuilder();
    
private:
    void addSource(ResourcePackDataSoure* source) {
        sources.push_back(source);
    }
    
    X3D_ResourcePackFile buildPackFileFromDataSource(ResourcePackDataSoure* source);
    
    std::vector<ResourcePackDataSoure*> sources;
};

