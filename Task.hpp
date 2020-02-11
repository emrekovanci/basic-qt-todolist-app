#pragma once

#include <QWidget>

class QString;

namespace Ui {
    class Task;
}

class Task : public QWidget {
    Q_OBJECT

public:
    explicit Task(const QString& name, QWidget* parent = nullptr);
    ~Task();

    void setName(const QString& name);
    QString name() const;
    bool isCompleted() const;

public slots:
    void rename();

signals:
    void removed(Task* task);
    void statusChanged(Task* task);

private slots:
    void checked(bool checked);

private:
    Ui::Task* ui;
};

