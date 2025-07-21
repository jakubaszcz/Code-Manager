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

        void Subscribe(const IListener&);
        void UnSubscribe(const IListener&);
    private:
        // Window

        // Subscribers
        std::vector<IListener> _subscribers;

        std::unique_ptr<Data> _data;
    protected:
};
