#pragma once

#include "../data/Data.hpp"
#include <memory>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>

class Software {
    public:
        Software(std::shared_ptr<Data> data);

        void Main();
        void DrawHeader(QWidget* window, QVBoxLayout *mainLayout);
        void DrawPanel(QWidget* window, QVBoxLayout *mainLayout);

    private:
        std::shared_ptr<Data> _data;
    protected:
};