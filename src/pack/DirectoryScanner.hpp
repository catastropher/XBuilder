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
#include <string>
#include <boost/filesystem.hpp>

class DirectoryScanner {
private:
    boost::filesystem::path directoryToScan;
    bool useRelativePaths;
    
public:
    
    DirectoryScanner(boost::filesystem::path dirToScan, bool useRelativePaths_) : directoryToScan(dirToScan), useRelativePaths(useRelativePaths_) { }
    
    std::vector<std::string> recursivelyScanFiles();
    
private:
    void addFilesFromDirectory(boost::filesystem::path directory, std::vector<std::string>& foundFiles);
    std::string getFilePath(boost::filesystem::path p);
};

