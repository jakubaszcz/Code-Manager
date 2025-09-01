#include "../../includes/software/Software.hpp"
#include <memory>
#include <QShortcut>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


Software::Software(std::shared_ptr<Application> app) : _application(std::move(app)) {
    _workflowGraphic = std::make_unique<WorkflowGraphic>(_application);
    _menuGraphic = std::make_unique<MenuGraphic>(_application);
    _menuGraphic->SetRedraw([this]() { this->Draw(); });
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Software::Draw() {
    static QWidget *window = nullptr;

    if (!window) {
        window = new QWidget();
        window->resize(_windowSize.width, _windowSize.height);
        window->setWindowFlags(Qt::FramelessWindowHint);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->setStyleSheet("background-color: #1e1e1e;");

        auto *escape = new QShortcut(QKeySequence(Qt::Key_Escape), window);
        QObject::connect(escape, &QShortcut::activated, window, [this]() {
            switch (_application->GetCurrentWindow()) {
                case Windows::Workflow:
                    _application->SetCurrentWindow(Windows::Menu);
                    Draw();
                    break;
                default: break;
            }
        });

        QVBoxLayout *mLayout = new QVBoxLayout(window);
        mLayout->setContentsMargins(0, 0, 0, 0);
        mLayout->setSpacing(0);
    }

    auto *mLayout = qobject_cast<QVBoxLayout*>(window->layout());
    QLayoutItem *child;
    while ((child = mLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    QWidget *bWidget = new QWidget();
    QVBoxLayout *bWidgetLayout = new QVBoxLayout(bWidget);

    switch (_application->GetCurrentWindow()) {
        case Windows::Menu: {
            _menuGraphic->Draw(bWidgetLayout);
            break;
        }
        case Windows::Setting: break;
        case Windows::Workflow: {
            _workflowGraphic->Draw(bWidgetLayout);
            break;
        }
        default: break;
    }
    mLayout->addWidget(bWidget);
    window->show();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Software::SetWindowSize(WindowSize size) {
    _windowSize = size;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


Software::WindowSize Software::GetWindowSize() const {
    return _windowSize;
}