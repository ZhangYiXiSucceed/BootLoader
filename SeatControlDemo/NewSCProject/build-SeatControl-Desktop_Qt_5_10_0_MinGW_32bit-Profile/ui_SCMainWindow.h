/********************************************************************************
** Form generated from reading UI file 'SCMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCMAINWINDOW_H
#define UI_SCMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCMainWindow
{
public:
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SCMainWindow)
    {
        if (SCMainWindow->objectName().isEmpty())
            SCMainWindow->setObjectName(QStringLiteral("SCMainWindow"));
        SCMainWindow->resize(684, 542);
        centralWidget = new QWidget(SCMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SCMainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SCMainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SCMainWindow->setStatusBar(statusBar);

        retranslateUi(SCMainWindow);

        QMetaObject::connectSlotsByName(SCMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SCMainWindow)
    {
        SCMainWindow->setWindowTitle(QApplication::translate("SCMainWindow", "SCMainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCMainWindow: public Ui_SCMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCMAINWINDOW_H
