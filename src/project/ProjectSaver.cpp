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
#include <iostream>

#include "ProjectSaver.hpp"
#include "pack/ResourcePack.hpp"

using namespace std;
using namespace boost::filesystem;

void ProjectSaver::saveToFile(string fileName) {
    path tempDir;
    path projectDir;
    
    try {
        tempDir = unique_path();
        projectDir = tempDir.string() + "/project";
        
        create_directories(projectDir);
        
        path texturesDir = projectDir.string() + "/textures";
        create_directory(texturesDir);
        
        project.getTextureManager().exportAllTexturesToDirectory(texturesDir.string());
        project.getLevel().saveLevelToJsonFile(projectDir.string() + "/level1.json");
        
        ResourcePackBuilder packBuilder;
        packBuilder.addFilesFromDirectoryToPackDirectory(projectDir.string(), "");
        packBuilder.writePackFile(fileName + ".xproj");
        
        remove_all(tempDir);
        
    }
    catch(filesystem_error err) {
        if(exists(tempDir))
            remove_all(tempDir);
        
        throw "Failed to save project: " + (string)err.what();
    }
}


