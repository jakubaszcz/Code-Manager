#include "../../includes/graphic/HeaderGraphic.hpp"
#include <QShortcut>

HeaderGraphic::HeaderGraphic(std::shared_ptr<Application> application) : IGraphic<QVBoxLayout *>(application) {
}

void HeaderGraphic::Draw(QVBoxLayout *layout) {


    // Create Header Widget
    QWidget *header = new QWidget();
    header->setFixedHeight(40);

    // Create layout for the header widget
    QHBoxLayout *mainHeaderLayout = new QHBoxLayout(header);
    mainHeaderLayout->setContentsMargins(0, 0, 0, 0);
    mainHeaderLayout->setSpacing(0);

    // Button to change working directory
    QPushButton *headerBox = new QPushButton();
    headerBox->setFixedHeight(40);
    headerBox->setFlat(true);
    headerBox->setStyleSheet(R"(
            QPushButton {
                border: none;
            }
        )");

    // Tab -> ouvrir le sélecteur de dossier (équivalent à un clic)
    auto *tab = new QShortcut(QKeySequence(Qt::Key_Tab), header);
    QObject::connect(tab, &QShortcut::activated, header, [headerBox]() {
        if (headerBox->isEnabled() && headerBox->isVisible())
            headerBox->click();
    });


    // Horizontal layout for the labels inside the button
    QHBoxLayout *headerLayout = new QHBoxLayout(headerBox);
    headerLayout->setContentsMargins(10, 5, 10, 5);
    headerLayout->setSpacing(10);

    QLabel *labelIcon = new QLabel("📁");

    std::string wpath = _application->GetData()->GetConfigMap()["wpath"];
    QString text =
            wpath.empty() ? "No working directory provided. Click here to choose one." : QString::fromStdString(wpath);
    QLabel *labelDirectory = new QLabel(text);

    QObject::connect(headerBox, &QPushButton::clicked, [=]() {
        QString dir = QFileDialog::getExistingDirectory(header, "Pick a working directory.");
        if (!dir.isEmpty()) {
            _application->GetData()->ChangeConfig("wpath", dir.toStdString());
            labelDirectory->setText(dir);
        }
    });

    // Add stretch before to push content to center
    headerLayout->addStretch();
    headerLayout->addWidget(labelIcon);
    headerLayout->addWidget(labelDirectory);
    // Add stretch after to center the content
    headerLayout->addStretch();

    // Add the button to the main header layout
    mainHeaderLayout->addWidget(headerBox);

    // Add the complete header to main layout
    layout->addWidget(header);
}
