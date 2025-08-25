#include "../../includes/graphic/BodyGraphic.hpp"
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>


BodyGraphic::BodyGraphic(std::shared_ptr<Application> application) : IGraphic<QVBoxLayout *>(application) {
}

void BodyGraphic::Draw(QVBoxLayout *layout) {

    QWidget *body = new QWidget();
    QVBoxLayout *bodyLayout = new QVBoxLayout(body);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    _rows.clear();
    _currentRow = 0;

    _rows.push_back(fileManagerRow());
    _rows.push_back(terminalRow());
    _rows.push_back(commandRow());

    for (auto w: _rows)
        bodyLayout->addWidget(w);
    bodyLayout->addStretch();

    layout->addWidget(body);

    auto *up = new QShortcut(QKeySequence(Qt::Key_Up), body);
    auto *down = new QShortcut(QKeySequence(Qt::Key_Down), body);

    QObject::connect(up, &QShortcut::activated, body, [this]() {
        if (_rows.empty())
            return;
        if (_currentRow == 0)
            _currentRow = static_cast<int>(_rows.size());
        _currentRow = (_currentRow - 1 + static_cast<int>(_rows.size())) % static_cast<int>(_rows.size());
        _rows[_currentRow]->setFocus(Qt::ShortcutFocusReason);
    });

    QObject::connect(down, &QShortcut::activated, body, [this]() {
        if (_rows.empty())
            return;
        if (_currentRow == 0)
            _currentRow = 0;
        else
            _currentRow = (_currentRow + 1) % static_cast<int>(_rows.size());
        _rows[_currentRow]->setFocus(Qt::ShortcutFocusReason);
    });

    if (!_rows.empty())
        _rows[_currentRow]->setFocus(Qt::OtherFocusReason);

    auto triggerCurrentRow = [this]() {
        if (_rows.empty())
            return;
        QWidget *row = _rows[_currentRow];

        if (auto *btn = qobject_cast<QPushButton *>(row->focusProxy())) {
            if (btn->isEnabled() && btn->isVisible()) {
                btn->click();
                return;
            }
        }

        if (auto *btn2 = row->findChild<QPushButton *>()) {
            if (btn2->isEnabled() && btn2->isVisible()) {
                btn2->click();
            }
        }
    };

    auto *enter1 = new QShortcut(QKeySequence(Qt::Key_Return), body);
    auto *enter2 = new QShortcut(QKeySequence(Qt::Key_Enter), body);
    QObject::connect(enter1, &QShortcut::activated, body, triggerCurrentRow);
    QObject::connect(enter2, &QShortcut::activated, body, triggerCurrentRow);
}


QWidget *BodyGraphic::fileManagerRow() {
    QWidget *row = new QWidget;
    QHBoxLayout *h = new QHBoxLayout(row);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    auto *btn = new QPushButton("Open File Manager", row);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    row->setFocusPolicy(Qt::StrongFocus);

    btn->setStyleSheet("QPushButton {"
                       "  border: none;"
                       "  border-radius: 0px;"
                       "  padding: 8px 12px;"
                       "  background-color: #1e1e1e;"
                       "  color: white;"
                       "  text-align: left;"
                       "}"
                       "QPushButton:hover { background-color: #343434; }"
                       "QPushButton:pressed { background-color: #1b1b1b; }"
                       "QPushButton:focus { background-color: #343434;"
                       " border: none;"
                       " border-radius: 0px;"
                       " outline: none;"
                       "}");

    QObject::connect(btn, &QPushButton::clicked, [this]() {
        const QString startDir = QString::fromStdString(_application->GetData()->GetConfigMap()["wpath"]);
        QDesktopServices::openUrl(QUrl::fromLocalFile(startDir));
    });

    h->addWidget(btn, /*stretch*/ 1);

    row->setFocusProxy(btn);

    return row;
}

