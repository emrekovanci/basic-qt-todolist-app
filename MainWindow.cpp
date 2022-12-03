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
        QJsonArray taskList = _JsonDoc.array();
        for (const auto val : taskList)
        {
            int id = val.toObject().value("id").toInt();
            QString name = val.toObject().value("name").toString();
            bool status = val.toObject().value("status").toBool();
            createTask(id, name, status);
            qDebug() << "Task ID:" << id << "-" << "Task Name:" << name;
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
        qDebug() << "Adding new task";
        Task* task = new Task(name);
        task->setTaskId(_Tasks.size());
        _Tasks.append(task);
        ui->tasksLayout->addWidget(task);
        connect(task, &Task::removed, this, &MainWindow::removeTask);
        updateStatus();

        // create new json object with properties
        QJsonObject taskValues;
        taskValues.insert("name", name);
        taskValues.insert("status", false);
        taskValues.insert("id", _Tasks.size());

        // add created json object to json array
        QJsonArray array = _JsonDoc.array();
        array.append(taskValues);
        _JsonDoc.setArray(array);

        saveJson(_JsonDoc, _DbPath);
    }
}

void MainWindow::removeTask(Task* task)
{
    int index{-1};

    // search task in json and remove it
    QJsonArray array = _JsonDoc.array();
    for (const auto taskInJson : array)
    {
        int id = taskInJson.toObject().find("id")->toInt();
        if (id != task->getTaskId()) { continue; }

        index = id;
    }

    array.takeAt(index);
    _JsonDoc.setArray(array);
    saveJson(_JsonDoc, _DbPath);

    ui->tasksLayout->removeWidget(task);

    // remove the task
    int taskIndex = _Tasks.indexOf(task);
    delete _Tasks.takeAt(_Tasks.indexOf(task));
    task = nullptr;
    _Tasks.shrink_to_fit();

    updateStatus();
}

bool MainWindow::readDB()
{
    qDebug() << "File reading:" << _DbPath;

    QJsonParseError json_error{};

    QFile file(_DbPath);
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString jsonString = QString::fromUtf8(file.readAll());
        _JsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &json_error);
        if (json_error.error != QJsonParseError::NoError)
        {
            qDebug() << "Error:" << json_error.errorString() << " offset:" << json_error.offset;
            return false;
        }
        return true;
    }
    return false;
}

void MainWindow::createTask(int id, const QString& name, bool status)
{
    Task* task = new Task(name);
    task->setChecked(status);
    task->setTaskId(id);
    _Tasks.append(task);
    ui->tasksLayout->addWidget(task);
    connect(task, &Task::removed, this, &MainWindow::removeTask);
    updateStatus();
}

QJsonDocument MainWindow::loadJson(const QString& fileName)
{
    QFile jsonFile{ fileName };
    jsonFile.open(QIODevice::ReadOnly);

    return QJsonDocument::fromJson(jsonFile.readAll());
}

void MainWindow::saveJson(const QJsonDocument& document, const QString& fileName)
{
    QFile jsonFile{ fileName };
    jsonFile.open(QIODevice::WriteOnly | QFile::Truncate);
    jsonFile.write(document.toJson());

    if (jsonFile.error())
    {
        qDebug() << "Error:" << jsonFile.errorString();
    }
}

void MainWindow::updateStatus()
{
    int completedTaskCount = std::count_if(
        std::begin(_Tasks),
        std::end(_Tasks),
        [](Task* task) { return task->isCompleted(); }
    );

    ui->infoLabel->setText(QString("%1 todo / %2 completed").arg(_Tasks.count()).arg(completedTaskCount));
}
