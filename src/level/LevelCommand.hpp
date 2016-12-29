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

#include "Level.hpp"

struct LevelCommand {
    Level& level;
    
    LevelCommand(Level& level_) : level(level_) { }
    
    virtual void execute() = 0;
    virtual ~LevelCommand() { }
};

struct UndoableLevelCommand : LevelCommand {
    virtual void undo() = 0;
    virtual void redo() { execute(); }
    virtual ~UndoableLevelCommand();
    
    UndoableLevelCommand(Level& level_) : LevelCommand(level_) { }
};

struct CommandStack {
    CommandStack() : stackPos(0) { }
    
    void addCommand(UndoableLevelCommand* command) {
        if(stackPos < (int)commandStack.size()) {
            delete commandStack[stackPos];
            commandStack[stackPos++] = command;
        }
        else {
            commandStack.push_back(command);
            ++stackPos;
        }
    }
    
    bool canUndo() const {
        return stackPos > 0;
    }
    
    bool canRedo() const {
        return stackPos < (int)commandStack.size();
    }
    
    void undo() {
        if(canUndo())
            commandStack[--stackPos]->undo();
    }
    
    void redo() {
        if(canRedo())
            commandStack[stackPos++]->redo();
    }
    
    int stackPos;
    std::vector<UndoableLevelCommand*> commandStack;
};

struct LevelCommandManager {
    void executeCommand(LevelCommand* command) {
        command->execute();
        
        if(UndoableLevelCommand* cmd = dynamic_cast<UndoableLevelCommand*>(command)) {
            stack.addCommand(cmd);
        }
        else {
            delete command;
        }
    }
    
    void undo() {
        return stack.undo();
    }
    
    void redo() {
        return stack.redo();
    }
    
    CommandStack stack;
};

struct ModifySegmentCommand : UndoableLevelCommand {
    Segment& seg;
    Segment segCopy;
    Prism3D geometry;
    
    ModifySegmentCommand(Segment& seg_) : UndoableLevelCommand(seg_.getLevel()), seg(seg_), segCopy(seg_), geometry(seg.getGeometry()) { }
    
    virtual ~ModifySegmentCommand();
    
    void undo() {
        seg = segCopy;
        seg.updateGeometry(geometry);
    }
};

struct AddSegmentCommand : ModifySegmentCommand {
    AddSegmentCommand(Segment& seg_) : ModifySegmentCommand(seg_) { }
    
    void execute() {
        seg.deleteSelf();
    }
};

