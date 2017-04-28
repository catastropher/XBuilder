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

#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <cstdio>
#include <algorithm>

#include "ResourcePack.hpp"
#include "DirectoryScanner.hpp"

using namespace std;
using namespace boost::filesystem;

vector<uint8> ResourcePackPhysicalFile::getSourceContents() {
    string fullFilePath = rootDirectory + physicalRelativePath;
    size_t fileSize = file_size(fullFilePath);
    vector<uint8> fileContents(fileSize);
    
    FILE* file = fopen(fullFilePath.c_str(), "rb");
    fread(&fileContents[0], 1, fileSize, file);
    fclose(file);
    
    return fileContents;
}

void ResourcePackBuilder::addFilesFromDirectoryToPackDirectory(string directory, string packDirectory) {
    DirectoryScanner scanner(directory, true);
    vector<string> fileNames = scanner.recursivelyScanFiles();
    
    if(directory[directory.length() - 1] != '/')
        directory += '/';
    
    for(string& fileName : fileNames) {
        addSource(new ResourcePackPhysicalFile(directory, fileName, packDirectory + "/" + fileName));
    }
}

X3D_ResourcePackFile ResourcePackBuilder::buildPackFileFromDataSource(ResourcePackDataSoure* source) {
    X3D_ResourcePackFile packFile;
    vector<uint8> data = source->getSourceContents();
    
    uint8* packFileData = new uint8[data.size()];
    copy(data.begin(), data.end(), packFileData);
    packFile.loaded_data = packFileData;
    packFile.size = data.size();
    
    x3d_strncpy(packFile.name, source->packFilePath.c_str(), 56);
    
    return packFile;
}

void ResourcePackBuilder::writePackFile(string packFileName) {
    X3D_ResourcePackFile* packFiles = new X3D_ResourcePackFile[sources.size()];
    
    for(int i = 0; i < (int)sources.size(); ++i) {
        packFiles[i] = buildPackFileFromDataSource(sources[i]);
    }
    
    x3d_resourcepack_save_packfiles_to_file(packFiles, sources.size(), packFileName.c_str());
    
    for(int i = 0; i < (int)sources.size(); ++i) {
        delete [] (uint8*)packFiles[i].loaded_data;
    }
    
    delete [] packFiles;
}

ResourcePackBuilder::~ResourcePackBuilder() {
    for(ResourcePackDataSoure * source : sources)
        delete source;
}


void ResourcePackExtractor::extractPackFilesToDirectory(string directoryName) {
    if(directoryName[directoryName.length() - 1] != '/')
        directoryName += '/';
    
    if(!x3d_resourcepack_load_from_file(&pack, packFileName.c_str()))
        throw "Failed to open resource pack " + packFileName;
    
    for(int i = 0; i < (int)pack.total_files; ++i)
        extractPackFile(pack.pack_files[i].name, directoryName);
    
    x3d_resourcepack_cleanup(&pack);
}

void ResourcePackExtractor::extractPackFile(const char* packFileName, string destDirectory) {
    X3D_Buffer packFileBuffer;
    if(!x3d_resourcepack_open_packfile(&pack, packFileName, &packFileBuffer))
        throw "Failed to open packfile " + string(packFileName);
    
    string packFileDest = destDirectory + packFileName;
    path outputDirectory = path(packFileDest).parent_path();
    
    if(!exists(outputDirectory))
        create_directories(outputDirectory);
    
    if(!x3d_buffer_save_to_file(&packFileBuffer, packFileDest.c_str()))
        throw "Failed to save extracted packfile " + packFileDest;
    
    x3d_resourcepack_close_packfile(&pack, &packFileBuffer);
}

