#include "../../includes/graphic/WorkflowGraphic.hpp"
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QUrl>
#include <QVBoxLayout>
#include <iostream>
#include <QWidget>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


WorkflowGraphic::WorkflowGraphic(std::shared_ptr<Application> application) : IGraphic<QVBoxLayout *>(application) {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::Draw(QVBoxLayout *layout) {

    std::cout << "Workflow" << std::endl;

    // Draw Header
    DrawHeader(layout);

    std::cout << "Workflow 1" << std::endl;


    // Draw Body
    DrawBody(layout);

    std::cout << "Workflow 2" << std::endl;

}

Tab WorkflowGraphic::GetTab() {
    return _tab;
}

void WorkflowGraphic::SetTab(Tab tab) {
    if (_tab == tab)
        return;
    _tab = tab;

    for (size_t i = 0; i < _keyboardEventTab.size(); ++i) {
        if (_keyboardEventTab[i] == _tab) {
            _currentKeyboardEventTab = i;
            break;
        }
    }

    RebuildBody();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::RebuildBody() {
    if (_layout == nullptr)
        return;

    QWidget *body = _layout->parentWidget();
    if (!body) {
        _layout = nullptr;  // Clear the invalid layout pointer
        return;
    }

    // Additional safety check before operating on layout
    if (_layout == nullptr) {
        return;
    }
    
    while (_layout->count() > 0) {
        QLayoutItem *item = _layout->takeAt(0);
        if (!item)
            continue;

        if (QWidget *w = item->widget()) {
            w->deleteLater();
        }
        if (QLayout *childLayout = item->layout()) {
            while (childLayout->count() > 0) {
                QLayoutItem *subItem = childLayout->takeAt(0);
                if (QWidget *sw = subItem->widget()) {
                    sw->deleteLater();
                }
                delete subItem;
            }
            delete childLayout;
        }
        delete item;
    }

    for (QWidget *child: body->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly)) {
        child->deleteLater();
    }

    // Safe deletion of layout
    if (_layout) {
        delete _layout;
    }
    _layout = new QVBoxLayout(body);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(0);

    DrawTabs(body);

    auto it = _tabsWindow.find(_tab);
    if (it != _tabsWindow.end()) {
        it->second();
    }
}
