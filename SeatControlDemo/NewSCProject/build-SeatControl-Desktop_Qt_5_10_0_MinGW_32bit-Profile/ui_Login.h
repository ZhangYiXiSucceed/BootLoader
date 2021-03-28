/********************************************************************************
** Form generated from reading UI file 'Login.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *lineEditRegistrationCode;
    QToolButton *tbnOk;
    QToolButton *tbnCancel;
    QLabel *label_2;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QStringLiteral("Login"));
        Login->resize(400, 300);
        groupBox = new QGroupBox(Login);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(60, 80, 271, 131));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 50, 51, 31));
        lineEditRegistrationCode = new QLineEdit(groupBox);
        lineEditRegistrationCode->setObjectName(QStringLiteral("lineEditRegistrationCode"));
        lineEditRegistrationCode->setGeometry(QRect(70, 50, 181, 31));
        tbnOk = new QToolButton(Login);
        tbnOk->setObjectName(QStringLiteral("tbnOk"));
        tbnOk->setGeometry(QRect(250, 237, 81, 31));
        tbnCancel = new QToolButton(Login);
        tbnCancel->setObjectName(QStringLiteral("tbnCancel"));
        tbnCancel->setGeometry(QRect(60, 237, 81, 31));
        label_2 = new QLabel(Login);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(60, 30, 271, 41));

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "\345\272\247\346\244\205\346\216\247\345\210\266\345\231\250", nullptr));
        groupBox->setTitle(QString());
        label->setText(QApplication::translate("Login", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">\346\263\250\345\206\214\347\240\201\357\274\232</span></p></body></html>", nullptr));
        tbnOk->setText(QApplication::translate("Login", "\347\231\273\345\275\225", nullptr));
        tbnCancel->setText(QApplication::translate("Login", "\351\200\200\345\207\272", nullptr));
        label_2->setText(QApplication::translate("Login", "<html><head/><body><p align=\"center\"><span style=\" font-size:24pt; font-weight:600; color:#5555ff;\">\345\272\247\346\244\205\346\216\247\345\210\266\345\231\250</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
