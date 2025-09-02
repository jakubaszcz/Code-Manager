#include "../../includes/graphic/WorkflowGraphic.hpp"
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


WorkflowGraphic::WorkflowGraphic(std::shared_ptr<Application> application) : IGraphic<QVBoxLayout *>(application) {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::Draw(QVBoxLayout *layout) {
    // Draw Header
    DrawHeader(layout);

    // Draw Body
    DrawBody(layout);
}

Tab WorkflowGraphic::GetTab() {
    return _tab;
}

void WorkflowGraphic::SetTab(Tab tab) {
    if (_tab == tab)
        return;
    _tab = tab;

    RebuildBody();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::RebuildBody() {
    if (_layout == nullptr)
        return;

    QWidget *body = _layout->parentWidget();
    if (!body)
        return;

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

    delete _layout;
    _layout = new QVBoxLayout(body);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(0);

    DrawTabs(body);

    auto it = _tabsWindow.find(_tab);
    if (it != _tabsWindow.end()) {
        it->second();
    }
}
