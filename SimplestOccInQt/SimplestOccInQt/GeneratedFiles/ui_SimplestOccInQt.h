/********************************************************************************
** Form generated from reading UI file 'SimplestOccInQt.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLESTOCCINQT_H
#define UI_SIMPLESTOCCINQT_H

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

class Ui_SimplestOccInQtClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SimplestOccInQtClass)
    {
        if (SimplestOccInQtClass->objectName().isEmpty())
            SimplestOccInQtClass->setObjectName(QStringLiteral("SimplestOccInQtClass"));
        SimplestOccInQtClass->resize(600, 400);
        menuBar = new QMenuBar(SimplestOccInQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        SimplestOccInQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SimplestOccInQtClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SimplestOccInQtClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SimplestOccInQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SimplestOccInQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SimplestOccInQtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SimplestOccInQtClass->setStatusBar(statusBar);

        retranslateUi(SimplestOccInQtClass);

        QMetaObject::connectSlotsByName(SimplestOccInQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *SimplestOccInQtClass)
    {
        SimplestOccInQtClass->setWindowTitle(QApplication::translate("SimplestOccInQtClass", "SimplestOccInQt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SimplestOccInQtClass: public Ui_SimplestOccInQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLESTOCCINQT_H
