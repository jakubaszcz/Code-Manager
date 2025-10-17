#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include "../../../includes/graphic/WorkflowGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawHeader(QVBoxLayout *layout) {

    // Create widget
    auto *widget = new QWidget();
    widget->setFixedHeight(40);

    // Add widget to layout
    layout->addWidget(widget);

    // Add header layout
    auto *headerLayout = new QHBoxLayout(widget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(0);

    // Add button
    auto *button = new QPushButton();
    button->setFixedHeight(40);
    button->setFlat(true);

    // Styling the button
    {
        button->setFocusPolicy(Qt::NoFocus);

        button->setObjectName("header-graphic-button-open-directory");
        button->setProperty("header-graphic-button-open-directory", "true");

        button->setAutoFillBackground(true);
        button->setAttribute(Qt::WA_StyledBackground, true);

        button->style()->unpolish(button);
        button->style()->polish(button);
        button->update();
    }

    // Setup key events
    auto *shortcutTab = new QShortcut(QKeySequence(Qt::Key_Tab), widget);

    // Event
    {
        QObject::connect(shortcutTab, &QShortcut::activated, widget, [button]() {
       if (button->isEnabled() && button->isVisible())
           button->click();
       });
    }

    // Create header button layout
    auto *headerButtonLayout = new QHBoxLayout(button);
    headerButtonLayout->setContentsMargins(10, 5, 10, 5);
    headerButtonLayout->setSpacing(10);

    auto iconText = "📁";

    auto *iconLabel = new QLabel(iconText);

    std::string wpath = _application->GetData()->GetConfigMap()["wpath"];

    auto text = wpath.empty()
        ? "No working directory provided. Click here to choose one."
        : QString::fromStdString(wpath);

    auto *labelDirectory = new QLabel(text);

    // Event
    {
        QObject::connect(button, &QPushButton::clicked, iconLabel, [=]() {
           auto dir = QFileDialog::getExistingDirectory(widget, "Pick a working directory.");
           if (!dir.isEmpty()) {
               _application->GetData()->ChangeConfig("wpath", dir.toStdString());
               labelDirectory->setText(dir);
           }
       });
    }

    // Add widgets to layout
    headerButtonLayout->addStretch();
    headerButtonLayout->addWidget(iconLabel);
    headerButtonLayout->addWidget(labelDirectory);
    headerButtonLayout->addStretch();

    // add widget to header
    headerLayout->addWidget(button);
}
