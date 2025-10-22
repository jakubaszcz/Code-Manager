#include "../../includes/application/Application.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <memory>
#include "../../includes/error/Error.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


Application::Application() {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


Application::~Application() {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


bool Application::Initialize() {
    // Try to initialize
    try {

        _software = std::make_unique<Software>(shared_from_this());

        _audio = std::make_shared<Audio>();

        // Make it unique otherwise the Initialize wont work
        _data = std::make_shared<Data>();

        SetCurrentWindow(Windows::Workflow);

        // Initialize data
        if (!_data->Initialize())
            THROW_ERROR("Failed to initialize data");

    } catch (const Error& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


std::shared_ptr<Data> Application::GetData() const {
    return _data;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


std::shared_ptr<Audio> Application::GetAudio() const {
    return _audio;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Application::SetCurrentWindow(Windows window) {
    _currentWindow = window;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


Windows Application::GetCurrentWindow() const {
    return _currentWindow;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────



int Application::Run(int argc, char *argv[]) {
    // Create application
    QApplication app(argc, argv);

    // Add QSS file to a list
    QStringList qssFiles = {
        ":/styles/main/WorkflowTab.qss",
        ":/styles/main/CommandTab.qss",
        ":/styles/main/BodyGraphic.qss",
        ":/styles/main/HeaderGraphic.qss",
        ":/styles/main/MenuGraphic.qss",
        ":/styles/main/Window.qss",
    };

    // Merged all those file to have one big QSS
    QString mergedQss;

    for (const QString &path : qssFiles) {
        QFile file(path);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            mergedQss += "\n/* " + path + " */\n" + file.readAll() + "\n";
            file.close();
        } else {
            return 1;
        }
    }

    // Apply the merged QSS
    app.setStyleSheet(mergedQss);

    // Draw
    _software->Draw();

    // Need to return this to run the app
    return app.exec();
}