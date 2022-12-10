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

#include "JsonEditor.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initializeOnBegin();
    updateStatus();

    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(ui->clearAllButton, &QPushButton::clicked, this, &MainWindow::clearAllTasks);
}

void MainWindow::initializeOnBegin()
{
    if (readDB())
    {
        for (const auto& val : _JsonDoc.array())
        {
            int id = val.toObject().value("id").toInt();
            QString name = val.toObject().value("name").toString();
            bool status = val.toObject().value("status").toBool();

            createTask(id, name, status);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearAllTasks()
{
    for (const Task* item : _Tasks)
    {
        delete item;
    }
    _Tasks.clear();

    QFile file(_DbPath);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.close();
}

void MainWindow::addTask()
{
    bool ok{ false };
    QString name = QInputDialog::getText(this, tr("Add Task"), tr("Task Name"), QLineEdit::Normal, tr("Untitled Task"), &ok);


    if (ok && !name.isEmpty())
    {
        createTask(_Tasks.size(), name, false);

        // create new json object with properties
        QJsonObject taskJson{};
        taskJson["name"] = name;
        taskJson["status"] = false;
        taskJson["id"] = _Tasks.size();

        _JsonEditor.Push(taskJson);
    }
}

void MainWindow::removeTask(Task* task)
{
    int index{-1};

    // search task in json and remove it
    QJsonArray array = _JsonDoc.array();
    for (const auto& taskInJson : array)
    {
        int id = taskInJson.toObject().find("id")->toInt();
        if (id != task->getTaskId()) { continue; }

        index = id;
    }

    array.takeAt(index);
    _JsonDoc.setArray(array);

    Json::JsonEditor reader{_DbPath};
    reader.SaveJson();

    ui->tasksLayout->removeWidget(task);

    // remove the task
    delete _Tasks.takeAt(_Tasks.indexOf(task));
    task = nullptr;
    _Tasks.shrink_to_fit();

    updateStatus();
}

bool MainWindow::readDB()
{
    QJsonParseError json_error{};

    if (QFile file(_DbPath); file.exists())
    {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString jsonString = QString::fromUtf8(file.readAll());
        _JsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &json_error);

        if (json_error.error != QJsonParseError::NoError)
        {
            qDebug() << "Error:" << json_error.errorString() << " offset:" << json_error.offset;
        }

        return json_error.error == QJsonParseError::NoError;
    }

    return false;
}

void MainWindow::createTask(int id, const QString& name, bool status)
{
    // prepare taks
    auto task = new Task(name);
    task->setChecked(status);
    task->setTaskId(id);

    // prepare widget
    _Tasks.append(task);
    ui->tasksLayout->addWidget(task);

    // bind events
    connect(task, &Task::removed, this, &MainWindow::removeTask);
    connect(task, &Task::statusChanged, this, &MainWindow::updateStatus);

    updateStatus();
}

void MainWindow::updateStatus()
{
    auto completedTaskCount = std::ranges::count_if(
        _Tasks,
        [](const Task* task) { return task->isCompleted(); }
    );

    ui->infoLabel->setText(QString("%1 todo / %2 completed")
        .arg(_Tasks.count())
        .arg(completedTaskCount));
}
