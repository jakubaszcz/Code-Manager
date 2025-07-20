#pragma once

#include "../listeners/IListener.hpp"
#include "../data/Data.hpp"
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Application {
    public:
        struct GLFWwindowDeleter {
            void operator()(GLFWwindow* ptr) const {
                if (ptr) glfwDestroyWindow(ptr);
            }
        };

        Application();
        ~Application();

        bool Initialize();
        void Run();

        void Subscribe(const IListener&);
        void UnSubscribe(const IListener&);
    private:
        // Window
        std::unique_ptr<GLFWwindow, GLFWwindowDeleter> _window;

        // Subscribers
        std::vector<IListener> _subscribers;

        std::unique_ptr<Data> _data;
    protected:
};
