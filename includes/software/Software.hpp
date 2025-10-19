#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include <memory>
#include <vector>
#include "../audio/Audio.hpp"
#include "../graphic/IGraphic.hpp"
#include "../graphic/MenuGraphic.hpp"
#include "../graphic/WorkflowGraphic.hpp"
#include "../graphic/SettingsGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class Application;
class HeaderGraphic;
class MenuGraphic;
class WorkflowGraphic;
class SettingsGraphic;

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class Software {
public:
    struct WindowSize {
        int width;
        int height;
    };

    explicit Software(std::shared_ptr<Application> application);

    void Draw();
    void SetWindows();
    void SetWindowSize(WindowSize);
    WindowSize GetWindowSize() const;

private:
    // Graphics addon for the software
    std::unique_ptr<WorkflowGraphic> _workflowGraphic;
    std::unique_ptr<MenuGraphic> _menuGraphic;
    std::unique_ptr<SettingsGraphic> _settingsGraphic;

    // Application with everything I need
    std::shared_ptr<Application> _application;
    WindowSize _windowSize{1200, 800};
    QShortcut *_softwareShortcutEscape = nullptr;
};
