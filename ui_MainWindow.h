/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *toolBarLayout;
    QLabel *infoLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *addTaskButton;
    QPushButton *clearAllButton;
    QProgressBar *progressBar;
    QGroupBox *tasks;
    QVBoxLayout *tasksLayout;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(613, 421);
        MainWindow->setAutoFillBackground(true);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        toolBarLayout = new QHBoxLayout();
        toolBarLayout->setObjectName("toolBarLayout");
        infoLabel = new QLabel(centralwidget);
        infoLabel->setObjectName("infoLabel");

        toolBarLayout->addWidget(infoLabel);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolBarLayout->addItem(horizontalSpacer);

        addTaskButton = new QPushButton(centralwidget);
        addTaskButton->setObjectName("addTaskButton");
        addTaskButton->setAutoFillBackground(true);
        addTaskButton->setStyleSheet(QString::fromUtf8(""));

        toolBarLayout->addWidget(addTaskButton);

        clearAllButton = new QPushButton(centralwidget);
        clearAllButton->setObjectName("clearAllButton");

        toolBarLayout->addWidget(clearAllButton);


        verticalLayout->addLayout(toolBarLayout);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy);
        progressBar->setValue(24);
        progressBar->setTextVisible(false);

        verticalLayout->addWidget(progressBar);

        tasks = new QGroupBox(centralwidget);
        tasks->setObjectName("tasks");
        tasksLayout = new QVBoxLayout(tasks);
        tasksLayout->setObjectName("tasksLayout");

        verticalLayout->addWidget(tasks);

        verticalSpacer = new QSpacerItem(20, 120, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "toDO", nullptr));
        infoLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        addTaskButton->setText(QCoreApplication::translate("MainWindow", "Add Task", nullptr));
        clearAllButton->setText(QCoreApplication::translate("MainWindow", "Clear All", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
