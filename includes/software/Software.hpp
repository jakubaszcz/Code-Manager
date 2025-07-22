#pragma once

#include "../data/Data.hpp"
#include <memory>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QWidget>

enum class SoftwareState {
  Menu,
  Panel
};

class Software {
    public:
        Software(std::shared_ptr<Data> data);

        void Main();

        void MainMenu();
        void MainPanel();
    private:
        SoftwareState _softwareState;
        std::shared_ptr<Data> _data;
    protected:
};