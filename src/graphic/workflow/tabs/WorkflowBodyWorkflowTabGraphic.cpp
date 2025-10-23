#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QStyle>
#include "../../../../includes/graphic/WorkflowGraphic.hpp"
#include "../../../../includes/graphic/utils/popup/NamePopup.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawWorkflowTab(QWidget *body) {

    // If the body don't exist return
    if (!body) return;

    // Clear the keyboard event
    {
        _keyboardEventWorkflow.clear();
        _currentKeyboardEventWorkflow = 0;
    }

    // If the body has no layout, create one
    {
        _workflowGlobalLayout = qobject_cast<QVBoxLayout *>(body->layout());

        if (!_workflowGlobalLayout) {
            _workflowGlobalLayout = new QVBoxLayout(body);
            _workflowGlobalLayout->setContentsMargins(0, 0, 0, 0);
            _workflowGlobalLayout->setSpacing(0);
        }
    }

    // Constructor for the button box + keyBoard events
    CONST_WorkflowButtonsBox();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QWidget *WorkflowGraphic::FileManagerRow(int id) {

    // Create widget
    auto *widget = new QWidget;

    // Create widget's layout
    auto *layout = new QHBoxLayout(widget);

    // Set layout's properties
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Create button
    const auto title = "Open File Manager";

    auto *button = new QPushButton(title, widget);
    if (!button)
        return nullptr;

    // Button style
    {
        _application->StyleSheetMultiple(button, "workflow", id, _currentKeyboardEventWorkflow);
    }

    // Event
    {

        QObject::connect(button, &QPushButton::clicked, [this]() {
            const auto cfg = _application->GetData()->GetConfigMap();
            const auto it = cfg.find("wpath");
            const QString startDir = (it != cfg.end()) ? QString::fromStdString(it->second) : QString();

            QDesktopServices::openUrl(QUrl::fromLocalFile(startDir));
        });
    }


    // Add button to layout
    layout->addWidget(button, 1);

    // Set focus proxy
    widget->setFocusProxy(button);

    // Return
    return widget;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QWidget *WorkflowGraphic::TerminalRow(int id) {

    // Create widget
    auto *widget = new QWidget;

    // Create layout
    auto *layout = new QHBoxLayout(widget);

    // Set layout's properties
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    const auto title = "Open Terminal";

    // Create button
    auto *button = new QPushButton(title, widget);
    if (!button)
        return nullptr;

    // Button style
    {
        _application->StyleSheetMultiple(button, "workflow", id, _currentKeyboardEventWorkflow);
    }

    // Event
    {

        QObject::connect(button, &QPushButton::clicked, [this]() {
            const auto cfg = _application->GetData()->GetConfigMap();
            const auto it = cfg.find("wpath");
            const QString wpath = (it != cfg.end()) ? QString::fromStdString(it->second) : QString();
            if (wpath.isEmpty())
                return;


            #ifdef Q_OS_WIN
                        QProcess::startDetached("cmd.exe", QStringList(), wpath);
            #elif defined(Q_OS_MAC)
                        QProcess::startDetached("open", QStringList() << "-a" << "Terminal" << wpath);
            #else
                        bool started = false;
                        started = QProcess::startDetached("x-terminal-emulator",
                                                          QStringList() << "--working-directory" << wpath);
                        if (!started) {
                            started = QProcess::startDetached("gnome-terminal",
                                                              QStringList() << ("--working-directory=" + wpath));
                        }
                        if (!started) {
                            started = QProcess::startDetached("konsole",
                                                              QStringList() << "--workdir" << wpath);
                        }
                        if (!started) {
                            const QString cmd = QString("cd \"%1\"; exec $SHELL -i").arg(wpath);
                            started = QProcess::startDetached("xterm",
                                                              QStringList() << "-e" << "bash" << "-lc" << cmd);
                        }
                        Q_UNUSED(started);
            #endif
        });
    }

    // Add button to layout
    layout->addWidget(button, 1);

    // Set focus proxy
    widget->setFocusProxy(button);

    // Return
    return widget;
}


QWidget *WorkflowGraphic::TouchFileRow(int id) {

    // Create widget
    auto *widget = new QWidget;

    // Create widget's layout
    auto *layout = new QHBoxLayout(widget);

    // Set layout's properties
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Create button
    const auto title = "Create File";

    auto *button = new QPushButton(title, widget);
    if (!button)
        return nullptr;

    // Button style
    {
        _application->StyleSheetMultiple(button, "workflow", id, _currentKeyboardEventWorkflow);
    }

    // Event
    {

        QObject::connect(button, &QPushButton::clicked, [this, widget]() {
            NamePopup popup(widget, _application);
            popup.OpenNear(widget);

            if (popup.exec() == QDialog::Accepted) {
                const std::string& name = popup.GetName();

                if (name.empty()) return;

                const auto oldDir = QDir::currentPath();
                const auto newDir = QString::fromStdString(_application->GetData()->GetConfigMap()["wpath"]);
                if (!newDir.isEmpty())
                QDir::setCurrent(newDir);

                std::ofstream outfile(name);
                if (outfile.is_open()) {
                    outfile.close();
                }

                QDir::setCurrent(oldDir);

            }
        });
    }


    // Add button to layout
    layout->addWidget(button, 1);

    // Set focus proxy
    widget->setFocusProxy(button);

    // Return
    return widget;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::CONST_WorkflowButtonsBox() {

    // Destroy button box if already existing
    {

        if (_workflowButtonBox) {
            _workflowButtonBox = nullptr;
        }
        _keyboardEventWorkflow.clear();

    }

    // Create button box
    _workflowButtonBox = new QWidget();

    // Keyboard events
    {
        _workflowTabUp = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up), _workflowButtonBox);
        _workflowTabDown = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down), _workflowButtonBox);
        _workflowTabEnter= new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), _workflowButtonBox);
    }

    // Create vertical box layout
    auto *boxLayout = new QVBoxLayout(_workflowButtonBox);


    // Connect keyboard events
    {
        QObject::connect(_workflowTabUp, &QShortcut::activated, _workflowButtonBox, [this]() {
            int previous = _currentKeyboardEventWorkflow;
            _currentKeyboardEventWorkflow--;
            if (_currentKeyboardEventWorkflow < 0) _currentKeyboardEventWorkflow = _keyboardEventWorkflow.size() - 1;

            UPDT_WorflowButton(previous, _currentKeyboardEventWorkflow);
        });

        QObject::connect(_workflowTabDown, &QShortcut::activated, _workflowButtonBox, [this]() {
            int previous = _currentKeyboardEventWorkflow;
            _currentKeyboardEventWorkflow = (_currentKeyboardEventWorkflow + 1) % _keyboardEventWorkflow.size();
            UPDT_WorflowButton(previous, _currentKeyboardEventWorkflow);
        });

        {
            QObject::connect(_workflowTabEnter, &QShortcut::activated, _workflowButtonBox, [this]() {
                QWidget *w = _keyboardEventWorkflow[_currentKeyboardEventWorkflow];
                if (w) {
                    QPushButton *btn = w->findChild<QPushButton *>();
                    if (btn) {
                        btn->click();
                    }
                }
            });
        }

    }

    // Create buttons
    {
        if (QWidget *w = FileManagerRow(0)) {
            boxLayout->addWidget(w);
            _keyboardEventWorkflow.push_back(w);
        }
        if (QWidget *w = TerminalRow(1)) {
            boxLayout->addWidget(w);
            _keyboardEventWorkflow.push_back(w);
        }
        if (QWidget *w = TouchFileRow(2)) {
            boxLayout->addWidget(w);
            _keyboardEventWorkflow.push_back(w);
        }
    }

    // Add stretch
    boxLayout->addStretch();
    _workflowGlobalLayout->addWidget(_workflowButtonBox);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::UPDT_WorflowButton(int previous, int current) {

    /*
     * Update Buttons
     * Fetch the previous button
     * Fetch the current button
     * Swap them
     */

    // Set the previous to false
    if (previous >= 0 && previous < static_cast<int>(_keyboardEventWorkflow.size())) {
        if (auto btn = _keyboardEventWorkflow[previous]->findChild<QPushButton*>()) {
            btn->setProperty("active", "false");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
            btn->update();
        }
    }

    // Set the current to true
    if (current >= 0 && current < static_cast<int>(_keyboardEventWorkflow.size())) {
        if (auto btn = _keyboardEventWorkflow[current]->findChild<QPushButton*>()) {
            btn->setProperty("active", "true");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
            btn->update();
        }
    }
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────