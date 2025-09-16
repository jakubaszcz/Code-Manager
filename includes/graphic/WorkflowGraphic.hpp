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
    void RebuildBody();
    void DrawHeader(QVBoxLayout *);
    void DrawBody(QVBoxLayout *);

    void DrawTabs(QWidget *);
    QPushButton *Tabs(const std::string&, std::function<void()>, Tab);

    void DrawWorkflowTab(QWidget *);
    QWidget *FileManagerRow();
    QWidget *TerminalRow();

    void DrawCommandTab(QWidget *);
    void AddCommand(QWidget *);
    QWidget *Command(const std::string&);

    void DrawApplicationTab(QWidget *);
    void AddApplication(QWidget *);
    QWidget *ApplicationButton(const std::string&);


    std::vector<QWidget *> _rowsWorkflowTab;
    std::unordered_map<Tab, std::function<void()>> _tabsWindow;
    Tab _tab{Tab::Workflow};
    int _currentRow{0};

    std::vector<Tab> _keyboardEventTab{Tab::Workflow, Tab::Command};
    int _currentKeyboardEventTab{0};

    QVBoxLayout *_layout{nullptr};
};

