#include "../../../includes/graphic/utils/popup/NamePopup.hpp"
#include "../../../includes/application/Application.hpp"
#include <QLineEdit>
#include <QVBoxLayout>
#include <string>
#include <unistd.h>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


NamePopup::NamePopup(QWidget *parent, const std::shared_ptr<Application>& application) : IPopup(parent, application) {
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void NamePopup::Draw() {
    auto *layout = new QVBoxLayout(this);

    auto *input = new QLineEdit(this);
    input->setPlaceholderText("Set command...");
    input->setClearButtonEnabled(true);
    input->setFixedWidth(240);

    _application->StyleSheetSimple(input, "command");

    QObject::connect(input, &QLineEdit::returnPressed, this, [this, input]() {
        _name = input->text().toStdString();
        accept(); // Au lieu de close()
    });

    layout->addWidget(input);
    input->setFocus(); // Mettre le focus sur l'input
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void NamePopup::DrawStyling() {
    setStyleSheet("QDialog { background: #222; border-radius: 8px; }"
                  "* { font-size: 14px; color: white; }");
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


const std::string& NamePopup::GetName() const {
    return _name;
}
