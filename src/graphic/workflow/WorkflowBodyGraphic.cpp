#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include "../../../includes/graphic/WorkflowGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawBody(QVBoxLayout *layout) {

    // Return if no layout
    if (!layout) return;

    // Create layout
    if (_layout == nullptr) {
        auto *body = new QWidget;
        _layout = new QVBoxLayout(body);
        _layout->setContentsMargins(0, 0, 0, 0);
        _layout->setSpacing(0);
        layout->addWidget(body);
    }

    // Draw tabs
    {
        _tabsWindow = {
            {Tab::Workflow,
            [this]() {
                if (_layout && _layout->parentWidget()) {
                    this->DrawWorkflowTab(_layout->parentWidget());
                }
            }},
           {Tab::Command,
            [this]() {
                if (_layout && _layout->parentWidget()) {
                    this->DrawCommandTab(_layout->parentWidget());
                }
           }}
        };
    }

    // Rebuild the body
    RebuildBody();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawTabs(QWidget *body) {

    // Create layout
    auto *layout = qobject_cast<QVBoxLayout *>(body->layout());

    // Create button event
    std::vector<std::pair<std::string, std::function<void()>>> buttonsEvent = {
            {"Workflow", [this]() { SetTab(Tab::Workflow); }},
            {"Command", [this]() { SetTab(Tab::Command); }},
            //{"Application", [this]() { SetTab(Tab::Application); }}
    };

    // Create widget
    auto *widget = new QWidget;

    // Create layout
    auto *boxLayout = new QHBoxLayout(widget);
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->setSpacing(0);

    // Draw tabs
    for (int i = 0; i < static_cast<int>(_keyboardEventTab.size()); ++i) {
        const auto& tab = _keyboardEventTab[i];
        const auto& [name, event] = buttonsEvent[i];

        auto *button = Tabs(name, event, tab);
        boxLayout->addWidget(button, 1);
    }

    // Keybinds
    auto *right = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right), widget);
    auto *left = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Left), widget);

    // Event
    QObject::connect(right, &QShortcut::activated, widget, [this]() {
        _currentKeyboardEventTab = (_currentKeyboardEventTab + 1) % _keyboardEventTab.size();
        SetTab(_keyboardEventTab[_currentKeyboardEventTab]);
    });

    QObject::connect(left, &QShortcut::activated, widget, [this]() {
        _currentKeyboardEventTab--;
        if (_currentKeyboardEventTab < 0) _currentKeyboardEventTab = _keyboardEventTab.size() - 1;
        SetTab(_keyboardEventTab[_currentKeyboardEventTab]);
    });

    // Set size policy
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Add widget
    layout->addWidget(widget);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QPushButton *WorkflowGraphic::Tabs(const std::string& label, std::function<void()> onClick, Tab tab) {
    // Create button
    auto *button = new QPushButton(QString::fromStdString(label));

    button->setCursor(Qt::PointingHandCursor);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    button->setMinimumWidth(0);
    button->setFlat(true);
    button->setFocusPolicy(Qt::NoFocus);

    // Style
    button->setObjectName("tab");
    button->setProperty("tab", "true");
    button->setProperty("active", (tab == _tab) ? "true" : "false");

    button->setAutoFillBackground(true);
    button->setAttribute(Qt::WA_StyledBackground, true);

    button->style()->unpolish(button);
    button->style()->polish(button);
    button->update();

    // Event
    QObject::connect(button, &QPushButton::clicked, button, [onClick]() { onClick(); });

    // Return
    return button;
}
