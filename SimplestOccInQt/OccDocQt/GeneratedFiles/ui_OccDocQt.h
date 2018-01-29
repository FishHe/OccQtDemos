/********************************************************************************
** Form generated from reading UI file 'OccDocQt.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OCCDOCQT_H
#define UI_OCCDOCQT_H

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

class Ui_OccDocQtClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *OccDocQtClass)
    {
        if (OccDocQtClass->objectName().isEmpty())
            OccDocQtClass->setObjectName(QStringLiteral("OccDocQtClass"));
        OccDocQtClass->resize(600, 400);
        menuBar = new QMenuBar(OccDocQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        OccDocQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(OccDocQtClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        OccDocQtClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(OccDocQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        OccDocQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(OccDocQtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        OccDocQtClass->setStatusBar(statusBar);

        retranslateUi(OccDocQtClass);

        QMetaObject::connectSlotsByName(OccDocQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *OccDocQtClass)
    {
        OccDocQtClass->setWindowTitle(QApplication::translate("OccDocQtClass", "OccDocQt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OccDocQtClass: public Ui_OccDocQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OCCDOCQT_H
