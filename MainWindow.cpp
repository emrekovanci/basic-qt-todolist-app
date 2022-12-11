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

    initTasks();
    updateStatus();

    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(ui->clearAllButton, &QPushButton::clicked, this, &MainWindow::clearAllTasks);
}

void MainWindow::initTasks()
{
    for (const auto& val : _JsonEditor.GetSource()->array())
    {
        const auto& jsonObject = val.toObject();

        int id = jsonObject["id"].toInt();
        QString name = jsonObject["name"].toString();
        bool status = jsonObject["status"].toBool();

        createTask(id, name, status);
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
    QJsonArray array = _JsonEditor.GetSource()->array();
    for (const auto& taskInJson : array)
    {
        int id = taskInJson.toObject().find("id")->toInt();
        if (id != task->getTaskId()) { continue; }

        index = id;
    }

    array.takeAt(index);
    _JsonEditor.GetSource()->setArray(array);
    _JsonEditor.SaveJson();

    ui->tasksLayout->removeWidget(task);

    // remove the task
    delete _Tasks.takeAt(_Tasks.indexOf(task));
    task = nullptr;
    _Tasks.shrink_to_fit();

    updateStatus();
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
