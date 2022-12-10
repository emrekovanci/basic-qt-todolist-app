#pragma once

#include <QMainWindow>
#include <QVector>
#include <QJsonDocument>
#include <QStandardPaths>

#include "Task.hpp"
#include "JsonEditor.hpp"

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
    void clearAllTasks();
    void addTask();
    void removeTask(Task* task);

private:
    Ui::MainWindow* ui;
    QVector<Task*> _Tasks;
    QJsonDocument _JsonDoc;
    const QString _DbPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + QString("/db.json");
    const Json::JsonEditor _JsonEditor { _DbPath };

private:
    bool readDB();
    void initializeOnBegin();
    void createTask(int id, const QString& name, bool status);
};
