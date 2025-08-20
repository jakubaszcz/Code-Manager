#pragma once

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
#include <memory>
#include <vector>
#include "../application/Application.hpp"
#include "IGraphic.hpp"

class BodyGraphic : public IGraphic<QVBoxLayout *> {
public:
    BodyGraphic(std::shared_ptr<Application> application);

    QWidget *fileManagerRow();
    QWidget *commandRow();
    QWidget *terminalRow();
    void Draw(QVBoxLayout *) override;

private:
    std::vector<QWidget *> _rows;
    int _currentRow;
};
