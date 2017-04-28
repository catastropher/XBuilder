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

#include <boost/filesystem.hpp>

#include "ProjectLoader.hpp"
#include "Project.hpp"
#include "pack/ResourcePack.hpp"

using namespace std;
using namespace boost::filesystem;

Project* ProjectLoader::loadFromFile() {
    Project* newProject = new Project;
    path tempDir = unique_path();
    
    try {
        create_directories(tempDir);
        
        ResourcePackExtractor extractor(fileName);
        extractor.extractPackFilesToDirectory(tempDir.string());
        
        newProject->getTextureManager().recursivelyImportTexturesFromDirectory(tempDir.string() + "/textures");        
        newProject->getLevel().loadLevelFromFile(tempDir.string() + "/level1.json");
        
        remove_all(tempDir);
        
        return newProject;
    }
    catch(filesystem_error err) {
        if(exists(tempDir))
            remove_all(tempDir);
        
        throw "Failed to load project: " + (string)err.what();
    }
    catch(...) {
        if(exists(tempDir))
            remove_all(tempDir);
        
        throw;
    }
}


