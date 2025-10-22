#include <QScrollArea>
#include <iostream>
#include <string>
#include "../../../../includes/graphic/WorkflowGraphic.hpp"
#include "../../../../includes/graphic/utils/popup/NamePopup.hpp"
#include "../../../../includes/graphic/utils/popup/TerminalPopup.hpp"

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawCommandTab(QWidget *body) {

    // If the body don't exist return
    if (!body) return;

    // Clear the keyboard event
    {
        _keyboardEventCommand.clear();
        _currentKeyboardEventCommand = 0;
    }

    // If the body has no layout, create one
    {
        _commandGlobalLayout = qobject_cast<QVBoxLayout *>(body->layout());

        if (!_commandGlobalLayout) {
            _commandGlobalLayout = new QVBoxLayout(body);
            _commandGlobalLayout->setContentsMargins(0, 0, 0, 0);
            _commandGlobalLayout->setSpacing(0);
        }
    }

    {
        // Create command
        CONST_CommandAddButton();

        // Create buttons
        CONST_CommandButtonsBox();
    }
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::CONST_CommandAddButton() {

    // Destroy widget if already exist
    {
        if (_commandAddWidget) {
            _commandAddWidget = nullptr;
        }
    }

    // Create widget
    _commandAddWidget = new QWidget();

    // Create add button
    {
        // Keyboard events
        {
            _commandShortcutAdd = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_A), _commandAddWidget);
        }

        // Connect keyboard events
        {
            QObject::connect(_commandShortcutAdd, &QShortcut::activated, _commandAddWidget, [this]() {
               auto *button = _commandAddWidget->findChild<QPushButton *>();
               if (button) {
                   button->click();
               }
           });
        }

        // Draw add command
        AddCommand();
    }

    // Add button to global Layout
    _commandGlobalLayout->addWidget(_commandAddWidget);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::AddCommand() {

    // Cast the layout
    auto *layout = qobject_cast<QVBoxLayout *>(_commandAddWidget->layout());

    // Create layout if don't exist
    {
        if (!layout) {
            layout = new QVBoxLayout(_commandAddWidget);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
        }
    }

    // Create widget
    auto *widget = new QWidget(_commandAddWidget);

    // Create layout
    auto *wLayout = new QHBoxLayout(widget);
    wLayout->setContentsMargins(8, 8, 8, 8);
    wLayout->setSpacing(8);

    const auto title = "+";

    // Create button
    auto *button = new QPushButton(title, widget);
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedSize(32, 32);

    // Style
    {
        _application->StyleSheetSimple(button, "add");
    }

    // Add button to layout
    wLayout->addWidget(button, 0, Qt::AlignRight);

    // Event
    {
        auto addAndRefresh = [this]() {
            NamePopup popup(_commandAddWidget, _application);
            popup.OpenNear(_commandAddWidget);

            _application->GetData()->AddCommand(popup.GetName());
            RebuildBody();
        };

        QObject::connect(button, &QPushButton::clicked, widget, addAndRefresh);
    }

    // Add widget
    layout->addWidget(widget, 0, Qt::AlignTop);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QWidget *WorkflowGraphic::Command(const std::string& code, int id) {

    // Create widget
    auto *widget = new QWidget;
    widget->setFocusPolicy(Qt::StrongFocus);

    // Create layout
    auto *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(8);

    const auto title = "Run";

    // Create button
    auto *button_execute = new QPushButton(title, widget);
    button_execute->setCursor(Qt::PointingHandCursor);
    button_execute->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    button_execute->setMinimumWidth(0);
    button_execute->setFlat(true);

    // Style
    {
        _application->StyleSheetMultiple(button_execute, "command", id, _currentKeyboardEventCommand);
    }

    // Create line
    auto *input = new QLineEdit(widget);
    {
        const auto cfg = _application->GetData()->GetConfigMap();
        const auto it = cfg.find(code);
        const std::string customCmd = (it != cfg.end()) ? it->second : std::string();
        input->setPlaceholderText(QString::fromStdString(customCmd.empty() ? "Enter custom command." : customCmd));
    }
    input->setClearButtonEnabled(true);

    // Style
    {
        _application->StyleSheetSimple(input, "command");
    }

    // Create button
    auto *button_remove = new QPushButton("-", widget);
    button_remove->setCursor(Qt::PointingHandCursor);
    button_remove->setFixedSize(32, 32);

    // Style
    {
        _application->StyleSheetSimple(button_remove, "remove");
    }


    // Add layout
    layout->addWidget(button_execute, 1);
    layout->addWidget(input, 0, Qt::AlignRight);
    layout->addWidget(button_remove, 0, Qt::AlignRight);

    // Event
    {
        QObject::connect(input, &QLineEdit::returnPressed, widget, [this, input, code]() {
           const auto command = input->text().trimmed();
           if (command.isEmpty())
               return;
           _application->GetData()->ChangeConfig(code, input->text().toStdString());
           input->setPlaceholderText(command);
           input->clear();
       });

        QObject::connect(button_execute, &QPushButton::clicked, widget, [this, input, code, widget]() {
            auto command = input->text().trimmed();
            if (command.isEmpty()) {
                const auto cfg = _application->GetData()->GetConfigMap();
                const auto it = cfg.find(code);
                if (it != cfg.end())
                    command = QString::fromStdString(it->second);
            }
            if (command.isEmpty())
                return;

            const auto oldDir = QDir::currentPath();
            const auto newDir = QString::fromStdString(_application->GetData()->GetConfigMap()["wpath"]);
            if (!newDir.isEmpty())
                QDir::setCurrent(newDir);

            auto* terminal = new TerminalPopup(widget, _application);
            terminal->show();
            terminal->StartCommand(command, {});
            terminal->Draw();

            QDir::setCurrent(oldDir);
        });

        QObject::connect(button_remove, &QPushButton::clicked, widget, [this, code]() {
            _application->GetData()->RemoveCommand(code);
            RebuildBody();
        });
    }

    // Return
    return widget;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::CONST_CommandButtonsBox() {

    // Destroy widget and scroll area if already exist
    {
        if (_commandButtonBox) {
            _commandButtonBox = nullptr;
        }

        if (_commandScrollArea) {
            _commandScrollArea = nullptr;
        }
    }

    // Create button box
    _commandButtonBox = new QWidget();

    // Create scroll area
    {
        _commandScrollArea = new QScrollArea(_commandButtonBox);

        // Style
        _commandScrollArea->setWidgetResizable(true);
        _commandScrollArea->setFrameShape(QFrame::NoFrame);
        _commandScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


        _application->StyleSheetSimple(_commandScrollArea, "scroll");
    }

    // Keyboard events
    {
        _commandTabUp = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up), _commandScrollArea);
        _commandTabDown = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down), _commandScrollArea);
        _commandTabEnter = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), _commandScrollArea);
        _commandShortcutDelete = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), _commandScrollArea);

    }


    // Connect keyboard events
    {
        QObject::connect(_commandTabUp, &QShortcut::activated, _commandScrollArea, [this]() {
            int previous = _currentKeyboardEventCommand;
           _currentKeyboardEventCommand--;
           if (_currentKeyboardEventCommand < 0) _currentKeyboardEventCommand = _keyboardEventCommand.size() - 1;
           UPDT_CommandButton(previous, _currentKeyboardEventCommand);
       });

        QObject::connect(_commandTabDown, &QShortcut::activated, _commandScrollArea, [this]() {
            int previous = _currentKeyboardEventCommand;
            _currentKeyboardEventCommand = (_currentKeyboardEventCommand + 1) % _keyboardEventCommand.size();
            UPDT_CommandButton(previous, _currentKeyboardEventCommand);
        });

        QObject::connect(_commandShortcutDelete, &QShortcut::activated, _commandScrollArea, [this]() {
            auto *widget = _keyboardEventCommand[_currentKeyboardEventCommand];
            if (widget) {
                QPushButton *removeBtn = widget->findChild<QPushButton *>("remove");
                if (removeBtn)
                    removeBtn->click();
                RebuildBody();
            }
        });

        {
            QObject::connect(_commandTabEnter, &QShortcut::activated, _commandScrollArea, [this]() {
               auto *widget = _keyboardEventCommand[_currentKeyboardEventCommand];
               if (widget) {
                   auto *button = widget->findChild<QPushButton *>();
                   if (button) {
                       button->click();
                   }
               }
           });
        }
    }

    // Create widget and layout
    auto *widget = new QWidget(_commandScrollArea);
    auto *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Draw all buttons
    {
        int id = 0;

        const auto& cfg = _application->GetData()->GetConfigMap();
        for (const auto& [key, value]: cfg) {
            if (key.rfind(_application->GetData()->GetCustomConfigMap()[Data::ConfigType::Command], 0) == 0) {
                if (QWidget *w = Command(key, id)) {
                    _keyboardEventCommand.push_back(w);
                    layout->addWidget(w, 0, Qt::AlignTop);
                    id++;
                }
            }
        }
    }

    // Setting
    {
        layout->addStretch();
        _commandScrollArea->setWidget(widget);
        _commandGlobalLayout->addWidget(_commandScrollArea);
    }
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::UPDT_CommandButton(int previous, int current) {

    /*
     * Update Buttons
     * Fetch the previous button
     * Fetch the current button
     * Swap them
     */

    // Set the previous to false
    if (previous >= 0 && previous < static_cast<int>(_keyboardEventCommand.size())) {
        if (auto btn = _keyboardEventCommand[previous]->findChild<QPushButton*>()) {
            btn->setProperty("active", "false");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
            btn->update();
        }
    }

    // Set the current to true
    if (current >= 0 && current < static_cast<int>(_keyboardEventCommand.size())) {
        if (auto btn = _keyboardEventCommand[current]->findChild<QPushButton*>()) {
            btn->setProperty("active", "true");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
            btn->update();
        }
    }
}