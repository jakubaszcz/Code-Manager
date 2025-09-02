#include "../../../../includes/graphic/WorkflowGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawCommandTab(QWidget *body) {
    auto *layout = qobject_cast<QVBoxLayout*>(body->layout());
    QLabel *label = new QLabel("Application");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("QLabel { color: white; font-size: 16px; font-weight: bold; }");
    layout->addWidget(label);
    layout->addStretch();
}