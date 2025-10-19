#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QShortcut>
#include <QWidget>
#include <functional>
#include <memory>
#include <vector>
#include "../application/Application.hpp"
#include "IGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class MenuGraphic : public IGraphic<QVBoxLayout *> {
public:
    MenuGraphic(std::shared_ptr<Application> application);

    void Draw(QVBoxLayout *) override;

    void SetRedraw(std::function<void()> redraw) { _redraw = std::move(redraw); }

private:
    QPushButton *WorkflowButton(int);
    QPushButton *SettingButton(int);
    QPushButton *ExitButton(int);
    void UPDT_MenuButton(int, int);
    std::function<void()> _redraw;
    std::vector<QPushButton*> _buttons;
    int _currentButton;
    QShortcut *_menuGraphicShortcutLeft = nullptr;
    QShortcut *_menuGraphicShortcutRight = nullptr;
    QShortcut *_menuGraphicShortcutEnter = nullptr;
};