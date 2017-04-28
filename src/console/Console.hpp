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
#include <cstdarg>
#include <functional>
#include <vector>
#include <map>

#pragma once

class XBuilderContext;
class Console;

struct ConsoleCommandContext {
    ConsoleCommandContext(XBuilderContext& context_, Console& console_)
        : context(context_), console(console_)
        { }
    
    XBuilderContext& context;
    Console& console;
};

struct ConsoleCommand {
    ConsoleCommand(std::string name_, std::string description_, std::function<void(ConsoleCommandContext&, std::vector<std::string>&)> handler_)
        : name(name_),
        description(description_),
        handler(handler_)
        { }
        
    ConsoleCommand() { }
    
    std::string name;
    std::string description;
    std::function<void(ConsoleCommandContext&, std::vector<std::string>&)> handler;
};

class Console {
public:
    Console(XBuilderContext& context_);
    
    void print(std::string str) {
        if(!enableLogging)
            return;
        
        output += str;
        hasNewText = true;
    }
    
    void printLine(std::string str) {
        if(!enableLogging)
            return;
        
        output += str;
        output += '\n';
        hasNewText = true;
    }
    
    void print(const char* format, ...) {
        if(!enableLogging)
            return;
        
        va_list list;
        va_start(list, format);
        
        char buf[8192];
        vsprintf(buf, format, list);
        
        output.append(buf);
        
        hasNewText = true;
    }
    
    void clear() {
        output.clear();
        hasNewText = true;
    }
    
    bool hasNewTextToDisplay() {
        bool newText = hasNewText;
        hasNewText = false;
        return newText;
    }
    
    std::string& getOutput() { return output; }
    
    void executeCommand(std::string command);
    
    void executeCommandSilently(std::string command) {
        bool oldEnableLogging = enableLogging;
        enableLogging = false;
        
        executeCommand(command);
        
        enableLogging = oldEnableLogging;
    }
    
private:
    void addCommand(std::string name, std::string description, std::function<void(ConsoleCommandContext&, std::vector<std::string>&)> handler) {
        commands[name] = ConsoleCommand(name, description, handler);
    }
    
    std::vector<std::string> parseCommandString(std::string& commandString);
    
    XBuilderContext& context;
    std::string output;
    std::map<std::string, ConsoleCommand> commands;
    bool hasNewText;
    bool enableLogging;
};

