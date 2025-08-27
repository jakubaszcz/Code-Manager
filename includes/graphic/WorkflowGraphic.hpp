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

class WorkflowGraphic : public IGraphic<QVBoxLayout *> {
public:
    WorkflowGraphic(std::shared_ptr<Application> application);

    QWidget *fileManagerRow();
    QWidget *commandRow();
    QWidget *terminalRow();
    void Draw(QVBoxLayout *) override;
    void DrawHeader(QVBoxLayout *);
    void DrawBody(QVBoxLayout *);

private:
    std::vector<QWidget *> _rows;
    int _currentRow;
};
