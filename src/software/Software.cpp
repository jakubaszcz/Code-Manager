#include "../../includes/software/Software.hpp"

Software::Software(std::shared_ptr<Data> data) : _data(data) {};

void Software::Main() {
    // Window
    QWidget *window = new QWidget();

    window->resize(1200, 600);
    window->setWindowFlags(Qt::FramelessWindowHint);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setStyleSheet(R"(
        background-color: #332845;
    )");

    // The way the winodw will be, in Vertical
    QVBoxLayout *mainLayout = new QVBoxLayout(window);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    DrawHeader(window, mainLayout);
    DrawPanel(window, mainLayout);
    window->setLayout(mainLayout);
    window->show();
};

void Software::DrawHeader(QWidget *window, QVBoxLayout *mainLayout) {

    QWidget *header = new QWidget();
    header->setFixedHeight(40);
    header->setStyleSheet("background-color: #444;");

    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(10, 5, 10, 5);
    headerLayout->setSpacing(10);

    QPushButton *button = new QPushButton("📁");
    button->setFixedSize(20, 20);
    button->setStyleSheet(R"(
        QPushButton {
            outline: none;
            border: none;
            background-color: #444;
            color: white;
        }
        QPushButton:focus {
            outline: none;
        }
    )");
    std::string wpath = _data->GetConfigMap()["wpath"];
    QString text = wpath.empty() ? "No working directory provided." : QString::fromStdString(wpath);
    QLabel *label = new QLabel(text);

    headerLayout->addWidget(button);
    headerLayout->addWidget(label);
    headerLayout->addStretch();
    mainLayout->addWidget(header);


    QObject::connect(button, &QPushButton::clicked, window, [=]() {
        QString dir = QFileDialog::getExistingDirectory(window, "Choisir un dossier");

        if (!dir.isEmpty()) {
            _data->ChangeConfig("wpath", dir.toStdString());
            label->setText(dir);
        }
    });
}

void Software::DrawPanel(QWidget *window, QVBoxLayout *mainLayout) {
    QWidget *content = new QWidget();
    content->setStyleSheet("background-color: #2c2c2c;");
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addWidget(content);
}
