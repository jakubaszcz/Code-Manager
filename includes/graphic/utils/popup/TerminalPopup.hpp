#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include "IPopup.hpp"
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


class TerminalPopup : public IPopup {
    public:
        explicit TerminalPopup(QWidget *parent = nullptr);

        void Draw();
        void StartCommand(const QString& program, const QStringList& args = {});
    private :
        void OnReadyStdOut();
        void OnReadyStdErr();
        void OnFinished(int code, QProcess::ExitStatus st);
        void SendInput();
        void AppendRaw(const QByteArray& data);
        QProcess* _proc;
        QPlainTextEdit* _view;
        QLineEdit* _input;
        QVBoxLayout* _layout;
};
