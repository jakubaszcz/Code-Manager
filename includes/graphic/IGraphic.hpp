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
#include <memory>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class Application;


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


template<class Parent>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class IGraphic {
public:
    IGraphic(std::shared_ptr<Application> application) : _application(application) {};

    virtual void Draw(Parent) = 0;

private:
protected:
    std::shared_ptr<Application> _application;
};
