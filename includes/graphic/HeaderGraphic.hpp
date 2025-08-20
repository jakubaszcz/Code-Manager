#pragma once

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
#include "../application/Application.hpp"
#include "IGraphic.hpp"


class HeaderGraphic : public IGraphic<QVBoxLayout *> {
public:
    HeaderGraphic(std::shared_ptr<Application> application);

    void Draw(QVBoxLayout *) override;
};
