#include "../../../includes/graphic/utils/popup/IPopup.hpp"

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


IPopup::IPopup(QWidget *parent) : QDialog(parent) {
    setModal(true);
    MakeFrameless();
    MakeAutoClose();
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void IPopup::DrawStyling() {
    setStyleSheet("QDialog { background: white; border-radius: 8px; }"
                  "* { font-size: 14px; }");
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void IPopup::MakeFrameless() {
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void IPopup::MakeAutoClose() {
    setWindowFlags(windowFlags() | Qt::Popup);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void IPopup::OpenNear(QWidget *anchor, const QPoint& offset, bool modal) {
    if (layout() == nullptr) {
        Draw();
        DrawStyling();
    }
    const QPoint global = anchor->mapToGlobal(QPoint(0, anchor->height())) + offset;
    move(global);
    if (modal)
        exec();
    else
        show();
}
