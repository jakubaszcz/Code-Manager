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
    QWidget *header = new QWidget();
    header->setFixedHeight(40);

    QHBoxLayout *mainHeaderLayout = new QHBoxLayout(header);
    mainHeaderLayout->setContentsMargins(0, 0, 0, 0);
    mainHeaderLayout->setSpacing(0);

    QPushButton *headerBox = new QPushButton();
    headerBox->setFixedHeight(40);
    headerBox->setFlat(true);
    headerBox->setStyleSheet(R"(
            QPushButton {
                border: none;
            }
        )");

    auto *tab = new QShortcut(QKeySequence(Qt::Key_Tab), header);
    QObject::connect(tab, &QShortcut::activated, header, [headerBox]() {
        if (headerBox->isEnabled() && headerBox->isVisible())
            headerBox->click();
    });


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

    headerLayout->addStretch();
    headerLayout->addWidget(labelIcon);
    headerLayout->addWidget(labelDirectory);
    headerLayout->addStretch();

    mainHeaderLayout->addWidget(headerBox);

    layout->addWidget(header);
}
