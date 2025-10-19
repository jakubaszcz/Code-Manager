#include "../../includes/software/Software.hpp"
#include <QShortcut>
#include <memory>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


Software::Software(std::shared_ptr<Application> app) : _application(std::move(app)) {
    _workflowGraphic = std::make_unique<WorkflowGraphic>(_application);
    _menuGraphic = std::make_unique<MenuGraphic>(_application);
    _settingsGraphic = std::make_unique<SettingsGraphic>(_application);
    _menuGraphic->SetRedraw([this]() { this->Draw(); });
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void Software::Draw() {
    std::cout << "Draw" << std::endl;
    static QWidget *window = nullptr;

    std::cout << "1" << std::endl;
    if (!window) {
        window = new QWidget();
        window->resize(_windowSize.width, _windowSize.height);
        window->setWindowFlags(Qt::FramelessWindowHint);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QVBoxLayout *mLayout = new QVBoxLayout(window);
        mLayout->setContentsMargins(0, 0, 0, 0);
        mLayout->setSpacing(0);
    }

    std::cout << "2" << std::endl;

    _softwareShortcutEscape = new QShortcut(QKeySequence(Qt::Key_Escape), window);
    QObject::connect(_softwareShortcutEscape, &QShortcut::activated, window, [this]() {
        switch (_application->GetCurrentWindow()) {
            std::cout << "Closing" << std::endl;
            case Windows::Workflow:
                _application->SetCurrentWindow(Windows::Menu);
                Draw();
                break;
            default:
                break;
        }
    });

    std::cout << "3" << std::endl;


    auto *mLayout = qobject_cast<QVBoxLayout *>(window->layout());
    QLayoutItem *child;
    while ((child = mLayout->takeAt(0)) != nullptr) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    std::cout << "4" << std::endl;


    auto *bWidget = new QWidget();
    auto *bWidgetLayout = new QVBoxLayout(bWidget);

    std::cout << "5" << std::endl;


    switch (_application->GetCurrentWindow()) {
        case Windows::Menu:
            _menuGraphic->Draw(bWidgetLayout);
            break;
        case Windows::Setting:
            _settingsGraphic->Draw(bWidgetLayout);
            break;
        case Windows::Workflow: {
            _workflowGraphic->Draw(bWidgetLayout);
            break;
        }
        default:
            break;
    }

    std::cout << "6" << std::endl;

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
