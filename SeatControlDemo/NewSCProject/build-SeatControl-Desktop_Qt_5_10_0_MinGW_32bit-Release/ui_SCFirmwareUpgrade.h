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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCFirmwareUpgrade
{
public:
    QGroupBox *groupBox;
    QToolButton *tbnOpenFile;
    QLineEdit *leFileAddress;
    QLabel *label;
    QLabel *label_2;
    QLabel *lbFirmwareSize;
    QLabel *lbDateTime;
    QGroupBox *groupBox_2;
    QLabel *leProgramState;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *leBoot;
    QLabel *label_9;
    QLabel *leFirmware;
    QLabel *label_11;
    QLabel *leHardware;
    QProgressBar *progressBar;
    QToolButton *tbnReadFirmwareInfo;
    QToolButton *tbnDownload;
    QToolButton *tbnEnterAPP;
    QToolButton *tbnReturnBootLoader;

    void setupUi(QWidget *SCFirmwareUpgrade)
    {
        if (SCFirmwareUpgrade->objectName().isEmpty())
            SCFirmwareUpgrade->setObjectName(QStringLiteral("SCFirmwareUpgrade"));
        SCFirmwareUpgrade->resize(661, 469);
        groupBox = new QGroupBox(SCFirmwareUpgrade);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 20, 311, 311));
        tbnOpenFile = new QToolButton(groupBox);
        tbnOpenFile->setObjectName(QStringLiteral("tbnOpenFile"));
        tbnOpenFile->setGeometry(QRect(10, 20, 91, 41));
        leFileAddress = new QLineEdit(groupBox);
        leFileAddress->setObjectName(QStringLiteral("leFileAddress"));
        leFileAddress->setGeometry(QRect(110, 19, 191, 41));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 80, 81, 21));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 110, 81, 21));
        lbFirmwareSize = new QLabel(groupBox);
        lbFirmwareSize->setObjectName(QStringLiteral("lbFirmwareSize"));
        lbFirmwareSize->setGeometry(QRect(90, 80, 181, 21));
        lbDateTime = new QLabel(groupBox);
        lbDateTime->setObjectName(QStringLiteral("lbDateTime"));
        lbDateTime->setGeometry(QRect(90, 110, 181, 21));
        groupBox_2 = new QGroupBox(SCFirmwareUpgrade);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(350, 20, 291, 251));
        leProgramState = new QLabel(groupBox_2);
        leProgramState->setObjectName(QStringLiteral("leProgramState"));
        leProgramState->setGeometry(QRect(90, 30, 151, 21));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 30, 71, 21));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 60, 71, 21));
        leBoot = new QLabel(groupBox_2);
        leBoot->setObjectName(QStringLiteral("leBoot"));
        leBoot->setGeometry(QRect(90, 60, 151, 21));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 90, 71, 21));
        leFirmware = new QLabel(groupBox_2);
        leFirmware->setObjectName(QStringLiteral("leFirmware"));
        leFirmware->setGeometry(QRect(90, 90, 151, 21));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(20, 120, 71, 21));
        leHardware = new QLabel(groupBox_2);
        leHardware->setObjectName(QStringLiteral("leHardware"));
        leHardware->setGeometry(QRect(90, 120, 151, 21));
        progressBar = new QProgressBar(SCFirmwareUpgrade);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(20, 360, 631, 23));
        progressBar->setValue(24);
        tbnReadFirmwareInfo = new QToolButton(SCFirmwareUpgrade);
        tbnReadFirmwareInfo->setObjectName(QStringLiteral("tbnReadFirmwareInfo"));
        tbnReadFirmwareInfo->setGeometry(QRect(360, 400, 121, 41));
        tbnDownload = new QToolButton(SCFirmwareUpgrade);
        tbnDownload->setObjectName(QStringLiteral("tbnDownload"));
        tbnDownload->setGeometry(QRect(520, 400, 121, 41));
        tbnEnterAPP = new QToolButton(SCFirmwareUpgrade);
        tbnEnterAPP->setObjectName(QStringLiteral("tbnEnterAPP"));
        tbnEnterAPP->setGeometry(QRect(160, 400, 111, 41));
        tbnReturnBootLoader = new QToolButton(SCFirmwareUpgrade);
        tbnReturnBootLoader->setObjectName(QStringLiteral("tbnReturnBootLoader"));
        tbnReturnBootLoader->setGeometry(QRect(30, 400, 111, 41));

        retranslateUi(SCFirmwareUpgrade);

        QMetaObject::connectSlotsByName(SCFirmwareUpgrade);
    } // setupUi

    void retranslateUi(QWidget *SCFirmwareUpgrade)
    {
        SCFirmwareUpgrade->setWindowTitle(QApplication::translate("SCFirmwareUpgrade", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\344\270\213\350\275\275", nullptr));
        tbnOpenFile->setText(QApplication::translate("SCFirmwareUpgrade", "\346\211\223\345\274\200", nullptr));
        label->setText(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\345\244\247\345\260\217\357\274\232", nullptr));
        label_2->setText(QApplication::translate("SCFirmwareUpgrade", "\344\270\213\350\275\275\346\227\245\346\234\237\357\274\232", nullptr));
        lbFirmwareSize->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        lbDateTime->setText(QApplication::translate("SCFirmwareUpgrade", "123456", nullptr));
        groupBox_2->setTitle(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\344\270\213\350\275\275", nullptr));
        leProgramState->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        label_5->setText(QApplication::translate("SCFirmwareUpgrade", "\350\277\220\350\241\214\347\212\266\346\200\201\357\274\232", nullptr));
        label_7->setText(QApplication::translate("SCFirmwareUpgrade", "Boot\347\211\210\346\234\254\357\274\232", nullptr));
        leBoot->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        label_9->setText(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\347\211\210\346\234\254\357\274\232", nullptr));
        leFirmware->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        label_11->setText(QApplication::translate("SCFirmwareUpgrade", "\347\241\254\344\273\266\347\211\210\346\234\254\357\274\232", nullptr));
        leHardware->setText(QApplication::translate("SCFirmwareUpgrade", "123", nullptr));
        tbnReadFirmwareInfo->setText(QApplication::translate("SCFirmwareUpgrade", "\350\257\273\345\217\226\345\233\272\344\273\266", nullptr));
        tbnDownload->setText(QApplication::translate("SCFirmwareUpgrade", "\345\233\272\344\273\266\344\270\213\350\275\275", nullptr));
        tbnEnterAPP->setText(QApplication::translate("SCFirmwareUpgrade", "\350\277\233\345\205\245APP", nullptr));
        tbnReturnBootLoader->setText(QApplication::translate("SCFirmwareUpgrade", "\350\277\224\345\233\236Boot", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCFirmwareUpgrade: public Ui_SCFirmwareUpgrade {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCFIRMWAREUPGRADE_H
