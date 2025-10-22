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

    // Create the window
    static QWidget *window = nullptr;

    // Verification if the window was created
    if (!window) {
        window = new QWidget();
        window->resize(_windowSize.width, _windowSize.height);
        window->setWindowFlags(Qt::FramelessWindowHint);
        window->setAttribute(Qt::WA_DeleteOnClose);

        QVBoxLayout *mLayout = new QVBoxLayout(window);
        mLayout->setContentsMargins(0, 0, 0, 0);
        mLayout->setSpacing(0);
    }


    window->setObjectName("window-widget");
    window->setProperty("window-widget", "true");

    window->setAutoFillBackground(true);
    window->setAttribute(Qt::WA_StyledBackground, true);

    window->style()->unpolish(window);
    window->style()->polish(window);
    window->update();


    // Create the escape key callback
    {
        if (!_softwareShortcutEscape) {

            // Create key callback
            {
                _softwareShortcutEscape = new QShortcut(QKeySequence(Qt::Key_Escape), window);
            }

            // Assign event to the key callback
            {
                QObject::connect(_softwareShortcutEscape, &QShortcut::activated, window, [this]() {
                   switch (_application->GetCurrentWindow()) {
                       case Windows::Workflow:
                       case Windows::Setting:
                           _application->SetCurrentWindow(Windows::Menu);
                           Draw();
                           break;
                       default:
                           break;
                   }
               });
            }
        }
    }


    // Create window layout with childs
    auto *layout = qobject_cast<QVBoxLayout *>(window->layout());
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    // Create widget and layout's widget
    auto *widget = new QWidget();
    auto *widgetLayout = new QVBoxLayout(widget);

    // Drawing the window by the current enum tag
    switch (_application->GetCurrentWindow()) {
        case Windows::Menu:
            _menuGraphic->Draw(widgetLayout);
            break;
        case Windows::Setting:
            _settingsGraphic->Draw(widgetLayout);
            break;
        case Windows::Workflow: {
            _workflowGraphic->Draw(widgetLayout);
            break;
        }
        default:
            break;
    }

    // Assign
    layout->addWidget(widget);
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
