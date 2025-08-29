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
