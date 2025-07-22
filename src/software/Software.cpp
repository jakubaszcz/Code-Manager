#include "../../includes/software/Software.hpp"

Software::Software(std::shared_ptr<Data> data) : _data(data) {
    _softwareState = SoftwareState::Panel;
};

void Software::Main() {
    switch (_softwareState) {
        case SoftwareState::Menu:
            MainMenu();
            break;
        case SoftwareState::Panel:
            MainPanel();
            break;
        default: break;
    }
};

void Software::MainMenu() {

}

void Software::MainPanel() {
    QWidget *window = new QWidget();
    QPushButton *button = new QPushButton("Set the working path", window);
    QPushButton *button2 = new QPushButton("Open terminal", window);
    button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
    button2->setGeometry(QRect(QPoint(20, 200), QSize(100, 20)));

    QObject::connect(button, &QPushButton::clicked, window, [=]() {
        QString dir = QFileDialog::getExistingDirectory(window, "Choisir un dossier");

        if (!dir.isEmpty()) {
            std::cout << dir.toStdString() << std::endl;
            _data->ChangeConfig("wpath", dir.toStdString());
            // Exemple : le stocker dans _data
        }
    });

    QObject::connect(button2, &QPushButton::clicked, window, [=]() {
QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(_data->GetConfigMap()["wpath"].c_str())));

    });

    window->resize(800, 600);
    window->setWindowFlags(Qt::FramelessWindowHint);
    window->setStyleSheet("background-color: red;");
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

