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
#include <QStandardPaths>
#include <QDir>

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QtAndroidExtras>
#endif

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);

    initializeOnBegin();
    updateStatus();

    #ifdef Q_OS_ANDROID
        requestAndroidPermissions();
    #endif
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

        // get db path
        auto path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        auto fileName = path + "/db.json";

        // create new json object with properties
        QJsonObject new_json;
        new_json.insert("name", name);
        new_json.insert("status", false);

        // add created json object to json array
        QJsonArray array = json_document.array();
        array.append(new_json);

        json_document.setArray(array);
        save_json(json_document, fileName);
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
    auto path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    auto fileName = path + "/db.json";
    qDebug() << "File reading:" << fileName;

    QJsonParseError json_error;

    QFile file(fileName);
    if (file.exists()) {
        file.open(QIODevice::ReadWrite | QIODevice::Text);
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
        QJsonArray task_array = json_document.array();
        foreach (const QJsonValue& val, task_array) {
            QString name = val.toObject().value("name").toString();
            bool status = val.toObject().value("status").toBool();
            createTask(name, status);
            qDebug() << "Task Name:" << name;
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

QJsonDocument MainWindow::load_json(QString file_name) {
    QFile json_file{ file_name };
    json_file.open(QIODevice::ReadWrite);
    return QJsonDocument().fromJson(json_file.readAll());
}

void MainWindow::save_json(QJsonDocument document, QString file_name) {
    QFile json_file{ file_name };
    json_file.open(QIODevice::ReadWrite | QFile::Truncate);
    json_file.write(document.toJson());
    if (json_file.error()) {
        qDebug() << "SaveError:" << json_file.errorString();
    }
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

#ifdef Q_OS_ANDROID
bool MainWindow::requestAndroidPermissions() {
    // Request requiered permissions at runtime
    const QVector<QString> permissions({
        "android.permission.WRITE_EXTERNAL_STORAGE",
        "android.permission.READ_EXTERNAL_STORAGE"});

    for (const QString& permission : permissions) {
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied) {
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
                return false;
        }
    }

    return true;
}
#endif
