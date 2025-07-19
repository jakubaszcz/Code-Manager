#include "../../includes/application/Application.hpp"
#include "../../includes/error/Error.hpp"

Application::Application() {
}

Application::~Application() {
    glfwTerminate();
}

bool Application::Initialize() {
    try {

        // Initialize GLFW
        if (!glfwInit())
            THROW_ERROR("Failed to initialize GLFW");

        // Specified GLFW version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _window = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>(
            glfwCreateWindow(800, 600, "Fenêtre OpenGL", nullptr, nullptr)
        );

        if (!_window)
            THROW_ERROR("Failed to create GLFW window");

        // Make the contexte glfwMakeContextCurrent(_window.get());
        glfwMakeContextCurrent(_window.get());

        // Load gladLoader
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            THROW_ERROR("Failed to initialize GLAD");

        // Setup the viewport
        glViewport(0, 0, 800, 600);

        // Initialize data
        if (!_data->Initialize())
			THROW_ERROR("Failed to initialize data");
    } catch(const Error &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

void Application::Run() {
    std::cout << "run" << std::endl;

    while (1) {
      std::cout << "1" << std::endl;
    }
}
