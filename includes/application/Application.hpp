#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <any>
#include <iostream>
#include <memory>
#include <vector>
#include "../audio/Audio.hpp"
#include "../data/Data.hpp"
#include "../software/Software.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class Software;


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


enum class Windows {
    Menu,
    Workflow,
    Setting
};


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class Application : public std::enable_shared_from_this<Application> {
public:
    Application();
    ~Application();


    bool Initialize();
    int Run(int argc, char *argv[]);
    std::shared_ptr<Data> GetData() const;
    std::shared_ptr<Audio> GetAudio() const;
    void SetCurrentWindow(Windows window);
    Windows GetCurrentWindow() const;

    template <typename T>
    void StyleSheetSimple(T widget, std::string component) {
        widget->setObjectName(component.c_str());
        widget->setProperty(component.c_str(), "true");

        widget->setFocusPolicy(Qt::NoFocus);
        widget->setAutoFillBackground(true);
        widget->setAttribute(Qt::WA_StyledBackground, true);

        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
        widget->update();
    }

    template <typename T>
    void StyleSheetMultiple(T widget, std::string component, int compared, int comparator) {
        widget->setObjectName(component.c_str());
        widget->setProperty(component.c_str(), "true");
        widget->setProperty("active", (compared == comparator) ? "true" : "false");

        widget->setFocusPolicy(Qt::NoFocus);
        widget->setAutoFillBackground(true);
        widget->setAttribute(Qt::WA_StyledBackground, true);

        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
        widget->update();
    }

private:
    // Data class
    std::shared_ptr<Data> _data;
    std::shared_ptr<Audio> _audio;
    std::unique_ptr<Software> _software;
    Windows _currentWindow;

protected:
};
