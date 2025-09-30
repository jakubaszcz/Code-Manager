#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QProcess>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <QShortcut>
#include <functional>
#include <memory>
#include <vector>
#include "../application/Application.hpp"
#include "IGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class SettingsGraphic : public IGraphic<QVBoxLayout *> {
public:
    SettingsGraphic(std::shared_ptr<Application> application);
    void Draw(QVBoxLayout *) override;

private:

    QVBoxLayout *_layout{nullptr};
};

