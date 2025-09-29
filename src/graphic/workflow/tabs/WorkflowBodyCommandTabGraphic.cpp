#include <QScrollArea>
#include <iostream>
#include <string>
#include "../../../../includes/graphic/WorkflowGraphic.hpp"
#include "../../../../includes/graphic/utils/popup/NamePopup.hpp"
#include "../../../../includes/graphic/utils/popup/TerminalPopup.hpp"

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawCommandTab(QWidget *body) {

    _keyboardEventCommand.clear();

    auto *layout = qobject_cast<QVBoxLayout *>(body->layout());
    if (!layout) {
        layout = new QVBoxLayout(body);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
    }

    QWidget *cmdWidget = new QWidget(body);
    AddCommand(cmdWidget);
    layout->addWidget(cmdWidget);

    _commandTabUp = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up), cmdWidget);
    _commandTabDown = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down), cmdWidget);
    _commandTabEnter = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), cmdWidget);

    QObject::connect(_commandTabUp, &QShortcut::activated, cmdWidget, [this]() {
        _currentKeyboardEventCommand--;
        if (_currentKeyboardEventCommand < 0) _currentKeyboardEventCommand = _keyboardEventCommand.size() - 1;
        RebuildBody();
    });

    QObject::connect(_commandTabDown, &QShortcut::activated, cmdWidget, [this]() {
        _currentKeyboardEventCommand = (_currentKeyboardEventCommand + 1) % _keyboardEventCommand.size();
        RebuildBody();
    });

    auto *scroll = new QScrollArea(body);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    scroll->setStyleSheet(R"(
        QScrollBar:vertical {
            border: none;
            background: #1e1e1e;
            width: 12px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #343434;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background: #1b1b1b;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )");

    layout->addWidget(scroll);

    QWidget *bbox = new QWidget(scroll);
    auto *boxLayout = new QVBoxLayout(bbox);
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->setSpacing(0);




    int id = 0;

    const auto& cfg = _application->GetData()->GetConfigMap();
    for (const auto& [key, value]: cfg) {
        if (key.rfind(_application->GetData()->GetCustomConfigMap()[Data::ConfigType::Command], 0) == 0) {
            if (QWidget *w = Command(key, id)) {
                _keyboardEventCommand.push_back(w);
                boxLayout->addWidget(w, 0, Qt::AlignTop);
                id++;
            }
        }
    }

    QObject::connect(_commandTabEnter, &QShortcut::activated, cmdWidget, [this]() {
        QWidget *w = _keyboardEventCommand[_currentKeyboardEventCommand];
        if (w) {
            QPushButton *btn = w->findChild<QPushButton *>();
            if (btn) {
                btn->click();  // ou btn->animateClick();
            }
}
    });

    boxLayout->addStretch();

    scroll->setWidget(bbox);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::AddCommand(QWidget *container) {
    auto *v = qobject_cast<QVBoxLayout *>(container->layout());
    if (!v) {
        v = new QVBoxLayout(container);
        v->setContentsMargins(0, 0, 0, 0);
        v->setSpacing(0);
    }

    QWidget *row = new QWidget(container);
    auto *h = new QHBoxLayout(row);
    h->setContentsMargins(8, 8, 8, 8);
    h->setSpacing(8);

    auto *btnPlus = new QPushButton("+", row);
    btnPlus->setCursor(Qt::PointingHandCursor);
    btnPlus->setFixedSize(32, 32);
    btnPlus->setStyleSheet(
            "QPushButton { background-color: #1e1e1e; color: white; border: 1px solid #3a3a3a; border-radius: 4px; }"
            "QPushButton:hover { background-color: #2a2a2a; }"
            "QPushButton:pressed { background-color: #171717; }"
            "QPushButton:focus { border: 1px solid #5a5a5a; }");
    h->addWidget(btnPlus, 0, Qt::AlignRight);

    auto addAndRefresh = [container, this
                          /*, this*/]() {
        NamePopup popup(container); // parent QWidget*
        popup.OpenNear(container); // ancre : le bouton +

        _application->GetData()->AddCommand(popup.GetName());
        RebuildBody();
    };

    QObject::connect(btnPlus, &QPushButton::clicked, row, addAndRefresh);

    v->addWidget(row, 0, Qt::AlignTop);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QWidget *WorkflowGraphic::Command(const std::string& code, int id) {
    QWidget *row = new QWidget;
    row->setFocusPolicy(Qt::StrongFocus);
    row->setStyleSheet("QWidget { background-color: #2a2a2a; }");

    auto *h = new QHBoxLayout(row);
    h->setContentsMargins(8, 8, 8, 8);
    h->setSpacing(8);

    auto *btn = new QPushButton("Run", row);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btn->setMinimumWidth(0);
    btn->setFlat(true);
    if (id == _currentKeyboardEventCommand) {
        btn->setStyleSheet("QPushButton { border: none; border-radius: 0px; padding: 8px 12px; background-color: #1b1b1b; "
                       "color: white; }"
                       "QPushButton:hover { background-color: #343434; }"
                       "QPushButton:pressed { background-color: #1b1b1b; }");
    } else {
        btn->setStyleSheet("QPushButton { border: none; border-radius: 0px; padding: 8px 12px; background-color: #2a2a2a; "
               "color: white; }"
               "QPushButton:hover { background-color: #343434; }"
               "QPushButton:pressed { background-color: #1b1b1b; }");
    }

    auto *input = new QLineEdit(row);
    {
        const auto cfg = _application->GetData()->GetConfigMap();
        const auto it = cfg.find(code);
        const std::string customCmd = (it != cfg.end()) ? it->second : std::string();
        input->setPlaceholderText(QString::fromStdString(customCmd.empty() ? "Enter custom command." : customCmd));
    }
    input->setClearButtonEnabled(true);
    input->setFixedWidth(240);
    input->setStyleSheet("QLineEdit { background: #1e1e1e; color: white; border: 1px solid #3a3a3a; padding: 6px 8px; }"
                         "QLineEdit:focus { border: 1px solid #5a5a5a; }");

    auto *btnRemove = new QPushButton("-", row);
    btnRemove->setCursor(Qt::PointingHandCursor);
    btnRemove->setFixedSize(32, 32);
    btnRemove->setToolTip("Remove this command");
    btnRemove->setStyleSheet(
            "QPushButton { background-color: #1e1e1e; color: white; border: 1px solid #3a3a3a; border-radius: 4px; }"
            "QPushButton:hover { background-color: #2a2a2a; }"
            "QPushButton:pressed { background-color: #171717; }"
            "QPushButton:focus { border: 1px solid #5a5a5a; }");

    h->addWidget(btn, 1);
    h->addWidget(input, 0, Qt::AlignRight);
    h->addWidget(btnRemove, 0, Qt::AlignRight);

    QObject::connect(input, &QLineEdit::returnPressed, row, [this, input, code]() {
        const QString cmd = input->text().trimmed();
        if (cmd.isEmpty())
            return;
        _application->GetData()->ChangeConfig(code, input->text().toStdString());
        input->setPlaceholderText(cmd);
        input->clear();
    });

    QObject::connect(btn, &QPushButton::clicked, row, [this, input, code, row]() {
        QString cmd = input->text().trimmed();
        if (cmd.isEmpty()) {
            const auto cfg = _application->GetData()->GetConfigMap();
            const auto it = cfg.find(code);
            if (it != cfg.end())
                cmd = QString::fromStdString(it->second);
        }
        if (cmd.isEmpty())
            return;

        const QString oldDir = QDir::currentPath();
        const QString newDir = QString::fromStdString(_application->GetData()->GetConfigMap()["wpath"]);
        if (!newDir.isEmpty())
            QDir::setCurrent(newDir);

        auto* term = new TerminalPopup(row);
term->show();
term->StartCommand(cmd, {});
        term->Draw();

        QDir::setCurrent(oldDir);
    });

    QObject::connect(btnRemove, &QPushButton::clicked, row, [this, code]() {
        _application->GetData()->RemoveCommand(code);
        RebuildBody();
    });

    row->setFocusProxy(btn);
    return row;
}
