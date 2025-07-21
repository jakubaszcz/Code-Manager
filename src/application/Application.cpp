#include "../../includes/application/Application.hpp"
#include "../../includes/error/Error.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>


Application::Application() {
}

Application::~Application() {
}

bool Application::Initialize() {
    try {
        // Make it unique otherwise the Initialize wont work
        _data = std::make_unique<Data>();

        // Initialize data
        if (!_data->Initialize())
			THROW_ERROR("Failed to initialize data");

        // Starting openGL

    } catch(const Error &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

int Application::Run(int argc, char *argv[]) {
	    QApplication app(argc, argv);

    QWidget window;
    window.resize(800, 600);

    // Supprimer les bordures et la barre de titre
    window.setWindowFlags(Qt::FramelessWindowHint);

    // Optionnel : couleur de fond pour voir la fenêtre
    window.setStyleSheet("background-color: red;");

    window.show();

    return app.exec();
}
