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

#include <cassert>
#include <queue>

#include "gui/GuiManager.hpp"
#include "gui/MainWindowManager.hpp"
#include "console/Console.hpp"

class XBuilderContext;

class XBuilderContextEvent {
public:
    virtual void processEvent(XBuilderContext& context) = 0;
    virtual ~XBuilderContextEvent() { }
    
protected:
    void setContextCurrentProject(XBuilderContext& context, Project* newProject);
};

class XBuilderContextChangeProjectEvent : public XBuilderContextEvent {
public:
    XBuilderContextChangeProjectEvent(Project* newProject_) : newProject(newProject_) { }
    
    void processEvent(XBuilderContext& context) {
        setContextCurrentProject(context, newProject);
    }
    
private:
    Project* newProject;
};

class XBuilderContext {
public:
    XBuilderContext(MainWindowManager& mainWindowManager_)
        : mainWindowManager(mainWindowManager_),
        currentProject(nullptr), 
        guiManager(nullptr),
        console(*this)
        {
            createNewProject();
        }
    
    void createNewProject() {
        Project* newProject = new Project;
        changeCurrentProject(newProject);
    }
    
    void changeCurrentProject(Project* newCurrentProject) {
        addEvent(new XBuilderContextChangeProjectEvent(newCurrentProject));
    }
    
    void processQueuedEvents() {
        while(!eventQueue.empty()) {
            XBuilderContextEvent* event = eventQueue.front();
            eventQueue.pop();            
            event->processEvent(*this);
            delete event;
        }
    }
    
    void addEvent(XBuilderContextEvent* event) {
        eventQueue.push(event);
    }
    
    void enterMainLoop() {
        while(!mainWindowManager.userWantsToQuit()) {
            processQueuedEvents();
            
            assert(guiManager);
            assert(currentProject);
            
            mainWindowManager.beginFrame();
            guiManager->render();
            mainWindowManager.endFrame();
        }
    }
    
    Project& getProject() {
        return *currentProject;
    }
    
    Console& getConsole() {
        return console;
    }
    
    ~XBuilderContext() {
        releaseCurrentState();
    }
    
private:
    void releaseCurrentState() {
        if(guiManager) delete guiManager;
        if(currentProject) delete currentProject;
    }
    
    void performSetCurrentProject(Project* newCurrentProject) {
        releaseCurrentState();
        
        currentProject = newCurrentProject;
        guiManager = new GuiManager(*this);
    }
    
    MainWindowManager& mainWindowManager;
    Project* currentProject;
    GuiManager* guiManager;
    Console console;
    
    std::queue<XBuilderContextEvent*> eventQueue;
    
    friend class XBuilderContextEvent;
};

