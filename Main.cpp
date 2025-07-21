#include "includes/application/Application.hpp"
#include <iostream>
#include "includes/error/Error.hpp"

int main(int argc, char *argv[]) {
    try {
        Application application;

        if (application.Initialize())
            application.Run(argc, argv);
        else
            THROW_ERROR("Cannot initialize application");
    } catch (const Error &e) {
        std::cerr << e.what() << std::endl;
    }
}
