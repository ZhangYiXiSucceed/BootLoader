/********************************************************************************
** Form generated from reading UI file 'SCButton.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCBUTTON_H
#define UI_SCBUTTON_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCButton
{
public:
    QGroupBox *groupBox;
    QToolButton *tbnMotorBackOpen;
    QToolButton *tbnMotorLegOpen;
    QToolButton *tbnMotorFootOpen;
    QToolButton *tbnMotorBackClose;
    QToolButton *tbnMotorLegClose;
    QToolButton *tbnMotorFootClose;
    QGroupBox *groupBox_2;
    QToolButton *tbnFastOpen;
    QToolButton *tbnFastClose;
    QToolButton *tbnHalf;
    QToolButton *tbnOpenCLose;
    QGroupBox *groupBox_3;
    QToolButton *tbnReadCurrent;
    QLineEdit *leMotorBackCurrent;
    QLineEdit *leMotorLegCurrent;
    QLineEdit *leMotorFootCurrent;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QGroupBox *groupBox_4;
    QToolButton *toolButton_9;

    void setupUi(QWidget *SCButton)
    {
        if (SCButton->objectName().isEmpty())
            SCButton->setObjectName(QStringLiteral("SCButton"));
        SCButton->resize(661, 462);
        groupBox = new QGroupBox(SCButton);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 10, 291, 191));
        tbnMotorBackOpen = new QToolButton(groupBox);
        tbnMotorBackOpen->setObjectName(QStringLiteral("tbnMotorBackOpen"));
        tbnMotorBackOpen->setGeometry(QRect(20, 30, 111, 31));
        tbnMotorLegOpen = new QToolButton(groupBox);
        tbnMotorLegOpen->setObjectName(QStringLiteral("tbnMotorLegOpen"));
        tbnMotorLegOpen->setGeometry(QRect(20, 80, 111, 31));
        tbnMotorFootOpen = new QToolButton(groupBox);
        tbnMotorFootOpen->setObjectName(QStringLiteral("tbnMotorFootOpen"));
        tbnMotorFootOpen->setGeometry(QRect(20, 130, 111, 31));
        tbnMotorBackClose = new QToolButton(groupBox);
        tbnMotorBackClose->setObjectName(QStringLiteral("tbnMotorBackClose"));
        tbnMotorBackClose->setGeometry(QRect(150, 30, 111, 31));
        tbnMotorLegClose = new QToolButton(groupBox);
        tbnMotorLegClose->setObjectName(QStringLiteral("tbnMotorLegClose"));
        tbnMotorLegClose->setGeometry(QRect(150, 80, 111, 31));
        tbnMotorFootClose = new QToolButton(groupBox);
        tbnMotorFootClose->setObjectName(QStringLiteral("tbnMotorFootClose"));
        tbnMotorFootClose->setGeometry(QRect(150, 130, 111, 31));
        groupBox_2 = new QGroupBox(SCButton);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(340, 10, 301, 191));
        tbnFastOpen = new QToolButton(groupBox_2);
        tbnFastOpen->setObjectName(QStringLiteral("tbnFastOpen"));
        tbnFastOpen->setGeometry(QRect(20, 40, 111, 31));
        tbnFastClose = new QToolButton(groupBox_2);
        tbnFastClose->setObjectName(QStringLiteral("tbnFastClose"));
        tbnFastClose->setGeometry(QRect(170, 40, 111, 31));
        tbnHalf = new QToolButton(groupBox_2);
        tbnHalf->setObjectName(QStringLiteral("tbnHalf"));
        tbnHalf->setGeometry(QRect(20, 110, 111, 31));
        tbnOpenCLose = new QToolButton(groupBox_2);
        tbnOpenCLose->setObjectName(QStringLiteral("tbnOpenCLose"));
        tbnOpenCLose->setGeometry(QRect(170, 110, 111, 31));
        groupBox_3 = new QGroupBox(SCButton);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 220, 291, 211));
        tbnReadCurrent = new QToolButton(groupBox_3);
        tbnReadCurrent->setObjectName(QStringLiteral("tbnReadCurrent"));
        tbnReadCurrent->setGeometry(QRect(150, 160, 121, 41));
        leMotorBackCurrent = new QLineEdit(groupBox_3);
        leMotorBackCurrent->setObjectName(QStringLiteral("leMotorBackCurrent"));
        leMotorBackCurrent->setGeometry(QRect(130, 40, 131, 31));
        leMotorLegCurrent = new QLineEdit(groupBox_3);
        leMotorLegCurrent->setObjectName(QStringLiteral("leMotorLegCurrent"));
        leMotorLegCurrent->setGeometry(QRect(130, 80, 131, 31));
        leMotorFootCurrent = new QLineEdit(groupBox_3);
        leMotorFootCurrent->setObjectName(QStringLiteral("leMotorFootCurrent"));
        leMotorFootCurrent->setGeometry(QRect(130, 120, 131, 31));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 40, 91, 31));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 80, 91, 31));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 121, 81, 31));
        groupBox_4 = new QGroupBox(SCButton);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(340, 220, 301, 211));
        toolButton_9 = new QToolButton(groupBox_4);
        toolButton_9->setObjectName(QStringLiteral("toolButton_9"));
        toolButton_9->setGeometry(QRect(20, 30, 91, 31));

        retranslateUi(SCButton);

        QMetaObject::connectSlotsByName(SCButton);
    } // setupUi

    void retranslateUi(QWidget *SCButton)
    {
        SCButton->setWindowTitle(QApplication::translate("SCButton", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("SCButton", "\346\214\211\351\224\256\346\250\241\346\213\237", nullptr));
        tbnMotorBackOpen->setText(QApplication::translate("SCButton", "\350\203\214\351\235\240\346\211\223\345\274\200", nullptr));
        tbnMotorLegOpen->setText(QApplication::translate("SCButton", "\350\205\277\351\235\240\346\211\223\345\274\200", nullptr));
        tbnMotorFootOpen->setText(QApplication::translate("SCButton", "\350\204\232\350\270\217\346\211\223\345\274\200", nullptr));
        tbnMotorBackClose->setText(QApplication::translate("SCButton", "\350\203\214\351\235\240\346\224\266\350\265\267", nullptr));
        tbnMotorLegClose->setText(QApplication::translate("SCButton", "\350\205\277\351\235\240\346\224\266\350\265\267", nullptr));
        tbnMotorFootClose->setText(QApplication::translate("SCButton", "\350\204\232\350\270\217\346\224\266\350\265\267", nullptr));
        groupBox_2->setTitle(QApplication::translate("SCButton", "\344\270\200\351\224\256\346\265\213\350\257\225", nullptr));
        tbnFastOpen->setText(QApplication::translate("SCButton", "\344\270\200\351\224\256\345\205\250\345\274\200", nullptr));
        tbnFastClose->setText(QApplication::translate("SCButton", "\344\270\200\351\224\256\345\205\250\346\224\266", nullptr));
        tbnHalf->setText(QApplication::translate("SCButton", "\344\270\200\351\224\256\345\215\212\350\272\272", nullptr));
        tbnOpenCLose->setText(QApplication::translate("SCButton", "\345\276\200\345\244\215\346\265\213\350\257\225", nullptr));
        groupBox_3->setTitle(QApplication::translate("SCButton", "\346\214\211\351\224\256\346\250\241\346\213\237", nullptr));
        tbnReadCurrent->setText(QApplication::translate("SCButton", "\350\257\273\345\217\226\345\217\202\346\225\260", nullptr));
        label->setText(QApplication::translate("SCButton", "\350\203\214\351\235\240\347\224\265\346\234\272\347\224\265\346\265\201", nullptr));
        label_2->setText(QApplication::translate("SCButton", "\350\205\277\351\235\240\347\224\265\346\234\272\347\224\265\346\265\201", nullptr));
        label_3->setText(QApplication::translate("SCButton", "\350\204\232\350\270\217\347\224\265\346\234\272\347\224\265\346\265\201", nullptr));
        groupBox_4->setTitle(QApplication::translate("SCButton", "\345\212\250\347\224\273\346\250\241\346\213\237", nullptr));
        toolButton_9->setText(QApplication::translate("SCButton", "\345\274\200\345\247\213\345\212\250\347\224\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCButton: public Ui_SCButton {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCBUTTON_H
