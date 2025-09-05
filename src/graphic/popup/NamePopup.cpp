#include "../../../includes/graphic/utils/popup/NamePopup.hpp"
#include <QLineEdit>
#include <QVBoxLayout>
#include <string>
#include <unistd.h>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


NamePopup::NamePopup(QWidget *parent) : IPopup(parent) {}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void NamePopup::Draw() {
    auto *layout = new QVBoxLayout(this);


    auto *input = new QLineEdit(this);
    input->setPlaceholderText("Set command...");
    input->setClearButtonEnabled(true);
    input->setFixedWidth(240);
    input->setStyleSheet("QLineEdit { background: #1e1e1e; color: white; border: 1px solid #3a3a3a; padding: 6px 8px; }"
                         "QLineEdit:focus { border: 1px solid #5a5a5a; }");

    {
    QObject::connect(input, &QLineEdit::returnPressed, this, [this, input]() {
            _name = input->text().toStdString();
            close();
        });
    }

    layout->addWidget(input);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void NamePopup::DrawStyling() {
    // Optional: specialize styling for NamePopup, or leave empty to use base defaults
    setStyleSheet(
        "QDialog { background: #222; border-radius: 8px; }"
        "* { font-size: 14px; color: white; }"
    );
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


const std::string& NamePopup::GetName() const {
return _name;
}