#include "../../includes/graphic/MenuGraphic.hpp"
#include <QPushButton>
#include <QShortcut>

MenuGraphic::MenuGraphic(std::shared_ptr<Application> application) : IGraphic<QVBoxLayout *>(std::move(application)) {
}

void MenuGraphic::Draw(QVBoxLayout *layout) {
    QPushButton *codeManagerButton = new QPushButton("🖥️");
    QPushButton *settingsButton = new QPushButton("⚙️");
    QPushButton *exitButton = new QPushButton("⏻");

    // Boutons carrés + style
    const int side = 120;
    auto styleSquare = QString("QPushButton {"
                               "  border: none;"
                               "  width:%1px; height:%1px;"
                               "  font-size:36px;"
                               "  color:#ffffff;"
                               "  background:#3a3a3a;"
                               "  border:1px solid #505050;"
                               "}"
                               "QPushButton:hover { background:#4a4a4a; }"
                               "QPushButton:pressed { background:#2f2f2f; }")
                               .arg(side);

    for (auto *btn: {codeManagerButton, settingsButton, exitButton}) {
        btn->setFixedSize(side, side);
        btn->setStyleSheet(styleSquare);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFocusPolicy(Qt::NoFocus);
    }

    // Connexions
    QObject::connect(codeManagerButton, &QPushButton::clicked, codeManagerButton, [this]() {
        _application->SetCurrentWindow(Windows::CodeManager);
        if (_redraw)
            _redraw();
    });

    QObject::connect(settingsButton, &QPushButton::clicked, settingsButton, [this]() {
        _application->SetCurrentWindow(Windows::Setting);
        if (_redraw)
            _redraw();
    });

    QObject::connect(exitButton, &QPushButton::clicked, exitButton, [this]() { QApplication::quit(); });

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *centerColumn = new QVBoxLayout();
    centerColumn->setAlignment(Qt::AlignCenter);

    QHBoxLayout *row = new QHBoxLayout();
    row->setSpacing(48);
    row->setAlignment(Qt::AlignCenter);
    row->addWidget(codeManagerButton);
    row->addWidget(settingsButton);
    row->addWidget(exitButton);

    centerColumn->addLayout(row);

    layout->addStretch();
    layout->addLayout(centerColumn);
    layout->addStretch();
}
