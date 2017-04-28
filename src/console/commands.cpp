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

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include "Console.hpp"
#include "pack/DirectoryScanner.hpp"
#include "XBuilderContext.hpp"
#include "project/ProjectSaver.hpp"
#include "project/ProjectLoader.hpp"
#include "pack/ResourcePack.hpp"

using namespace std;

namespace ConsoleCommands {

void commandClear(ConsoleCommandContext& context, std::vector<std::string>& args) {
    context.console.clear();
}

void commandEcho(ConsoleCommandContext& context, std::vector<std::string>& args) {
    for(auto arg : args)
        context.console.print(arg + "\n");
    
    context.console.print("\n");
}

void commandImportTex(ConsoleCommandContext& context, std::vector<std::string>& args) {
    if(args.size() == 0)
        throw "Expected directory to scan for bitmaps";
    
    context.console.printLine("Importing textures from " + args[0]);
    context.context.getProject().getTextureManager().recursivelyImportTexturesFromDirectory(args[0]);
    context.console.printLine("Done.");
}

void commandSave(ConsoleCommandContext& context, std::vector<std::string>& args) {
    if(args.size() == 0)
        throw "Expected file name to save project to";
    
    ProjectSaver saver(context.context.getProject());
    saver.saveToFile(args[0]);
    
    context.console.printLine("Saved to " + args[0]);
}

void commandLoad(ConsoleCommandContext& context, std::vector<std::string>& args) {
    if(args.size() == 0)
        throw "Expected file name to load project from";
    
    ProjectLoader loader(args[0]);
    Project* newProject = loader.loadFromFile();
    
    context.context.addEvent(new XBuilderContextChangeProjectEvent(newProject));
}

void commandExtractPack(ConsoleCommandContext& context, std::vector<std::string>& args) {
    if(args.size() != 2)
        throw "Wrong numbers of args";
    
    try {
        ResourcePackExtractor packExtractor(args[0]);
        packExtractor.extractPackFilesToDirectory(args[1]);
        
        context.console.printLine("Extracted pack file to " + args[1]);
    }
    catch(boost::filesystem::filesystem_error err) {
        throw "Could not extrack packfile " + args[0] + ": " + err.what();
    }
}
    
};
