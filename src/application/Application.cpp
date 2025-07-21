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
	// Create Application QT
	QApplication app(argc, argv);

	// Allow QTApplication loop
    return app.exec();

}
