/********************************************************************************
** Form generated from reading UI file 'SCFirmwareUpgrade.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCFIRMWAREUPGRADE_H
#define UI_SCFIRMWAREUPGRADE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCFirmwareUpgrade
{
public:
    QGroupBox *groupBox;
    QToolButton *tbnOpenFile;
    QWidget *widget;
    QGridLayout *gridLayout_5;
    QLineEdit *leFileAddress;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QWidget *widget1;
    QGridLayout *gridLayout_6;
    QLabel *lbFirmwareSize;
    QLabel *label;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *verticalSpacer_8;
    QWidget *widget2;
    QGridLayout *gridLayout_7;
    QLabel *lbDateTime;
    QSpacerItem *verticalSpacer_9;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *verticalSpacer_10;
    QGroupBox *groupBox_2;
    QWidget *widget3;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_8;
    QLabel *leHardware;
    QLabel *label_11;
    QSpacerItem *verticalSpacer_17;
    QSpacerItem *verticalSpacer_18;
    QWidget *widget4;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_7;
    QLabel *leFirmware;
    QLabel *label_9;
    QSpacerItem *verticalSpacer_15;
    QSpacerItem *verticalSpacer_16;
    QWidget *widget5;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer_13;
    QSpacerItem *horizontalSpacer_6;
    QLabel *leBoot;
    QLabel *label_7;
    QSpacerItem *verticalSpacer_14;
    QWidget *widget6;
    QGridLayout *gridLayout_4;
    QSpacerItem *verticalSpacer_11;
    QSpacerItem *horizontalSpacer_5;
    QLabel *leProgramState;
    QLabel *label_5;
    QSpacerItem *verticalSpacer_12;
    QProgressBar *progressBar;
    QWidget *widget7;
    QGridLayout *gridLayout_8;
    QToolButton *tbnDownload;
    QToolButton *tbnReadFirmwareInfo;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_16;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QWidget *widget8;
    QGridLayout *gridLayout_9;
    QSpacerItem *horizontalSpacer_18;
    QSpacerItem *verticalSpacer_5;
    QToolButton *tbnEnterAPP;
    QToolButton *tbnReturnBootLoader;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *verticalSpacer_6;

    void setupUi(QWidget *SCFirmwareUpgrade)
    {
        if (SCFirmwareUpgrade->objectName().isEmpty())
            SCFirmwareUpgrade->setObjectName(QStringLiteral("SCFirmwareUpgrade"));
        SCFirmwareUpgrade->resize(1024, 650);
        groupBox = new QGroupBox(SCFirmwareUpgrade);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 20, 521, 451));
        tbnOpenFile = new QToolButton(groupBox);
        tbnOpenFile->setObjectName(QStringLiteral("tbnOpenFile"));
        tbnOpenFile->setGeometry(QRect(20, 20, 131, 51));
        widget = new QWidget(groupBox);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(170, 19, 331, 51));
        gridLayout_5 = new QGridLayout(widget);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        leFileAddress = new QLineEdit(widget);
        leFileAddress->setObjectName(QStringLiteral("leFileAddress"));

        gridLayout_5->addWidget(leFileAddress, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 2, 0, 1, 1);

        widget1 = new QWidget(groupBox);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(20, 83, 191, 34));
        gridLayout_6 = new QGridLayout(widget1);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        lbFirmwareSize = new QLabel(widget1);
        lbFirmwareSize->setObjectName(QStringLiteral("lbFirmwareSize"));

        gridLayout_6->addWidget(lbFirmwareSize, 1, 1, 1, 1);

        label = new QLabel(widget1);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_6->addWidget(label, 1, 0, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_7, 0, 1, 1, 1);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_13, 1, 2, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_8, 2, 1, 1, 1);

        widget2 = new QWidget(groupBox);
        widget2->setObjectName(QStringLiteral("widget2"));
        widget2->setGeometry(QRect(20, 130, 191, 34));
        gridLayout_7 = new QGridLayout(widget2);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        lbDateTime = new QLabel(widget2);
        lbDateTime->setObjectName(QStringLiteral("lbDateTime"));

        gridLayout_7->addWidget(lbDateTime, 1, 1, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_9, 0, 1, 1, 1);

        label_2 = new QLabel(widget2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_7->addWidget(label_2, 1, 0, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_12, 1, 2, 1, 1);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_10, 2, 1, 1, 1);

        groupBox_2 = new QGroupBox(SCFirmwareUpgrade);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(560, 20, 441, 451));
        widget3 = new QWidget(groupBox_2);
        widget3->setObjectName(QStringLiteral("widget3"));
        widget3->setGeometry(QRect(20, 170, 211, 34));
        gridLayout = new QGridLayout(widget3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_8, 1, 2, 1, 1);

        leHardware = new QLabel(widget3);
        leHardware->setObjectName(QStringLiteral("leHardware"));

        gridLayout->addWidget(leHardware, 1, 1, 1, 1);

        label_11 = new QLabel(widget3);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout->addWidget(label_11, 1, 0, 1, 1);

        verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_17, 0, 2, 1, 1);

        verticalSpacer_18 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_18, 2, 2, 1, 1);

        widget4 = new QWidget(groupBox_2);
        widget4->setObjectName(QStringLiteral("widget4"));
        widget4->setGeometry(QRect(20, 120, 211, 34));
        gridLayout_2 = new QGridLayout(widget4);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 1, 2, 1, 1);

        leFirmware = new QLabel(widget4);
        leFirmware->setObjectName(QStringLiteral("leFirmware"));

        gridLayout_2->addWidget(leFirmware, 1, 1, 1, 1);

        label_9 = new QLabel(widget4);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_2->addWidget(label_9, 1, 0, 1, 1);

        verticalSpacer_15 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_15, 0, 2, 1, 1);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_16, 2, 2, 1, 1);

        widget5 = new QWidget(groupBox_2);
        widget5->setObjectName(QStringLiteral("widget5"));
        widget5->setGeometry(QRect(20, 70, 211, 34));
        gridLayout_3 = new QGridLayout(widget5);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_13, 0, 2, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_6, 1, 2, 1, 1);

        leBoot = new QLabel(widget5);
        leBoot->setObjectName(QStringLiteral("leBoot"));

        gridLayout_3->addWidget(leBoot, 1, 1, 1, 1);

        label_7 = new QLabel(widget5);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_3->addWidget(label_7, 1, 0, 1, 1);

        verticalSpacer_14 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_14, 2, 2, 1, 1);

        widget6 = new QWidget(groupBox_2);
        widget6->setObjectName(QStringLiteral("widget6"));
        widget6->setGeometry(QRect(20, 20, 211, 34));
        gridLayout_4 = new QGridLayout(widget6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_11, 0, 2, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_5, 1, 2, 1, 1);

        leProgramState = new QLabel(widget6);
        leProgramState->setObjectName(QStringLiteral("leProgramState"));

        gridLayout_4->addWidget(leProgramState, 1, 1, 1, 1);

        label_5 = new QLabel(widget6);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_4->addWidget(label_5, 1, 0, 1, 1);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_12, 2, 2, 1, 1);

        progressBar = new QProgressBar(SCFirmwareUpgrade);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(20, 490, 981, 31));
        progressBar->setValue(24);
        widget7 = new QWidget(SCFirmwareUpgrade);
        widget7->setObjectName(QStringLiteral("widget7"));
        widget7->setGeometry(QRect(650, 550, 341, 41));
        gridLayout_8 = new QGridLayout(widget7);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        tbnDownload = new QToolButton(widget7);
        tbnDownload->setObjectName(QStringLiteral("tbnDownload"));

        gridLayout_8->addWidget(tbnDownload, 1, 3, 1, 1);

        tbnReadFirmwareInfo = new QToolButton(widget7);
        tbnReadFirmwareInfo->setObjectName(QStringLiteral("tbnReadFirmwareInfo"));

        gridLayout_8->addWidget(tbnReadFirmwareInfo, 1, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_4, 2, 3, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_16, 1, 4, 1, 1);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_15, 1, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_3, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer, 1, 2, 1, 1);

        widget8 = new QWidget(SCFirmwareUpgrade);
        widget8->setObjectName(QStringLiteral("widget8"));
        widget8->setGeometry(QRect(20, 550, 271, 41));
        gridLayout_9 = new QGridLayout(widget8);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_18, 1, 3, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_5, 0, 1, 1, 1);

        tbnEnterAPP = new QToolButton(widget8);
        tbnEnterAPP->setObjectName(QStringLiteral("tbnEnterAPP"));

        gridLayout_9->addWidget(tbnEnterAPP, 1, 2, 1, 1);

        tbnReturnBootLoader = new QToolButton(widget8);
        tbnReturnBootLoader->setObjectName(QStringLiteral("tbnReturnBootLoader"));

        gridLayout_9->addWidget(tbnReturnBootLoader, 1, 1, 1, 1);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_17, 1, 0, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_6, 2, 2, 1, 1);


        retranslateUi(SCFirmwareUpgrade);

        QMetaObject::connectSlotsByName(SCFirmwareUpgrade);
    } // setupUi

    void retranslateUi(QWidget *SCFirmwareUpgrade)
    {
        SCFirmwareUpgrade->setWindowTitle(QApplication::translate("SCFirmwareUpgrade", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\344\270\213\350\275\275", nullptr));
        tbnOpenFile->setText(QApplication::translate("SCFirmwareUpgrade", "\346\211\223\345\274\200", nullptr));
        lbFirmwareSize->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        label->setText(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\345\244\247\345\260\217\357\274\232", nullptr));
        lbDateTime->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        label_2->setText(QApplication::translate("SCFirmwareUpgrade", "\344\270\213\350\275\275\346\227\245\346\234\237\357\274\232", nullptr));
        groupBox_2->setTitle(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\344\270\213\350\275\275", nullptr));
        leHardware->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        label_11->setText(QApplication::translate("SCFirmwareUpgrade", "\347\241\254\344\273\266\347\211\210\346\234\254\357\274\232", nullptr));
        leFirmware->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        label_9->setText(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\347\211\210\346\234\254\357\274\232", nullptr));
        leBoot->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        label_7->setText(QApplication::translate("SCFirmwareUpgrade", "Boot\347\211\210\346\234\254\357\274\232", nullptr));
        leProgramState->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        label_5->setText(QApplication::translate("SCFirmwareUpgrade", "\350\277\220\350\241\214\347\212\266\346\200\201\357\274\232", nullptr));
        tbnDownload->setText(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\344\270\213\350\275\275", nullptr));
        tbnReadFirmwareInfo->setText(QApplication::translate("SCFirmwareUpgrade", "\350\257\273\345\217\226\345\233\272\344\273\266", nullptr));
        tbnEnterAPP->setText(QApplication::translate("SCFirmwareUpgrade", "\350\277\233\345\205\245APP", nullptr));
        tbnReturnBootLoader->setText(QApplication::translate("SCFirmwareUpgrade", "\350\277\224\345\233\236Boot", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCFirmwareUpgrade: public Ui_SCFirmwareUpgrade {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCFIRMWAREUPGRADE_H
