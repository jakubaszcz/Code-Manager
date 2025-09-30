#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include "../../../../includes/graphic/WorkflowGraphic.hpp"

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawWorkflowTab(QWidget *body) {

    if (!body) return;

    {
        _keyboardEventWorkflow.clear();
        _currentKeyboardEventWorkflow = 0;
    }

    _workflowGlobalLayout = qobject_cast<QVBoxLayout *>(body->layout());
    if (!_workflowGlobalLayout) {
        _workflowGlobalLayout = new QVBoxLayout(body);
        _workflowGlobalLayout->setContentsMargins(0, 0, 0, 0);
        _workflowGlobalLayout->setSpacing(0);
    }

    CONST_WorkflowButtonsBox();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QWidget *WorkflowGraphic::FileManagerRow(int id) {
    QWidget *row = new QWidget;
    QHBoxLayout *h = new QHBoxLayout(row);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    auto *btn = new QPushButton("Open File Manager", row);
    if (!btn)
        return nullptr;

    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    row->setFocusPolicy(Qt::NoFocus);

    if (id == _currentKeyboardEventWorkflow) {
        btn->setStyleSheet("QPushButton {"
                   "  border: none;"
                   "  border-radius: 0px;"
                   "  padding: 8px 12px;"
                   "  background-color: #1b1b1b;"
                   "  color: white;"
                   "  text-align: left;"
                   "}"
                   "QPushButton:hover { background-color: #343434; }"
                   "QPushButton:pressed { background-color: #1b1b1b; }");
    } else {
        btn->setStyleSheet("QPushButton {"
                   "  border: none;"
                   "  border-radius: 0px;"
                   "  padding: 8px 12px;"
                   "  background-color: #1e1e1e;"
                   "  color: white;"
                   "  text-align: left;"
                   "}"
                   "QPushButton:hover { background-color: #343434; }"
                   "QPushButton:pressed { background-color: #1b1b1b; }");

    }


    // Event
    {

        QObject::connect(btn, &QPushButton::clicked, [this]() {
            const auto cfg = _application->GetData()->GetConfigMap();
            const auto it = cfg.find("wpath");
            const QString startDir = (it != cfg.end()) ? QString::fromStdString(it->second) : QString();
            QDesktopServices::openUrl(QUrl::fromLocalFile(startDir));
        });
    }


    h->addWidget(btn, 1);

    row->setFocusProxy(btn);

    return row;
}

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QWidget *WorkflowGraphic::TerminalRow(int id) {
    QWidget *row = new QWidget;
    QHBoxLayout *h = new QHBoxLayout(row);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    auto *btn = new QPushButton("Open Terminal", row);
    if (!btn)
        return nullptr;

    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    row->setFocusPolicy(Qt::NoFocus);

    if (id == _currentKeyboardEventWorkflow) {
        btn->setStyleSheet("QPushButton {"
                   "  border: none;"
                   "  border-radius: 0px;"
                   "  padding: 8px 12px;"
                   "  background-color: #1b1b1b;"
                   "  color: white;"
                   "  text-align: left;"
                   "}"
                   "QPushButton:hover { background-color: #343434; }"
                   "QPushButton:pressed { background-color: #1b1b1b; }");
    } else {
        btn->setStyleSheet("QPushButton {"
                   "  border: none;"
                   "  border-radius: 0px;"
                   "  padding: 8px 12px;"
                   "  background-color: #1e1e1e;"
                   "  color: white;"
                   "  text-align: left;"
                   "}"
                   "QPushButton:hover { background-color: #343434; }"
                   "QPushButton:pressed { background-color: #1b1b1b; }");

    }
    // Event
    {

        QObject::connect(btn, &QPushButton::clicked, [this]() {
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

    h->addWidget(btn, 1);

    row->setFocusProxy(btn);

    return row;
}


void WorkflowGraphic::CONST_WorkflowButtonsBox() {

    {
        if (_workflowButtonBox) {
            _workflowButtonBox->deleteLater();
            _workflowButtonBox = nullptr;
        }

        _keyboardEventWorkflow.clear();
    }

    _workflowButtonBox = new QWidget();
    auto *boxLayout = new QVBoxLayout(_workflowButtonBox);

    _workflowTabUp = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up), _workflowButtonBox);
    _workflowTabDown = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down), _workflowButtonBox);
    _workflowTabEnter= new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), _workflowButtonBox);

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

    if (QWidget *w = FileManagerRow(0)) {
        boxLayout->addWidget(w);
        _keyboardEventWorkflow.push_back(w);
    }
    if (QWidget *w = TerminalRow(1)) {
        boxLayout->addWidget(w);
        _keyboardEventWorkflow.push_back(w);
    }

    boxLayout->addStretch();
    _workflowGlobalLayout->addWidget(_workflowButtonBox);
}

void WorkflowGraphic::UPDT_WorflowButton(int previous, int current) {
    if (previous >= 0 && previous < _keyboardEventWorkflow.size()) {
        if (auto btn = _keyboardEventWorkflow[previous]->findChild<QPushButton*>()) {
            btn->setStyleSheet("QPushButton {"
                               "  border: none;"
                               "  border-radius: 0px;"
                               "  padding: 8px 12px;"
                               "  background-color: #1e1e1e;"
                               "  color: white;"
                               "  text-align: left;"
                               "}"
                               "QPushButton:hover { background-color: #343434; }"
                               "QPushButton:pressed { background-color: #1b1b1b; }");
        }
    }

    if (current >= 0 && current < _keyboardEventWorkflow.size()) {
        if (auto btn = _keyboardEventWorkflow[current]->findChild<QPushButton*>()) {
            btn->setStyleSheet("QPushButton {"
                               "  border: none;"
                               "  border-radius: 0px;"
                               "  padding: 8px 12px;"
                               "  background-color: #1b1b1b;"
                               "  color: white;"
                               "  text-align: left;"
                               "}"
                               "QPushButton:hover { background-color: #343434; }"
                               "QPushButton:pressed { background-color: #1b1b1b; }");
        }
    }
}


