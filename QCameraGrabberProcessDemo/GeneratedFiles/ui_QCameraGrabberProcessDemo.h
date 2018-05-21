/********************************************************************************
** Form generated from reading UI file 'QCameraGrabberProcessDemo.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCAMERAGRABBERPROCESSDEMO_H
#define UI_QCAMERAGRABBERPROCESSDEMO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QCameraGrabberProcessDemoClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QCameraGrabberProcessDemoClass)
    {
        if (QCameraGrabberProcessDemoClass->objectName().isEmpty())
            QCameraGrabberProcessDemoClass->setObjectName(QStringLiteral("QCameraGrabberProcessDemoClass"));
        QCameraGrabberProcessDemoClass->resize(600, 400);
        menuBar = new QMenuBar(QCameraGrabberProcessDemoClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        QCameraGrabberProcessDemoClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QCameraGrabberProcessDemoClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QCameraGrabberProcessDemoClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QCameraGrabberProcessDemoClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QCameraGrabberProcessDemoClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QCameraGrabberProcessDemoClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QCameraGrabberProcessDemoClass->setStatusBar(statusBar);

        retranslateUi(QCameraGrabberProcessDemoClass);

        QMetaObject::connectSlotsByName(QCameraGrabberProcessDemoClass);
    } // setupUi

    void retranslateUi(QMainWindow *QCameraGrabberProcessDemoClass)
    {
        QCameraGrabberProcessDemoClass->setWindowTitle(QApplication::translate("QCameraGrabberProcessDemoClass", "QCameraGrabberProcessDemo", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QCameraGrabberProcessDemoClass: public Ui_QCameraGrabberProcessDemoClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCAMERAGRABBERPROCESSDEMO_H
