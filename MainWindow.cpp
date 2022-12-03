#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QString>
#include <QFile>

#include <QInputDialog>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(ui->clearAllButton, &QPushButton::clicked, [this] () {
        auto fileName = this->path + "/db.json";
        QFile file(fileName);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        file.close();
        for (auto item : mTasks) {
            delete item;
        }
    });
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
        task->setTaskId(mTasks.size());
        mTasks.append(task);
        ui->tasksLayout->addWidget(task);
        connect(task, &Task::removed, this, &MainWindow::removeTask);
        updateStatus();

        // get db path
        auto fileName = this->path + "/db.json";

        // create new json object with properties
        QJsonObject newJson;
        newJson.insert("name", name);
        newJson.insert("status", false);
        newJson.insert("id", mTasks.size());

        // add created json object to json array
        QJsonArray array = jsonDocument.array();
        array.append(newJson);

        jsonDocument.setArray(array);
        saveJson(jsonDocument, fileName);
    }
}

void MainWindow::removeTask(Task* task) {
    // search task in json and remove it
    auto fileName = this->path + "/db.json";
    QJsonArray array = jsonDocument.array();
    for (const auto taskInJson : array) {
        auto jsonId = taskInJson.toObject().find("id")->toInt();
        qDebug() << "jsonId:" << jsonId;
        if (jsonId == task->getTaskId()) {
            array.removeAt(jsonId);
            jsonDocument.setArray(array);
            saveJson(jsonDocument, fileName);
            qDebug() << "task removed in json database!";
        }
    }

    // remove the task
    mTasks.removeOne(task);
    ui->tasksLayout->removeWidget(task);
    delete task;
    updateStatus();
}

void MainWindow::taskStatusChanged(Task* /*task*/) {
    updateStatus();
}

bool MainWindow::readDB() {
    auto fileName = this->path + "/db.json";
    qDebug() << "File reading:" << fileName;

    QJsonParseError json_error{};

    QFile file(fileName);
    if (file.exists()) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString jsonString = QString::fromUtf8(file.readAll());
        jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8(), &json_error);
        if (json_error.error != QJsonParseError::NoError) {
            qDebug() << "Error:" << json_error.errorString() << " offset:" << json_error.offset;
            return false;
        }
        return true;
    }
    return false;
}

void MainWindow::initializeOnBegin() {
    if (readDB()) {
        QJsonArray taskList = jsonDocument.array();
        for (const auto val : taskList) {
            int id = val.toObject().value("id").toInt();
            QString name = val.toObject().value("name").toString();
            bool status = val.toObject().value("status").toBool();
            createTask(id, name, status);
            qDebug() << "Task ID:" << id << "-" << "Task Name:" << name;
        }
    }
}

void MainWindow::createTask(int id, const QString& name, bool status) {
    Task* task = new Task(name);
    task->setChecked(status);
    task->setTaskId(id);
    mTasks.append(task);
    ui->tasksLayout->addWidget(task);
    connect(task, &Task::removed, this, &MainWindow::removeTask);
    updateStatus();
}

QJsonDocument MainWindow::loadJson(const QString& fileName) {
    QFile jsonFile{ fileName };
    jsonFile.open(QIODevice::ReadOnly);
    return QJsonDocument::fromJson(jsonFile.readAll());
}

void MainWindow::saveJson(const QJsonDocument& document, const QString& fileName) {
    QFile jsonFile{ fileName };
    jsonFile.open(QIODevice::WriteOnly | QFile::Truncate);
    jsonFile.write(document.toJson());
    if (jsonFile.error()) {
        qDebug() << "SaveError:" << jsonFile.errorString();
    }
}

void MainWindow::updateStatus() {
    int completedCount{ 0 };
    for (const auto& task: mTasks) {
        if (task->isCompleted()) {
            ++completedCount;
        }
    }

    int todoCount = mTasks.size() - completedCount;
    // ui->statusLabel->setText(QString("Status: %1 todo / %2 completed").arg(todoCount).arg(completedCount));
}
