#include "../../../../includes/graphic/WorkflowGraphic.hpp"


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawCommandTab(QWidget *body) {
    auto *layout = qobject_cast<QVBoxLayout *>(body->layout());
    if (!layout) {
        layout = new QVBoxLayout(body);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
    }

    QWidget *box = new QWidget(body);
    auto *boxLayout = new QVBoxLayout(box);
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->setSpacing(0);

    if (QWidget *w = CommandRow())
        boxLayout->addWidget(w);

    boxLayout->addStretch();

    layout->addWidget(box);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QWidget *WorkflowGraphic::CommandRow() {
    QWidget *row = new QWidget;
    row->setFocusPolicy(Qt::StrongFocus);
    row->setStyleSheet("QWidget { background-color: #2a2a2a; }");

    auto *h = new QHBoxLayout(row);
    h->setContentsMargins(8, 8, 8, 8);
    h->setSpacing(8);

    auto *btn = new QPushButton("Run", row);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btn->setMinimumWidth(0);
    btn->setFlat(true);
    btn->setStyleSheet("QPushButton { border: none; border-radius: 0px; padding: 8px 12px; background-color: #2a2a2a; "
                       "color: white; }"
                       "QPushButton:hover { background-color: #343434; }"
                       "QPushButton:pressed { background-color: #1b1b1b; }"
                       "QPushButton:focus { background-color: #343434; border: none; outline: none; }");

    auto *input = new QLineEdit(row);
    {
        const auto cfg = _application->GetData()->GetConfigMap();
        const auto it = cfg.find("customCommand");
        const std::string customCmd = (it != cfg.end()) ? it->second : std::string();
        input->setPlaceholderText(QString::fromStdString(customCmd.empty() ? "Enter custom command." : customCmd));
    }
    input->setClearButtonEnabled(true);
    input->setFixedWidth(240);
    input->setStyleSheet("QLineEdit { background: #1e1e1e; color: white; border: 1px solid #3a3a3a; padding: 6px 8px; }"
                         "QLineEdit:focus { border: 1px solid #5a5a5a; }");

    h->addWidget(btn, 1);
    h->addWidget(input, 0, Qt::AlignRight);

    QObject::connect(input, &QLineEdit::returnPressed, row, [this, input]() {
        const QString cmd = input->text().trimmed();
        if (cmd.isEmpty())
            return;
        _application->GetData()->ChangeConfig("customCommand", input->text().toStdString());
        input->setPlaceholderText(cmd);
        input->clear();
    });

    QObject::connect(btn, &QPushButton::clicked, row, [this, input]() {
        QString cmd = input->text().trimmed();
        if (cmd.isEmpty()) {
            const auto cfg = _application->GetData()->GetConfigMap();
            const auto it = cfg.find("customCommand");
            if (it != cfg.end())
                cmd = QString::fromStdString(it->second);
        }
        if (cmd.isEmpty())
            return;

        const QString oldDir = QDir::currentPath();
        const QString newDir = QString::fromStdString(_application->GetData()->GetConfigMap()["wpath"]);
        if (!newDir.isEmpty())
            QDir::setCurrent(newDir);

#ifdef Q_OS_WIN
        int exitCode = QProcess::execute("cmd", {"/C", cmd});
#else
        int exitCode = QProcess::execute("/bin/sh", {"-c", cmd});
#endif
        Q_UNUSED(exitCode);

        QDir::setCurrent(oldDir);
    });

    row->setFocusProxy(btn);
    return row;
}
