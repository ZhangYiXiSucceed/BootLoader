/********************************************************************************
** Form generated from reading UI file 'SCRecordEvent.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCRECORDEVENT_H
#define UI_SCRECORDEVENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCRecordEvent
{
public:
    QTextEdit *textEdit;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;

    void setupUi(QWidget *SCRecordEvent)
    {
        if (SCRecordEvent->objectName().isEmpty())
            SCRecordEvent->setObjectName(QStringLiteral("SCRecordEvent"));
        SCRecordEvent->resize(857, 501);
        textEdit = new QTextEdit(SCRecordEvent);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(10, 10, 841, 421));
        toolButton = new QToolButton(SCRecordEvent);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(780, 450, 71, 31));
        toolButton_2 = new QToolButton(SCRecordEvent);
        toolButton_2->setObjectName(QStringLiteral("toolButton_2"));
        toolButton_2->setGeometry(QRect(680, 450, 71, 31));
        toolButton_3 = new QToolButton(SCRecordEvent);
        toolButton_3->setObjectName(QStringLiteral("toolButton_3"));
        toolButton_3->setGeometry(QRect(580, 450, 71, 31));

        retranslateUi(SCRecordEvent);

        QMetaObject::connectSlotsByName(SCRecordEvent);
    } // setupUi

    void retranslateUi(QWidget *SCRecordEvent)
    {
        SCRecordEvent->setWindowTitle(QApplication::translate("SCRecordEvent", "Form", nullptr));
        toolButton->setText(QApplication::translate("SCRecordEvent", "\346\270\205\347\251\272\350\256\260\345\275\225", nullptr));
        toolButton_2->setText(QApplication::translate("SCRecordEvent", "\344\277\235\345\255\230\350\256\260\345\275\225", nullptr));
        toolButton_3->setText(QApplication::translate("SCRecordEvent", "\350\257\273\345\217\226\350\256\260\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCRecordEvent: public Ui_SCRecordEvent {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCRECORDEVENT_H
