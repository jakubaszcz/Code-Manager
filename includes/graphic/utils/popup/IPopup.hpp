#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPoint>
#include <QProcess>
#include <QPushButton>
#include <QVBoxLayout>
#include <memory>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class Application;

class IPopup : public QDialog {
public:
    IPopup(QWidget *parent, std::shared_ptr<Application> application) : _application(application) {};

    virtual ~IPopup() = default;

    void OpenNear(QWidget *anchor, const QPoint& offset = QPoint(0, 0), bool modal = true);

protected:

    std::shared_ptr<Application> _application;
    explicit IPopup(QWidget *parent = nullptr);

    virtual void Draw() = 0;
    virtual void DrawStyling();

    void MakeFrameless();
    void MakeAutoClose();
};
