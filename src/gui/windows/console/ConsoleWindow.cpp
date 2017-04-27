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

#include "ConsoleWindow.hpp"
#include "XBuilderContext.hpp"

ConsoleWindow::ConsoleWindow(WindowContext& context_) : Window(context_), inputBuf("") {
    context.context.getConsole().printLine("Welcome to XBuilder!");
}

void ConsoleWindow::render() {
    ImGui::Begin("Console");
    ImGui::BeginChild("ScrollingRegion", ImVec2(0,-ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
    
    Console& console = context.context.getConsole();
    std::string consoleText = console.getOutput();
    
    ImGui::TextUnformatted(&consoleText[0], &consoleText[consoleText.length() - 1]);
    
    ImGui::EndChild();
    
    ImGui::Separator();
    
    if(ImGui::InputText("Input", inputBuf, sizeof(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        console.printLine(inputBuf);
        console.executeCommand(inputBuf);
        inputBuf[0] = '\0';
    }
    
    if(ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
    
    ImGui::End();
}

