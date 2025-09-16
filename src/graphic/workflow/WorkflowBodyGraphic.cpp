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
    if (_layout == nullptr) {
        QWidget *body = new QWidget;
        _layout = new QVBoxLayout(body);
        _layout->setContentsMargins(0, 0, 0, 0);
        _layout->setSpacing(0);
        layout->addWidget(body);
    }

    _tabsWindow = {{Tab::Workflow,
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
                    }},
                   // {Tab::Application, [this]() {
                   //      if (_layout && _layout->parentWidget()) {
                   //          this->DrawApplicationTab(_layout->parentWidget());
                   //      }
                   //  }}
                    };



    RebuildBody();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawTabs(QWidget *body) {
    auto *layout = qobject_cast<QVBoxLayout *>(body->layout());

    std::vector<std::pair<std::string, std::function<void()>>> buttonsEvent = {
            {"Workflow", [this]() { SetTab(Tab::Workflow); }},
            {"Command", [this]() { SetTab(Tab::Command); }},
            //{"Application", [this]() { SetTab(Tab::Application); }}
    };


    QWidget *tabsWidget = new QWidget;
    auto *h = new QHBoxLayout(tabsWidget);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    for (int i = 0; i < _keyboardEventTab.size(); ++i) {
        const auto& tab = _keyboardEventTab[i];
        const auto& [name, event] = buttonsEvent[i];

        QPushButton *btn = Tabs(name, event, tab);
        h->addWidget(btn, 1);
    }


    auto *right = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right), tabsWidget);
    auto *left = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Left), tabsWidget);

    QObject::connect(right, &QShortcut::activated, tabsWidget, [this]() {
        _currentKeyboardEventTab = (_currentKeyboardEventTab + 1) % _keyboardEventTab.size();
        SetTab(_keyboardEventTab[_currentKeyboardEventTab]);
    });

    QObject::connect(left, &QShortcut::activated, tabsWidget, [this]() {
        _currentKeyboardEventTab--;
        if (_currentKeyboardEventTab < 0) _currentKeyboardEventTab = _keyboardEventTab.size() - 1;
        SetTab(_keyboardEventTab[_currentKeyboardEventTab]);
    });

    tabsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addWidget(tabsWidget);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QPushButton *WorkflowGraphic::Tabs(const std::string& label, std::function<void()> onClick, Tab tab) {
    auto *btn = new QPushButton(QString::fromStdString(label));
    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btn->setMinimumWidth(0);
    btn->setFlat(true);
    btn->setFocusPolicy(Qt::NoFocus);

    if (tab == _tab) {
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

    QObject::connect(btn, &QPushButton::clicked, btn, [onClick]() { onClick(); });
    return btn;
}
