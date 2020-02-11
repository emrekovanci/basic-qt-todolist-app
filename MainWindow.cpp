#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QString>
#include <QList>
#include <QVariant>
#include <QFile>

#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);

    initializeOnBegin();
    updateStatus();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addTask() {
    bool ok{ false };
    QString name = QInputDialog::getText(this, tr("Add Task"), tr("Task Name"), QLineEdit::Normal, tr("Untitled Task"), &ok);

    if (ok && !name.isEmpty()) {
        qDebug() << "Adding new task";
        Task* task = new Task(name);
        mTasks.append(task);
        ui->tasksLayout->addWidget(task);
        connect(task, &Task::removed, this, &MainWindow::removeTask);
        updateStatus();
    }
}

void MainWindow::removeTask(Task* task) {
    mTasks.removeOne(task);
    ui->tasksLayout->removeWidget(task);
    delete task;
    updateStatus();
}

void MainWindow::taskStatusChanged(Task* /*task*/) {
    updateStatus();
}

bool MainWindow::readDB() {
    auto path = qApp->applicationDirPath(); //QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    auto fileName = path + "/db.json";
    qDebug() << "File reading:" << fileName;

    QJsonParseError json_error;

    QFile file(fileName);
    if (file.exists()) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString jsonString = QString::fromUtf8(file.readAll());
        json_document = QJsonDocument::fromJson(jsonString.toUtf8(), &json_error);
        if (json_error.error != QJsonParseError::NoError) {
            qDebug() << "Error:" << json_error.errorString() << " offset:" << json_error.offset;
            return false;
        }
        return true;
    } else {
        return false;
    }
}

void MainWindow::initializeOnBegin() {
    if (readDB()) {
        QList<QVariant> list = json_document.toVariant().toList();
        qDebug() << "Total task count: " << list.size();

        for (int i{ 0 }; i < list.size(); ++i) {
            QMap<QString, QVariant> map = list[i].toMap();
            createTask(map["name"].toString(), map["status"].toBool());
            qDebug() << map["name"].toString() << "-" << map["status"].toBool();
        }
    }
}

void MainWindow::createTask(const QString& name, bool status) {
    Task* task = new Task(name);
    task->setChecked(status);
    mTasks.append(task);
    ui->tasksLayout->addWidget(task);
    connect(task, &Task::removed, this, &MainWindow::removeTask);
    updateStatus();
}

void MainWindow::updateStatus() {
    int completedCount{ 0 };
    for (auto task: mTasks) {
        if (task->isCompleted()) {
            ++completedCount;
        }
    }

    int todoCount{ mTasks.size() - completedCount };
    ui->statusLabel->setText(QString("Status: %1 todo / %2 completed").arg(todoCount).arg(completedCount));
}

