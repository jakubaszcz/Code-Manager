#include "../../includes/graphic/MenuGraphic.hpp"
#include <QPushButton>
#include <QShortcut>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


MenuGraphic::MenuGraphic(std::shared_ptr<Application> application) : IGraphic<QVBoxLayout *>(std::move(application)) {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void MenuGraphic::Draw(QVBoxLayout *layout) {

    // Reset
    {
        _buttons.clear();
        _currentButton = 0;
    }

    QWidget *parentWidget = layout->parentWidget();

    {
        _menuGraphicShortcutLeft = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Left), parentWidget);
        _menuGraphicShortcutRight = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right), parentWidget);
        _menuGraphicShortcutEnter = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), parentWidget);
    }

    if (!_menuGraphicShortcutLeft && !_menuGraphicShortcutRight) std::cout << "Shortcuts not created" << std::endl;

    {
        QObject::connect(_menuGraphicShortcutLeft, &QShortcut::activated, parentWidget, [this]() {
            int previous = _currentButton;
            _currentButton--;
            if (_currentButton < 0) _currentButton = _buttons.size() - 1;

            UPDT_MenuButton(previous, _currentButton);
        });

        QObject::connect(_menuGraphicShortcutRight, &QShortcut::activated, parentWidget, [this]() {
            int previous = _currentButton;
            _currentButton = (_currentButton + 1) % _buttons.size();


            UPDT_MenuButton(previous, _currentButton);
        });

        {
            QObject::connect(_menuGraphicShortcutEnter, &QShortcut::activated, parentWidget, [this]() {
                QPushButton *button = _buttons[_currentButton];
                if (button) {
                    button->click();
                }
            });
        }
    }

    if (QPushButton *pb = WorkflowButton(0))
        _buttons.push_back(pb);
    if (QPushButton *pb = SettingButton(1))
        _buttons.push_back(pb);
    if (QPushButton *pb = ExitButton(2))
        _buttons.push_back(pb);

    QVBoxLayout *centerColumn = new QVBoxLayout();
    centerColumn->setAlignment(Qt::AlignCenter);

    QHBoxLayout *row = new QHBoxLayout();
    row->setSpacing(48);
    row->setAlignment(Qt::AlignCenter);

    for (auto *button: _buttons) {
        row->addWidget(button);
    }
    centerColumn->addLayout(row);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch();
    layout->addLayout(centerColumn);
    layout->addStretch();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QPushButton *MenuGraphic::WorkflowButton(int id) {
    QPushButton *workflowButton = new QPushButton("🖥️");

    workflowButton->setCursor(Qt::PointingHandCursor);
    workflowButton->setFocusPolicy(Qt::StrongFocus);
    workflowButton->setFixedSize(96, 96);

    {
        _application->StyleSheetMultiple(workflowButton, "menu-button", id, _currentButton);
    }

    // Event
    {

        QObject::connect(workflowButton, &QPushButton::clicked, workflowButton, [this]() {
            _application->SetCurrentWindow(Windows::Workflow);
            if (_redraw)
                _redraw();
        });
    }
    return workflowButton;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QPushButton *MenuGraphic::SettingButton(int id) {
    QPushButton *settingButton = new QPushButton("🛠️");

    settingButton->setCursor(Qt::PointingHandCursor);
    settingButton->setFocusPolicy(Qt::StrongFocus);
    settingButton->setFixedSize(96, 96);

    {

        _application->StyleSheetMultiple(settingButton, "menu-button", id, _currentButton);
    }

    // Event
    {

        QObject::connect(settingButton, &QPushButton::clicked, settingButton, [this]() {
            _application->SetCurrentWindow(Windows::Setting);
            if (_redraw)
                _redraw();
        });
    }
    return settingButton;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QPushButton *MenuGraphic::ExitButton(int id) {
    QPushButton *exitButton = new QPushButton("🚪");

    exitButton->setCursor(Qt::PointingHandCursor);
    exitButton->setFocusPolicy(Qt::StrongFocus);
    exitButton->setFixedSize(96, 96);

    {
        _application->StyleSheetMultiple(exitButton, "menu-button", id, _currentButton);

    }

    // Event
    {

        QObject::connect(exitButton, &QPushButton::clicked, exitButton, [this]() { QApplication::quit(); });
    }
    return exitButton;
}


void MenuGraphic::UPDT_MenuButton(int previous, int current) {

    /*
     * Update Buttons
     * Fetch the previous button
     * Fetch the current button
     * Swap them
     */

    // Set the previous to false
    if (previous >= 0 && previous < static_cast<int>(_buttons.size())) {
        if (auto btn = _buttons[previous]) {
            btn->setProperty("active", "false");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
            btn->update();
        }
    }

    // Set the current to true
    if (current >= 0 && current < static_cast<int>(_buttons.size())) {
        if (auto btn = _buttons[current]) {
            btn->setProperty("active", "true");
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
            btn->update();
        }
    }
}