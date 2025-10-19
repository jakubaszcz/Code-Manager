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
    try {

        _software = std::make_unique<Software>(shared_from_this());

        _audio = std::make_shared<Audio>();

        // Make it unique otherwise the Initialize wont work
        _data = std::make_shared<Data>();

        SetCurrentWindow(Windows::Workflow);

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
    QApplication app(argc, argv);


    QStringList qssFiles = {
        ":/styles/main/WorkflowTab.qss",
        ":/styles/main/CommandTab.qss",
        ":/styles/main/BodyGraphic.qss",
        ":/styles/main/HeaderGraphic.qss",
        ":/styles/main/MenuGraphic.qss",
    };

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

    app.setStyleSheet(mergedQss);

    _software->Draw();

    return app.exec();
}

QString Application::LoadQSSFile(const QString &path) {
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return "";
    return QLatin1String(file.readAll());
}