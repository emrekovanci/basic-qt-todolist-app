#pragma once

#include <QMainWindow>
#include <QVector>

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
};
