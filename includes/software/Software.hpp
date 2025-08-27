#pragma once

#include <memory>
#include <vector>
#include "../audio/Audio.hpp"
#include "../graphic/IGraphic.hpp"
#include "../graphic/MenuGraphic.hpp"
#include "../graphic/WorkflowGraphic.hpp"

// Forward declaration pour éviter les includes lourds ici
class Application;
class HeaderGraphic;
class MenuGraphic;
class WorkflowGraphic;

class Software {
public:
    explicit Software(std::shared_ptr<Application> application);

    void Draw();

private:
    // Graphics addon for the software
    std::unique_ptr<WorkflowGraphic> _workflowGraphic;
    std::unique_ptr<MenuGraphic> _menuGraphic;

    // Application with everything I need
    std::shared_ptr<Application> _application;
};
