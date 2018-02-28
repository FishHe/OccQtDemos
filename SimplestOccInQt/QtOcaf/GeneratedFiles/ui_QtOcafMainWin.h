/********************************************************************************
** Form generated from reading UI file 'QtOcafMainWin.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTOCAFMAINWIN_H
#define UI_QTOCAFMAINWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtOcafMainWinClass
{
public:
    QAction *actionBox;
    QAction *actionUndo;
    QAction *actionRedo;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menuEdit;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtOcafMainWinClass)
    {
        if (QtOcafMainWinClass->objectName().isEmpty())
            QtOcafMainWinClass->setObjectName(QStringLiteral("QtOcafMainWinClass"));
        QtOcafMainWinClass->resize(600, 400);
        actionBox = new QAction(QtOcafMainWinClass);
        actionBox->setObjectName(QStringLiteral("actionBox"));
        actionUndo = new QAction(QtOcafMainWinClass);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionRedo = new QAction(QtOcafMainWinClass);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        centralWidget = new QWidget(QtOcafMainWinClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QtOcafMainWinClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtOcafMainWinClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        QtOcafMainWinClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtOcafMainWinClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtOcafMainWinClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtOcafMainWinClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtOcafMainWinClass->setStatusBar(statusBar);

        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menu->menuAction());
        menu->addAction(actionBox);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);

        retranslateUi(QtOcafMainWinClass);

        QMetaObject::connectSlotsByName(QtOcafMainWinClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtOcafMainWinClass)
    {
        QtOcafMainWinClass->setWindowTitle(QApplication::translate("QtOcafMainWinClass", "QtOcaf", nullptr));
        actionBox->setText(QApplication::translate("QtOcafMainWinClass", "Box", nullptr));
        actionUndo->setText(QApplication::translate("QtOcafMainWinClass", "Undo", nullptr));
        actionRedo->setText(QApplication::translate("QtOcafMainWinClass", "Redo", nullptr));
        menu->setTitle(QApplication::translate("QtOcafMainWinClass", "Model", nullptr));
        menu_2->setTitle(QApplication::translate("QtOcafMainWinClass", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("QtOcafMainWinClass", "Edit", nullptr));
        menuView->setTitle(QApplication::translate("QtOcafMainWinClass", "View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtOcafMainWinClass: public Ui_QtOcafMainWinClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTOCAFMAINWIN_H
