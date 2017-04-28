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

#include "../Window.hpp"
#include "tools/tools.hpp"

class ToolWindow : public Window {
public:
    ToolWindow(WindowContext& context_) : Window(context_, "Tools", true), toolManager(context_) { }
    
    void render() {
        toolManager.renderToolWindow();
    }
    
    void handleMouse(MouseState mouseState) {
        toolManager.viewWindowHandleMouse(mouseState);
    }
    
private:
    ToolManager toolManager;
};

