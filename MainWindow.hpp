#pragma once

#include <QMainWindow>
#include <QVector>
#include <QJsonDocument>
#include "Task.hpp"

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

    bool readDB();
    void initializeOnBegin();
    void createTask(const QString& name, bool status);

    QJsonDocument load_json(QString file_name);
    void save_json(QJsonDocument document, QString file_name);

    QJsonDocument json_document;

    bool requestAndroidPermissions();
};
