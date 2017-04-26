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

#include "IndexedFile.hpp"

using namespace std;

// Creates a new entry in the indexed file. Returns the index ID of the entry.
int IndexedFileWriter::beginEntry() {
    assert(!insideEntryBlock);
    
    insideEntryBlock = true;
    entries.push_back(vector<unsigned char>());
    return currentEntryIndex();
}

// Ends the current entry in the indexed file. Returns the size of the entry.
int IndexedFileWriter::endEntry() {
    assert(insideEntryBlock);
    
    insideEntryBlock = false;
    return entries[currentEntryIndex()].size();
}

void IndexedFileWriter::writeInt(int val) {
    auto currentBlock = getCurrentEntry();
    
    for(int i = 0; i < 4; ++i) {
        currentBlock.push_back(val & 0xFF);
        val >>= 8;
    }
}

void IndexedFileWriter::writeShort(short val) {
    auto currentBlock = getCurrentEntry();
    
    for(int i = 0; i < 2; ++i) {
        currentBlock.push_back(val & 0xFF);
        val >>= 8;
    }
}

void IndexedFileWriter::writeBlock(unsigned char* blockData, size_t size) {
    auto currentBlock = getCurrentEntry();
    
    for(size_t i = 0; i < size; ++i)
        currentBlock.push_back(blockData[i]);
}

void IndexedFileWriter::saveToFile(string fileName) {
    FILE* file = fopen(fileName.c_str(), "wb");
    
    if(!file)
        throw "Failed to open " + fileName + " for writing";
    
    saveHeaderToFile(file);
    saveEntriesToFile(file);
    
    fclose(file);
}

void IndexedFileWriter::saveHeaderToFile(FILE* file) {
    writeIntToFile(file, totalEntries());
    
    int sum = 0;
    for(int i = 0; i < totalEntries(); ++i) {
        writeIntToFile(file, sum);
        sum += entries[i].size();
    }
}

void IndexedFileWriter::saveEntriesToFile(FILE* file) {
    for(int i = 0; i < totalEntries(); ++i)
        fwrite(&entries[i][0], entries[i].size(), 1, file);
}

