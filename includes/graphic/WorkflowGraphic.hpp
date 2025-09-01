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
#include <functional>
#include <memory>
#include <vector>
#include "../application/Application.hpp"
#include "IGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


enum class Tab { Workflow, Command, Application };


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class WorkflowGraphic : public IGraphic<QVBoxLayout *> {
public:
    WorkflowGraphic(std::shared_ptr<Application> application);
    void Draw(QVBoxLayout *) override;
    void SetTab(Tab tab);
    Tab GetTab();

private:
    void DrawHeader(QVBoxLayout *);

    void DrawBody(QVBoxLayout *);

    void DrawTabs(QVBoxLayout *);
    QWidget *DrawWorkflowTab();
    QWidget *DrawCommandTab();
    QWidget *DrawApplicationTab();
    QPushButton *Tabs(const std::string&, std::function<void()>);

    QWidget *FileManagerRow();
    QWidget *CommandRow();
    QWidget *TerminalRow();

    std::vector<QWidget *> _rows;
    Tab _tab{Tab::Workflow};
    int _currentRow{0};
};