QWidget *BodyGraphic::terminalRow() {
    QWidget *row = new QWidget;
    QHBoxLayout *h = new QHBoxLayout(row);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    auto *btn = new QPushButton("Open Terminal", row);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    row->setFocusPolicy(Qt::StrongFocus);

    btn->setStyleSheet("QPushButton {"
                       "  border: none;"
                       "  border-radius: 0px;"
                       "  padding: 8px 12px;"
                       "  background-color: #1e1e1e;"
                       "  color: white;"
                       "  text-align: left;"
                       "}"
                       "QPushButton:hover { background-color: #343434; }"
                       "QPushButton:pressed { background-color: #1b1b1b; }"
                       "QPushButton:focus { background-color: #343434;"
                       " border: none;"
                       " border-radius: 0px;"
                       " outline: none;"
                       "}");

    QObject::connect(btn, &QPushButton::clicked, [this]() {
        const QString wpath = QString::fromStdString(_application->GetData()->GetConfigMap()["wpath"]);
        if (wpath.isEmpty())
            return;

#ifdef Q_OS_WIN
        QProcess::startDetached("cmd.exe", QStringList(), wpath);
#elif defined(Q_OS_MAC)
        QProcess::startDetached("open", QStringList() << "-a" << "Terminal" << wpath);
#else
        bool started = false;
        started = QProcess::startDetached("x-terminal-emulator",
                                          QStringList() << "--working-directory" << wpath);
        // 2) GNOME Terminal
        if (!started) {
            started = QProcess::startDetached("gnome-terminal",
                                              QStringList() << ("--working-directory=" + wpath));
        }
        if (!started) {
            started = QProcess::startDetached("konsole",
                                              QStringList() << "--workdir" << wpath);
        }
        if (!started) {
            const QString cmd = QString("cd \"%1\"; exec $SHELL -i").arg(wpath);
            started = QProcess::startDetached("xterm",
                                              QStringList() << "-e" << "bash" << "-lc" << cmd);
        }
        Q_UNUSED(started);
#endif
    });

    h->addWidget(btn, /*stretch*/ 1);

    row->setFocusProxy(btn);

    return row;
}


QWidget *BodyGraphic::commandRow() {
    QWidget *row = new QWidget;
    row->setFocusPolicy(Qt::StrongFocus);
    row->setStyleSheet("QWidget {"
                       "  border: none;"
                       "  border-radius: 0px;"
                       "  padding: 8px 12px;"
                       "  background-color: #1e1e1e;"
                       "  color: white;"
                       "}"
                       "QWidget:hover { background-color: #343434; }"
                       "QWidget:pressed { background-color: #1b1b1b; }"
                       "QWidget:focus { background-color: #343434;"
                       " border: none;"
                       " border-radius: 0px;"
                       " outline: none;"
                       "}");

    auto *h = new QHBoxLayout(row);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(8);

    auto *label = new QPushButton("Execute Command", row);
    label->setCursor(Qt::PointingHandCursor);
    label->setFlat(true);
    label->setStyleSheet("QPushButton {"
                         "  background-color: transparent;"
                         "  border: none;"
                         "  color: white;"
                         "  text-align: left;"
                         "}"
                         "QPushButton:hover { color: #e0e0e0; }"
                         "QPushButton:focus { background-color: #343434;"
                         " border: none;"
                         " border-radius: 0px;"
                         " outline: none;"
                         "}");

    // Champ input à droite
    auto *input = new QLineEdit(row);
    std::string customCmd = _application->GetData()->GetConfigMap()["customCommand"];
    input->setPlaceholderText(QString::fromStdString(customCmd.empty() ? "Enter custom command." : customCmd));
    input->setClearButtonEnabled(true);
    input->setFixedWidth(240);
    input->setStyleSheet("QLineEdit {"
                         "  border-radius: 0;"
                         "  padding: 4px 8px;"
                         "  background-color: transparent;"
                         "  color: white;"
                         "}");

    h->addWidget(label, 1);
    h->addWidget(input, 0, Qt::AlignRight);

    QObject::connect(input, &QLineEdit::returnPressed, row, [this, input]() {
        const QString cmd = input->text().trimmed();
        if (cmd.isEmpty())
            return;
        _application->GetData()->ChangeConfig("customCommand", input->text().toStdString());
        input->setPlaceholderText(cmd);
        input->clear();
    });

    QObject::connect(label, &QPushButton::clicked, row, [this, input]() {
        QString cmd = input->text().trimmed();
        if (cmd.isEmpty()) {
            const std::string saved = _application->GetData()->GetConfigMap()["customCommand"];
            cmd = QString::fromStdString(saved);
        }
        if (cmd.isEmpty())
            return;

        const QString oldDir = QDir::currentPath();
        const QString newDir = QString::fromStdString(_application->GetData()->GetConfigMap()["wpath"]);

        QDir::setCurrent(newDir);
#ifdef Q_OS_WIN
        int exitCode = QProcess::execute("cmd", {"/C", cmd});
#else
        int exitCode = QProcess::execute("/bin/sh", {"-c", cmd});
#endif
        QDir::setCurrent(oldDir);

        Q_UNUSED(exitCode);
    });

    row->setFocusProxy(label);

    return row;
}
