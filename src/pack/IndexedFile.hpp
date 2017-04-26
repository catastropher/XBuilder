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
#include <vector>
#include <cassert>
#include <cstdio>

class IndexedFileWriter {
public:
    IndexedFileWriter() : insideEntryBlock(false) { }
    
    int beginEntry();
    int endEntry();
    void writeInt(int val);
    void writeShort(short val);
    void writeBlock(unsigned char* blockData, size_t size);
    
    void saveToFile(std::string fileName);
    
private:
    int currentEntryIndex() const {
        assert(entries.size() >= 1);
        return entries.size() - 1;
    }
    
    std::vector<unsigned char>& getCurrentEntry() {
        assert(insideEntryBlock);
        return entries[currentEntryIndex()];
    }
    
    void writeIntToFile(FILE* file, int val) {
        for(int i = 0; i < 4; ++i) {
            fputc(val, file);
            val >>= 8;
        }
    }
    
    int totalEntries() {
        assert(!insideEntryBlock);
        
        return entries.size();
    }
    
    void saveHeaderToFile(FILE* file);
    void saveEntriesToFile(FILE* file);
    
    std::vector<std::vector<unsigned char>> entries;
    bool insideEntryBlock;
};


