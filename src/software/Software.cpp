#include "../../includes/software/Software.hpp"
#include <memory>


Software::Software(std::shared_ptr<Application> app) : _application(std::move(app)) {
    _headerGraphic = std::make_unique<HeaderGraphic>(_application);
    _bodyGraphic = std::make_unique<BodyGraphic>(_application);
}

void Software::Draw() {
    QWidget *window = new QWidget();

    window->resize(1200, 800);
    window->setWindowFlags(Qt::FramelessWindowHint);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setStyleSheet("background-color: #1e1e1e;");

    // Layout principal vertical
    QVBoxLayout *headerLayout = new QVBoxLayout(window);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(0);

    // Header
    _headerGraphic->Draw(headerLayout);

    // Corps principal
    QWidget *body = new QWidget();
    body->setStyleSheet("background-color: #2b2b2b;");

    QVBoxLayout *bodyLayout = new QVBoxLayout(body);
    bodyLayout->setContentsMargins(10, 10, 10, 10);
    bodyLayout->setSpacing(10);

    // Body
    _bodyGraphic->Draw(bodyLayout);

    headerLayout->addWidget(body);

    window->setLayout(headerLayout);
    window->setLayout(bodyLayout);
    window->show();
}
