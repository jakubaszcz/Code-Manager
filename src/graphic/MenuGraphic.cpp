#include "../../includes/graphic/MenuGraphic.hpp"
#include <QPushButton>
#include <QShortcut>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


MenuGraphic::MenuGraphic(std::shared_ptr<Application> application) : IGraphic<QVBoxLayout *>(std::move(application)) {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void MenuGraphic::Draw(QVBoxLayout *layout) {

    _buttons.clear();
    _currentButton = 0;

    if (QPushButton *pb = WorkflowButton())
        _buttons.push_back(pb);
    if (QPushButton *pb = SettingButton())
        _buttons.push_back(pb);
    if (QPushButton *pb = ExitButton())
        _buttons.push_back(pb);

    QVBoxLayout *centerColumn = new QVBoxLayout();
    centerColumn->setAlignment(Qt::AlignCenter);

    QHBoxLayout *row = new QHBoxLayout();
    row->setSpacing(48);
    row->setAlignment(Qt::AlignCenter);

    for (auto *button: _buttons) {
        row->addWidget(button);
    }
    centerColumn->addLayout(row);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch();
    layout->addLayout(centerColumn);
    layout->addStretch();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QPushButton *MenuGraphic::WorkflowButton() {
    QPushButton *workflowButton = new QPushButton("🖥️");

    workflowButton->setCursor(Qt::PointingHandCursor);
    workflowButton->setFocusPolicy(Qt::StrongFocus);
    workflowButton->setFixedSize(96, 96); // taille via API Qt

    workflowButton->setStyleSheet("QPushButton {"
                                  "  border: none;"
                                  "  font-size:36px;"
                                  "  color:#ffffff;"
                                  "  background:#2E2E2E;"
                                  "}"
                                  "QPushButton:hover { background:#4a4a4a; }"
                                  "QPushButton:pressed { background:#2f2f2f; }"
                                  "QPushButton:focus {"
                                  "  background:#3a3a3a;"
                                  "  outline: none;"
                                  "}");

    // Event
    {

        QObject::connect(workflowButton, &QPushButton::clicked, workflowButton, [this]() {
            _application->SetCurrentWindow(Windows::Workflow);
            if (_redraw)
                _redraw();
        });
    }
    return workflowButton;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QPushButton *MenuGraphic::SettingButton() {
    QPushButton *settingButton = new QPushButton("🛠️");

    settingButton->setCursor(Qt::PointingHandCursor);
    settingButton->setFocusPolicy(Qt::StrongFocus);
    settingButton->setFixedSize(96, 96);

    settingButton->setStyleSheet("QPushButton {"
                                 "  border: none;"
                                 "  font-size:36px;"
                                 "  color:#ffffff;"
                                 "  background:#2E2E2E;"
                                 "}"
                                 "QPushButton:hover { background:#4a4a4a; }"
                                 "QPushButton:pressed { background:#2f2f2f; }"
                                 "QPushButton:focus {"
                                 "  background:#3a3a3a;"
                                 "  outline: none;"
                                 "}");

    // Event
    {

        QObject::connect(settingButton, &QPushButton::clicked, settingButton, [this]() {
            _application->SetCurrentWindow(Windows::Setting);
            if (_redraw)
                _redraw();
        });
    }
    return settingButton;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QPushButton *MenuGraphic::ExitButton() {
    QPushButton *exitButton = new QPushButton("🚪");

    exitButton->setCursor(Qt::PointingHandCursor);
    exitButton->setFocusPolicy(Qt::StrongFocus);
    exitButton->setFixedSize(96, 96);

    exitButton->setStyleSheet("QPushButton {"
                              "  border: none;"
                              "  font-size:36px;"
                              "  color:#ffffff;"
                              "  background:#2E2E2E;"
                              "}"
                              "QPushButton:hover { background:#4a4a4a; }"
                              "QPushButton:pressed { background:#2f2f2f; }"
                              "QPushButton:focus {"
                              "  background:#3a3a3a;"
                              "  outline: none;"
                              "}");

    // Event
    {

        QObject::connect(exitButton, &QPushButton::clicked, exitButton, [this]() { QApplication::quit(); });
    }
    return exitButton;
}
