#pragma once


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


#include "IPopup.hpp"


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
        void appendRaw(const QByteArray& data);
        QProcess* proc_;
        QPlainTextEdit* view_;
        QLineEdit* input_;
        QPushButton* sendBtn_;
};