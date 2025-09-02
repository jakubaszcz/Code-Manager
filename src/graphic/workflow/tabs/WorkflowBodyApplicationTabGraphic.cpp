#include "../../../../includes/graphic/WorkflowGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawApplicationTab(QWidget *body) {
    auto *layout = qobject_cast<QVBoxLayout*>(body->layout());
    QLabel *label = new QLabel("Codfj");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("QLabel { color: white; font-size: 16px; font-weight: bold; }");
    layout->addWidget(label);
    layout->addStretch();
}