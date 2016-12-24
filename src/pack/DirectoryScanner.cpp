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
#include <vector>
#include <string>

#include "DirectoryScanner.hpp"

#include <iostream>

using namespace std;
using namespace boost::filesystem;

vector<string> DirectoryScanner::recursivelyScanFiles() {
    vector<string> foundFiles;
    addFilesFromDirectory(directoryToScan, foundFiles);
    return foundFiles;
}

string DirectoryScanner::getFilePath(path p) {
    if(useRelativePaths)
        return relative(p, directoryToScan).string();
    
    return p.string();
}


void DirectoryScanner::addFilesFromDirectory(path directory, vector<string>& foundFiles) {
    for(auto&& item : directory_iterator(directory)) {
        if(is_regular_file(item)) {
            foundFiles.push_back(getFilePath(item));
        }
        else if(is_directory(item)) {
            addFilesFromDirectory(item, foundFiles);
        }
    }
}

