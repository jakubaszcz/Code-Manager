#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPoint>
#include <QPushButton>
#include <QVBoxLayout>

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class IPopup : public QDialog {
public:
    ~IPopup() = default;

    void OpenNear(QWidget *anchor, const QPoint& offset = QPoint(0, 0), bool modal = true);

protected:
    explicit IPopup(QWidget *parent = nullptr);

    virtual void Draw() = 0;
    virtual void DrawStyling();

    void MakeFrameless();
    void MakeAutoClose();
};
