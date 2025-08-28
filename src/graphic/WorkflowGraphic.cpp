#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include "../../includes/graphic/WorkflowGraphic.hpp"


WorkflowGraphic::WorkflowGraphic(std::shared_ptr<Application> application) : IGraphic<QVBoxLayout *>(application) {
}

void WorkflowGraphic::Draw(QVBoxLayout *layout) {
    DrawHeader(layout);
    DrawBody(layout);
}