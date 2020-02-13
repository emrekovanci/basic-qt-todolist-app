#pragma once

#include <QMainWindow>
#include <QVector>
#include <QJsonDocument>
#include "Task.hpp"

#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void updateStatus();

public slots:
    void addTask();
    void removeTask(Task* task);
    void taskStatusChanged(Task*);

private:
    Ui::MainWindow* ui;
    QVector<Task*> mTasks;
    QJsonDocument jsonDocument;
    const QString path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

private:
    bool readDB();
    void initializeOnBegin();
    void createTask(int id, const QString& name, bool status);

    QJsonDocument loadJson(const QString& fileName);
    void saveJson(const QJsonDocument& document, const QString& fileName);


    #ifdef Q_OS_ANDROID
        bool requestAndroidPermissions();
    #endif
};
