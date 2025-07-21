#pragma once

#include "../listeners/IListener.hpp"
#include "../data/Data.hpp"
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
        std::unique_ptr<Data> _data;
    protected:
};
