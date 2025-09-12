#include "../../../includes/graphic/utils/popup/TerminalPopup.hpp"
#include <QVBoxLayout>
#include <string>
#include <unistd.h>


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


TerminalPopup::TerminalPopup(QWidget *parent) : IPopup(parent), _proc(new QProcess(this)) {
    _view = nullptr;
    _input = nullptr;
    _layout = nullptr;
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void TerminalPopup::Draw() {
    _layout = new QVBoxLayout(this);
    this->setFixedSize(1000, 1000);

    _view = new QPlainTextEdit(this);

    connect(_proc, &QProcess::readyReadStandardOutput, this, &TerminalPopup::OnReadyStdOut);
    connect(_proc, &QProcess::readyReadStandardError,  this, &TerminalPopup::OnReadyStdErr);
    connect(_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &TerminalPopup::OnFinished);

    _layout->addWidget(_view);
}


void TerminalPopup::StartCommand(const QString& program, const QStringList& args) {
    if (_view != nullptr) {
        _view->clear();
    }
#ifdef Q_OS_WIN
    QStringList full = args;
    _proc->start(program, full);
#else
    _proc->start("/bin/bash", {"-lc", program + (args.isEmpty() ? "" : " " + args.join(' '))});
#endif
}

void TerminalPopup::OnReadyStdOut() {
    QByteArray data = _proc->readAllStandardOutput();
    AppendRaw(data);
}

void TerminalPopup::OnReadyStdErr() {
    QByteArray data = _proc->readAllStandardError();
    AppendRaw(data);
}

void TerminalPopup::OnFinished(int code, QProcess::ExitStatus st) {
    Q_UNUSED(code);
    Q_UNUSED(st);
    if (_view != nullptr) {
        _view->appendPlainText("\n[Process finished]\n");
    }
}

void TerminalPopup::AppendRaw(const QByteArray& data) {
    if (_view != nullptr) {
        QString text = QString::fromUtf8(data);
        _view->insertPlainText(text);
        QTextCursor cursor = _view->textCursor();
        cursor.movePosition(QTextCursor::End);
        _view->setTextCursor(cursor);
    }
}

