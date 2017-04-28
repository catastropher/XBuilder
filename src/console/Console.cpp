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

#include "Console.hpp"
#include "commands.hpp"

using namespace ConsoleCommands;
using namespace std;

Console::Console(XBuilderContext& context_) : context(context_), hasNewText(false) {
    addCommand(
        "clear",
        "Clears the console",
        commandClear
    );
    
    addCommand(
        "echo",
        "Echos text to the console",
        commandEcho
    );
    
    addCommand(
        "import-tex",
        "Imports bitmaps as textures from a directory",
        commandImportTex
    );
    
    addCommand(
        "save",
        "Saves the project to a file",
        commandSave
    );
}

void Console::executeCommand(string command) {
    try {
        vector<string> args = parseCommandString(command);
        
        if(args.size() == 0)
            return;
        
        string commandName = args[0];
        args.erase(args.begin());
        
        if(commands.count(commandName) == 0)
            throw "Unknown command '" + commandName + "'";
        
        ConsoleCommandContext commandContext(context, *this);
        commands[commandName].handler(commandContext, args);
        
    }
    catch(std::string err) {
        print("Error: %s\n", err.c_str());
    }
    catch(const char* err) {
        print("Error: %s\n", err);
    }
}


vector<string> Console::parseCommandString(string& commandString) {
    vector<string> tokens;
    string token;
    
    size_t pos = 0;
    
    while(pos < commandString.length()) {
        if(commandString[pos] == ' ') {
            if(token != "")
                tokens.push_back(token);
            
            token = "";
            ++pos;
        }
        else if(commandString[pos] == '"') {
            while(++pos < commandString.size() && commandString[pos] != '"')
                token += commandString[pos];
            
            bool wasMatched = pos < commandString.size();
            if(!wasMatched)
                throw "Umatched '\"'";
            
            ++pos;
        }
        else {
            token += commandString[pos++];
        }
    }
    
    if(token != "")
        tokens.push_back(token);
    
    return tokens;
}



