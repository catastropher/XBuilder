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

#include <cstdio>

static inline void writeIntToFile(FILE* file, int val) {
    for(int i = 0; i < 4; ++i) {
        fputc(val & 0xFF, file);
        val >>= 8;
    }
}

static inline int readIntFromFile(FILE* file) {
    int val = 0;
    
    for(int i = 0; i < 4; ++i)
        val = (val << 8) | fgetc(file) << 24;
    
    return val;
}

