#include "Task.hpp"
#include "ui_Task.h"

#include <QDebug>
#include <QString>
#include <QInputDialog>

Task::Task(const QString& name, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Task) {
    ui->setupUi(this);
    setName(name);
    connect(ui->editButton, &QPushButton::clicked, this, &Task::rename);
    connect(ui->removeButton, &QPushButton::clicked, [this, name] {
        qDebug() << "Trying to remove " << name;
        this->emit removed(this);
    });
    connect(ui->checkBox, &QCheckBox::toggled, this, &Task::checked);
}

Task::~Task() {
    delete ui;
}

void Task::setName(const QString& name) {
    ui->checkBox->setText(name);
}

QString Task::name() const {
    return ui->checkBox->text();
}

bool Task::isCompleted() const {
    return ui->checkBox->isChecked();
}

void Task::setChecked(bool checked) {
    QFont font(ui->checkBox->font());
    font.setStrikeOut(checked);
    ui->checkBox->setFont(font);
    emit statusChanged(this);
}

void Task::rename() {
    bool ok{ false };
    QString value = QInputDialog::getText(this, tr("Edit Task"), tr("Task Name"), QLineEdit::Normal, this->name(), &ok);

    if (ok && !value.isEmpty()) {
        setName(value);
    }
}

void Task::checked(bool checked) {
    QFont font(ui->checkBox->font());
    font.setStrikeOut(checked);
    ui->checkBox->setFont(font);
    emit statusChanged(this);
}
