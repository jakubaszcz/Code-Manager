#include "../../includes/application/Application.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <memory>
#include "../../includes/error/Error.hpp"


Application::Application() {
}

Application::~Application() {
}

bool Application::Initialize() {
    try {

        _software = std::make_unique<Software>(shared_from_this());

        _audio = std::make_shared<Audio>();

        // Make it unique otherwise the Initialize wont work
        _data = std::make_shared<Data>();

        // Initialize data
        if (!_data->Initialize())
            THROW_ERROR("Failed to initialize data");

        // Starting openGL

    } catch (const Error& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

std::shared_ptr<Data> Application::GetData() const {
    return _data;
}

std::shared_ptr<Audio> Application::GetAudio() const {
    return _audio;
}


#include "../../includes/graphic/IGraphic.hpp"

int Application::Run(int argc, char *argv[]) {
    QApplication app(argc, argv);

    _software->Draw();

    return app.exec();
}
