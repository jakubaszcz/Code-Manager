#include "../../../../includes/graphic/WorkflowGraphic.hpp"
#include <iostream>
#include <string>
#include <QScrollArea>

// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::DrawCommandTab(QWidget *body) {

    auto *layout = qobject_cast<QVBoxLayout *>(body->layout());
    if (!layout) {
        layout = new QVBoxLayout(body);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
    }

    QWidget *cmdWidget = new QWidget(body);
    AddCommand(cmdWidget);
    layout->addWidget(cmdWidget);

    auto *scroll = new QScrollArea(body);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    layout->addWidget(scroll);

    QWidget *bbox = new QWidget(scroll);
    auto *boxLayout = new QVBoxLayout(bbox);
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->setSpacing(0);

    const auto& cfg = _application->GetData()->GetConfigMap();
    for (const auto& [key, value] : cfg) {
        if (key.rfind("cs#", 0) == 0) {
            if (QWidget *w = Command(key)) {
                boxLayout->addWidget(w, 0, Qt::AlignTop);
            }
        }
    }

    boxLayout->addStretch();

    scroll->setWidget(bbox);
}



// ────────────────────────────────────────────────────────────────────────────────────────────────────────


void WorkflowGraphic::AddCommand(QWidget *container) {

    auto *v = qobject_cast<QVBoxLayout *>(container->layout());
    if (!v) {
        v = new QVBoxLayout(container);
        v->setContentsMargins(0, 0, 0, 0);
        v->setSpacing(0);
    }

    QWidget *row = new QWidget(container);
    auto *h = new QHBoxLayout(row);
    h->setContentsMargins(8, 8, 8, 8);
    h->setSpacing(8);

    auto *btnPlus = new QPushButton("+", row);
    btnPlus->setCursor(Qt::PointingHandCursor);
    btnPlus->setFixedSize(32, 32);
    btnPlus->setStyleSheet(
        "QPushButton { background-color: #1e1e1e; color: white; border: 1px solid #3a3a3a; border-radius: 4px; }"
        "QPushButton:hover { background-color: #2a2a2a; }"
        "QPushButton:pressed { background-color: #171717; }"
        "QPushButton:focus { border: 1px solid #5a5a5a; }"
    );

    h->addWidget(btnPlus, 0, Qt::AlignRight);

    auto addAndRefresh = [this]() {

        _application->GetData()->AddCommand();
        RebuildBody();
    };

    QObject::connect(btnPlus, &QPushButton::clicked, row, addAndRefresh);


    v->addWidget(row, 0, Qt::AlignTop);
}


// ────────────────────────────────────────────────────────────────────────────────────────────────────────


QWidget *WorkflowGraphic::Command(const std::string& id) {
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
        const auto it = cfg.find(id);
        const std::string customCmd = (it != cfg.end()) ? it->second : std::string();
        input->setPlaceholderText(QString::fromStdString(customCmd.empty() ? "Enter custom command." : customCmd));
    }
    input->setClearButtonEnabled(true);
    input->setFixedWidth(240);
    input->setStyleSheet("QLineEdit { background: #1e1e1e; color: white; border: 1px solid #3a3a3a; padding: 6px 8px; }"
                         "QLineEdit:focus { border: 1px solid #5a5a5a; }");

    auto *btnRemove = new QPushButton("-", row);
    btnRemove->setCursor(Qt::PointingHandCursor);
    btnRemove->setFixedSize(32, 32);
    btnRemove->setToolTip("Remove this command");
    btnRemove->setStyleSheet(
        "QPushButton { background-color: #1e1e1e; color: white; border: 1px solid #3a3a3a; border-radius: 4px; }"
        "QPushButton:hover { background-color: #2a2a2a; }"
        "QPushButton:pressed { background-color: #171717; }"
        "QPushButton:focus { border: 1px solid #5a5a5a; }"
    );

    h->addWidget(btn, 1);
    h->addWidget(input, 0, Qt::AlignRight);
    h->addWidget(btnRemove, 0, Qt::AlignRight);

    QObject::connect(input, &QLineEdit::returnPressed, row, [this, input, id]() {
        const QString cmd = input->text().trimmed();
        if (cmd.isEmpty())
            return;
        _application->GetData()->ChangeConfig(id, input->text().toStdString());
        input->setPlaceholderText(cmd);
        input->clear();
    });

    QObject::connect(btn, &QPushButton::clicked, row, [this, input, id]() {
        QString cmd = input->text().trimmed();
        if (cmd.isEmpty()) {
            const auto cfg = _application->GetData()->GetConfigMap();
            const auto it = cfg.find(id);
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

    QObject::connect(btnRemove, &QPushButton::clicked, row, [this, id]() {
        _application->GetData()->RemoveCommand(id);
        RebuildBody();
    });

    row->setFocusProxy(btn);
    return row;
}
