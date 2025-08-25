#include "../../includes/software/Software.hpp"
#include <memory>
#include <QShortcut>


Software::Software(std::shared_ptr<Application> app) : _application(std::move(app)) {
    _headerGraphic = std::make_unique<HeaderGraphic>(_application);
    _bodyGraphic = std::make_unique<BodyGraphic>(_application);
    _menuGraphic = std::make_unique<MenuGraphic>(_application);
    _menuGraphic->SetRedraw([this]() { this->Draw(); });

}

void Software::Draw() {
    static QWidget *window = nullptr; // ⚡ Garde la même fenêtre

    if (!window) {
        window = new QWidget();
        window->resize(1200, 800);
        window->setWindowFlags(Qt::FramelessWindowHint);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->setStyleSheet("background-color: #1e1e1e;");

        auto *escape = new QShortcut(QKeySequence(Qt::Key_Escape), window);
        QObject::connect(escape, &QShortcut::activated, window, [this]() {
            switch (_application->GetCurrentWindow()) {
                case Windows::CodeManager:
                    _application->SetCurrentWindow(Windows::Menu);
                    Draw();
                    break;
                default: break;
            }
        });

        QVBoxLayout *mainLayout = new QVBoxLayout(window);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        window->setLayout(mainLayout);
    }

    auto *mainLayout = qobject_cast<QVBoxLayout*>(window->layout());
    QLayoutItem *child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    switch (_application->GetCurrentWindow()) {
        case Windows::Menu: {
            QWidget *menu = new QWidget();
            QVBoxLayout *menuLayout = new QVBoxLayout(menu);

            _menuGraphic->Draw(menuLayout);

            mainLayout->addWidget(menu);
            break;
        }
        case Windows::Setting:
            mainLayout->addWidget(new QLabel("Settings scene"));
            break;
        case Windows::CodeManager: {
            QWidget *header = new QWidget();
            QVBoxLayout *headerLayout = new QVBoxLayout(header);
            _headerGraphic->Draw(headerLayout);

            QWidget *body = new QWidget();
            body->setStyleSheet("background-color: #2b2b2b;");
            QVBoxLayout *bodyLayout = new QVBoxLayout(body);
            _bodyGraphic->Draw(bodyLayout);

            headerLayout->addWidget(body);
            mainLayout->addWidget(header);
            break;
        }
        default: break;
    }

    window->show();
}

