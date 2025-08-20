#include <iostream>
#include <memory>
#include "includes/application/Application.hpp"
#include "includes/error/Error.hpp"

int main(int argc, char *argv[]) {
    try {
        std::shared_ptr<Application> application = std::make_shared<Application>();
        if (application->Initialize())
            application->Run(argc, argv);
        else
            THROW_ERROR("Cannot initialize application");
    } catch (const Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
