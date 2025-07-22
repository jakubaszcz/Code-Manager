#pragma once

#include "../data/Data.hpp"
#include "../software/Software.hpp"
#include <memory>
#include <vector>
#include <iostream>

class Application {
    public:

        Application();
        ~Application();

        bool Initialize();
        int Run(int argc, char* argv[]);
    private:

		// Data class
        std::shared_ptr<Data> _data;

        // Software class
        std::unique_ptr<Software> _software;

        // Software Type
    protected:
};
