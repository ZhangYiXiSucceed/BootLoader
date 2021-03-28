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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCButton
{
public:
    QToolButton *tbnReadCurrent;
    QGroupBox *groupBox;
    QLabel *label_4;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_3;
    QToolButton *tbnOpenCLose;
    QSpacerItem *horizontalSpacer_6;
    QToolButton *tbnFastClose;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_3;
    QToolButton *tbnHalf;
    QSpacerItem *horizontalSpacer_5;
    QToolButton *tbnFastOpen;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *tbnMotorBackClose;
    QToolButton *tbnMotorLegClose;
    QToolButton *tbnMotorFootClose;
    QToolButton *tbnMotorFootOpen;
    QToolButton *tbnMotorBackOpen;
    QToolButton *tbnMotorLegOpen;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QLineEdit *leMotorFootCurrent;
    QLineEdit *leMotorLegCurrent;
    QLabel *label;
    QLineEdit *leMotorBackCurrent;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_3;
    QLabel *label_5;
    QWidget *widget;
    QGridLayout *gridLayout_4;
    QLineEdit *leMotorBackPosition;
    QLabel *label_8;
    QSpacerItem *verticalSpacer_5;
    QLineEdit *leMotorFootPosition;
    QSpacerItem *verticalSpacer_10;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_9;
    QSpacerItem *verticalSpacer_6;
    QLineEdit *leMotorLegPosition;
    QLabel *label_6;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_11;

    void setupUi(QWidget *SCButton)
    {
        if (SCButton->objectName().isEmpty())
            SCButton->setObjectName(QStringLiteral("SCButton"));
        SCButton->resize(1024, 650);
        tbnReadCurrent = new QToolButton(SCButton);
        tbnReadCurrent->setObjectName(QStringLiteral("tbnReadCurrent"));
        tbnReadCurrent->setGeometry(QRect(810, 20, 171, 41));
        groupBox = new QGroupBox(SCButton);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 240, 821, 381));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 10, 811, 361));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/Image/ControlBoard.png")));
        groupBox_2 = new QGroupBox(SCButton);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 70, 971, 141));
        layoutWidget = new QWidget(groupBox_2);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(110, 80, 731, 51));
        gridLayout_3 = new QGridLayout(layoutWidget);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        tbnOpenCLose = new QToolButton(layoutWidget);
        tbnOpenCLose->setObjectName(QStringLiteral("tbnOpenCLose"));

        gridLayout_3->addWidget(tbnOpenCLose, 1, 5, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_6, 1, 6, 1, 1);

        tbnFastClose = new QToolButton(layoutWidget);
        tbnFastClose->setObjectName(QStringLiteral("tbnFastClose"));

        gridLayout_3->addWidget(tbnFastClose, 1, 2, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_4, 2, 3, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 0, 3, 1, 1);

        tbnHalf = new QToolButton(layoutWidget);
        tbnHalf->setObjectName(QStringLiteral("tbnHalf"));

        gridLayout_3->addWidget(tbnHalf, 1, 3, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 1, 0, 1, 1);

        tbnFastOpen = new QToolButton(layoutWidget);
        tbnFastOpen->setObjectName(QStringLiteral("tbnFastOpen"));

        gridLayout_3->addWidget(tbnFastOpen, 1, 4, 1, 1);

        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 20, 921, 51));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 1, 9, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 1, 0, 1, 1);

        tbnMotorBackClose = new QToolButton(layoutWidget1);
        tbnMotorBackClose->setObjectName(QStringLiteral("tbnMotorBackClose"));

        gridLayout_2->addWidget(tbnMotorBackClose, 1, 2, 1, 1);

        tbnMotorLegClose = new QToolButton(layoutWidget1);
        tbnMotorLegClose->setObjectName(QStringLiteral("tbnMotorLegClose"));

        gridLayout_2->addWidget(tbnMotorLegClose, 1, 5, 1, 1);

        tbnMotorFootClose = new QToolButton(layoutWidget1);
        tbnMotorFootClose->setObjectName(QStringLiteral("tbnMotorFootClose"));

        gridLayout_2->addWidget(tbnMotorFootClose, 1, 7, 1, 1);

        tbnMotorFootOpen = new QToolButton(layoutWidget1);
        tbnMotorFootOpen->setObjectName(QStringLiteral("tbnMotorFootOpen"));

        gridLayout_2->addWidget(tbnMotorFootOpen, 1, 8, 1, 1);

        tbnMotorBackOpen = new QToolButton(layoutWidget1);
        tbnMotorBackOpen->setObjectName(QStringLiteral("tbnMotorBackOpen"));

        gridLayout_2->addWidget(tbnMotorBackOpen, 1, 3, 1, 1);

        tbnMotorLegOpen = new QToolButton(layoutWidget1);
        tbnMotorLegOpen->setObjectName(QStringLiteral("tbnMotorLegOpen"));

        gridLayout_2->addWidget(tbnMotorLegOpen, 1, 6, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 2, 5, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 0, 5, 1, 1);

        layoutWidget2 = new QWidget(SCButton);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(20, 10, 771, 51));
        gridLayout = new QGridLayout(layoutWidget2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 5, 1, 1);

        label_2 = new QLabel(layoutWidget2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        leMotorFootCurrent = new QLineEdit(layoutWidget2);
        leMotorFootCurrent->setObjectName(QStringLiteral("leMotorFootCurrent"));

        gridLayout->addWidget(leMotorFootCurrent, 0, 6, 1, 1);

        leMotorLegCurrent = new QLineEdit(layoutWidget2);
        leMotorLegCurrent->setObjectName(QStringLiteral("leMotorLegCurrent"));

        gridLayout->addWidget(leMotorLegCurrent, 0, 4, 1, 1);

        label = new QLabel(layoutWidget2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        leMotorBackCurrent = new QLineEdit(layoutWidget2);
        leMotorBackCurrent->setObjectName(QStringLiteral("leMotorBackCurrent"));

        gridLayout->addWidget(leMotorBackCurrent, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 7, 1, 1);

        groupBox_3 = new QGroupBox(SCButton);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(840, 240, 171, 241));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 20, 54, 12));
        widget = new QWidget(groupBox_3);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 25, 151, 191));
        gridLayout_4 = new QGridLayout(widget);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        leMotorBackPosition = new QLineEdit(widget);
        leMotorBackPosition->setObjectName(QStringLiteral("leMotorBackPosition"));

        gridLayout_4->addWidget(leMotorBackPosition, 3, 1, 1, 1);

        label_8 = new QLabel(widget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_4->addWidget(label_8, 1, 1, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_5, 12, 1, 1, 1);

        leMotorFootPosition = new QLineEdit(widget);
        leMotorFootPosition->setObjectName(QStringLiteral("leMotorFootPosition"));

        gridLayout_4->addWidget(leMotorFootPosition, 11, 1, 1, 1);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_10, 6, 1, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_7, 4, 1, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_8, 8, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_7, 3, 0, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_9, 2, 1, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_6, 0, 1, 1, 1);

        leMotorLegPosition = new QLineEdit(widget);
        leMotorLegPosition->setObjectName(QStringLiteral("leMotorLegPosition"));

        gridLayout_4->addWidget(leMotorLegPosition, 7, 1, 1, 1);

        label_6 = new QLabel(widget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_4->addWidget(label_6, 5, 1, 1, 1);

        label_7 = new QLabel(widget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_4->addWidget(label_7, 9, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_8, 9, 2, 1, 1);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_11, 10, 1, 1, 1);


        retranslateUi(SCButton);

        QMetaObject::connectSlotsByName(SCButton);
    } // setupUi

    void retranslateUi(QWidget *SCButton)
    {
        SCButton->setWindowTitle(QApplication::translate("SCButton", "Form", nullptr));
        tbnReadCurrent->setText(QApplication::translate("SCButton", "\350\257\273\345\217\226\345\217\202\346\225\260", nullptr));
        groupBox->setTitle(QApplication::translate("SCButton", "\345\272\247\346\244\205\347\244\272\346\204\217\345\233\276", nullptr));
        label_4->setText(QString());
        groupBox_2->setTitle(QApplication::translate("SCButton", "\345\272\247\346\244\205\346\216\247\345\210\266", nullptr));
        tbnOpenCLose->setText(QApplication::translate("SCButton", "\345\276\200\345\244\215\346\265\213\350\257\225", nullptr));
        tbnFastClose->setText(QApplication::translate("SCButton", "K7", nullptr));
        tbnHalf->setText(QApplication::translate("SCButton", "K8", nullptr));
        tbnFastOpen->setText(QApplication::translate("SCButton", "K9", nullptr));
        tbnMotorBackClose->setText(QApplication::translate("SCButton", "K1", nullptr));
        tbnMotorLegClose->setText(QApplication::translate("SCButton", "K3", nullptr));
        tbnMotorFootClose->setText(QApplication::translate("SCButton", "K5", nullptr));
        tbnMotorFootOpen->setText(QApplication::translate("SCButton", "K6", nullptr));
        tbnMotorBackOpen->setText(QApplication::translate("SCButton", "K2", nullptr));
        tbnMotorLegOpen->setText(QApplication::translate("SCButton", "K4", nullptr));
        label_3->setText(QApplication::translate("SCButton", "\350\204\232\350\270\217\347\224\265\346\234\272\347\224\265\346\265\201", nullptr));
        label_2->setText(QApplication::translate("SCButton", "\350\205\277\351\235\240\347\224\265\346\234\272\347\224\265\346\265\201", nullptr));
        label->setText(QApplication::translate("SCButton", "\350\203\214\351\235\240\347\224\265\346\234\272\347\224\265\346\265\201", nullptr));
        groupBox_3->setTitle(QApplication::translate("SCButton", "\347\224\265\346\234\272\344\275\215\347\275\256", nullptr));
        label_5->setText(QString());
        label_8->setText(QApplication::translate("SCButton", "\350\203\214\351\235\240\347\224\265\346\234\272", nullptr));
        leMotorFootPosition->setText(QString());
        label_6->setText(QApplication::translate("SCButton", "\350\205\277\351\235\240\347\224\265\346\234\272", nullptr));
        label_7->setText(QApplication::translate("SCButton", "\350\204\232\350\270\217\347\224\265\346\234\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCButton: public Ui_SCButton {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCBUTTON_H
