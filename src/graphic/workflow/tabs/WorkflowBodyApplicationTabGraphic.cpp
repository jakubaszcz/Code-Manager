#include "../../../../includes/graphic/WorkflowGraphic.hpp"
#include <string>
#include <QScrollArea>

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawApplicationTab(QWidget *body) {
    auto *layout = qobject_cast<QVBoxLayout *>(body->layout());
    if (!layout) {
        layout = new QVBoxLayout(body);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
    }

    QWidget *appWidget = new QWidget(body);
    AddApplication(appWidget);
    layout->addWidget(appWidget);

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

    // const auto& cfg = _application->GetData()->GetConfigMap();
    // for (const auto& [key, value]: cfg) {
    //     if (key.rfind(_application->GetData()->GetCustomConfigMap()[Data::ConfigType::Command], 0) == 0) {
    //         if (QWidget *w = Command(key)) {
    //             boxLayout->addWidget(w, 0, Qt::AlignTop);
    //         }
    //     }
    // }

    boxLayout->addStretch();

    scroll->setWidget(bbox);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::AddApplication(QWidget *container) {

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
        "QPushButton:focus { border: 1px solid #5a5a5a; }"
    );

    h->addWidget(btnPlus, 0, Qt::AlignRight);

    auto addAndRefresh = [this]() {

        _application->GetData()->AddApplication();
        RebuildBody();
    };

    QObject::connect(btnPlus, &QPushButton::clicked, row, addAndRefresh);


    v->addWidget(row, 0, Qt::AlignTop);
}
