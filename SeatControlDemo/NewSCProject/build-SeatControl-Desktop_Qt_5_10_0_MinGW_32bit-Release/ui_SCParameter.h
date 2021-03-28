/********************************************************************************
** Form generated from reading UI file 'SCParameter.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCPARAMETER_H
#define UI_SCPARAMETER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCParameter
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *leMotorBackClosePosition;
    QLineEdit *leMotorBackOpenPosition;
    QLabel *label_2;
    QLineEdit *leMotorBackSpeed;
    QLabel *label_3;
    QCheckBox *cbMotorBackDirectionIsReverse;
    QCheckBox *cbMotorBackPositionIsReverse;
    QLabel *label_22;
    QRadioButton *rbnSelectMotorBackClose;
    QRadioButton *rbnSelectMotorBackOpen;
    QRadioButton *rbnSelectMotorBackSpeed;
    QToolButton *tbnConnect;
    QGroupBox *groupBox_5;
    QLineEdit *leMotorLegPosition;
    QLabel *label_21;
    QLabel *label_20;
    QLabel *label_16;
    QLineEdit *leMotorFootPosition;
    QLineEdit *leMotorBackPosition;
    QGroupBox *groupBox_2;
    QLabel *label_13;
    QLineEdit *leMotorLegClosePosition;
    QLineEdit *leMotorLegOpenPosition;
    QLabel *label_14;
    QLineEdit *leMotorLegSpeed;
    QLabel *label_15;
    QCheckBox *cbMotorLegDirectionIsReverse;
    QCheckBox *cbMotorLegPositionIsReverse;
    QRadioButton *rbnSelectMotorLegOpen;
    QRadioButton *rbnSelectMotorLegSpeed;
    QRadioButton *rbnSelectMotorLegClose;
    QGroupBox *groupBox_3;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *leMotorFootClosePosition;
    QLineEdit *leMotorFootOpenPosition;
    QLineEdit *leMotorFootSpeed;
    QLabel *label_19;
    QCheckBox *cbMotorFootPositionIsReverse;
    QCheckBox *cbMotorFootDirectionIsReverse;
    QRadioButton *rbnSelectMotorFootClose;
    QRadioButton *rbnSelectMotorFootSpeed;
    QRadioButton *rbnSelectMotorFootOpen;
    QGroupBox *groupBox_6;
    QLineEdit *leMotorBackStart;
    QLabel *label_23;
    QLabel *label_24;
    QLineEdit *leMotorBackEnd;
    QLabel *label_25;
    QLineEdit *leMotorBackMotorLegEnd;
    QLabel *label_26;
    QLineEdit *leMotorLegStart;
    QLabel *label_27;
    QLineEdit *leMotorLegEnd;
    QLabel *label_28;
    QLineEdit *leMotorBackHalf;
    QLineEdit *leMotorLegHalf;
    QLabel *label_29;
    QLineEdit *leMotorFootHalf;
    QLabel *label_30;
    QRadioButton *rbnSelectMotorBackStart;
    QRadioButton *rbnSelectMotorBackEnd;
    QRadioButton *rbnSelectMotorBackMotorLegEnd;
    QRadioButton *rbnSelectMotorLegStart;
    QRadioButton *rbnSelectMotorLegEnd;
    QRadioButton *rbnSelectMotorBackHalf;
    QRadioButton *rbnSelectMotorLegHalf;
    QRadioButton *rbnSelectMotorFootHalf;
    QRadioButton *rbnBlack;
    QToolButton *tbnLoadPara;
    QToolButton *tbnSavePara;
    QToolButton *tbnReadPara;
    QToolButton *tbnWritePara;
    QComboBox *comboBoxComPort;
    QCheckBox *cbCalibrate;

    void setupUi(QWidget *SCParameter)
    {
        if (SCParameter->objectName().isEmpty())
            SCParameter->setObjectName(QStringLiteral("SCParameter"));
        SCParameter->resize(661, 471);
        groupBox = new QGroupBox(SCParameter);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 80, 181, 191));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 71, 20));
        leMotorBackClosePosition = new QLineEdit(groupBox);
        leMotorBackClosePosition->setObjectName(QStringLiteral("leMotorBackClosePosition"));
        leMotorBackClosePosition->setGeometry(QRect(90, 20, 61, 21));
        leMotorBackOpenPosition = new QLineEdit(groupBox);
        leMotorBackOpenPosition->setObjectName(QStringLiteral("leMotorBackOpenPosition"));
        leMotorBackOpenPosition->setGeometry(QRect(90, 60, 61, 21));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 60, 71, 20));
        leMotorBackSpeed = new QLineEdit(groupBox);
        leMotorBackSpeed->setObjectName(QStringLiteral("leMotorBackSpeed"));
        leMotorBackSpeed->setGeometry(QRect(90, 100, 61, 21));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 100, 71, 20));
        label_3->setAlignment(Qt::AlignCenter);
        cbMotorBackDirectionIsReverse = new QCheckBox(groupBox);
        cbMotorBackDirectionIsReverse->setObjectName(QStringLiteral("cbMotorBackDirectionIsReverse"));
        cbMotorBackDirectionIsReverse->setGeometry(QRect(30, 130, 121, 21));
        cbMotorBackPositionIsReverse = new QCheckBox(groupBox);
        cbMotorBackPositionIsReverse->setObjectName(QStringLiteral("cbMotorBackPositionIsReverse"));
        cbMotorBackPositionIsReverse->setGeometry(QRect(30, 160, 121, 21));
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(0, 350, 71, 20));
        rbnSelectMotorBackClose = new QRadioButton(groupBox);
        rbnSelectMotorBackClose->setObjectName(QStringLiteral("rbnSelectMotorBackClose"));
        rbnSelectMotorBackClose->setGeometry(QRect(160, 20, 21, 21));
        rbnSelectMotorBackOpen = new QRadioButton(groupBox);
        rbnSelectMotorBackOpen->setObjectName(QStringLiteral("rbnSelectMotorBackOpen"));
        rbnSelectMotorBackOpen->setGeometry(QRect(160, 60, 21, 21));
        rbnSelectMotorBackSpeed = new QRadioButton(groupBox);
        rbnSelectMotorBackSpeed->setObjectName(QStringLiteral("rbnSelectMotorBackSpeed"));
        rbnSelectMotorBackSpeed->setGeometry(QRect(160, 100, 21, 21));
        tbnConnect = new QToolButton(SCParameter);
        tbnConnect->setObjectName(QStringLiteral("tbnConnect"));
        tbnConnect->setGeometry(QRect(10, 20, 101, 41));
        groupBox_5 = new QGroupBox(SCParameter);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(230, 10, 421, 61));
        leMotorLegPosition = new QLineEdit(groupBox_5);
        leMotorLegPosition->setObjectName(QStringLiteral("leMotorLegPosition"));
        leMotorLegPosition->setGeometry(QRect(200, 20, 61, 21));
        label_21 = new QLabel(groupBox_5);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(150, 10, 54, 41));
        label_20 = new QLabel(groupBox_5);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(280, 10, 54, 41));
        label_16 = new QLabel(groupBox_5);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(20, 10, 54, 41));
        leMotorFootPosition = new QLineEdit(groupBox_5);
        leMotorFootPosition->setObjectName(QStringLiteral("leMotorFootPosition"));
        leMotorFootPosition->setGeometry(QRect(330, 20, 61, 21));
        leMotorBackPosition = new QLineEdit(groupBox_5);
        leMotorBackPosition->setObjectName(QStringLiteral("leMotorBackPosition"));
        leMotorBackPosition->setGeometry(QRect(70, 20, 61, 21));
        groupBox_2 = new QGroupBox(SCParameter);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(230, 80, 181, 191));
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 20, 71, 20));
        leMotorLegClosePosition = new QLineEdit(groupBox_2);
        leMotorLegClosePosition->setObjectName(QStringLiteral("leMotorLegClosePosition"));
        leMotorLegClosePosition->setGeometry(QRect(90, 20, 61, 21));
        leMotorLegOpenPosition = new QLineEdit(groupBox_2);
        leMotorLegOpenPosition->setObjectName(QStringLiteral("leMotorLegOpenPosition"));
        leMotorLegOpenPosition->setGeometry(QRect(90, 60, 61, 21));
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(10, 60, 71, 20));
        leMotorLegSpeed = new QLineEdit(groupBox_2);
        leMotorLegSpeed->setObjectName(QStringLiteral("leMotorLegSpeed"));
        leMotorLegSpeed->setGeometry(QRect(90, 100, 61, 21));
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(10, 100, 71, 20));
        label_15->setAlignment(Qt::AlignCenter);
        cbMotorLegDirectionIsReverse = new QCheckBox(groupBox_2);
        cbMotorLegDirectionIsReverse->setObjectName(QStringLiteral("cbMotorLegDirectionIsReverse"));
        cbMotorLegDirectionIsReverse->setGeometry(QRect(30, 130, 121, 21));
        cbMotorLegPositionIsReverse = new QCheckBox(groupBox_2);
        cbMotorLegPositionIsReverse->setObjectName(QStringLiteral("cbMotorLegPositionIsReverse"));
        cbMotorLegPositionIsReverse->setGeometry(QRect(30, 160, 121, 21));
        rbnSelectMotorLegOpen = new QRadioButton(groupBox_2);
        rbnSelectMotorLegOpen->setObjectName(QStringLiteral("rbnSelectMotorLegOpen"));
        rbnSelectMotorLegOpen->setGeometry(QRect(160, 60, 21, 21));
        rbnSelectMotorLegSpeed = new QRadioButton(groupBox_2);
        rbnSelectMotorLegSpeed->setObjectName(QStringLiteral("rbnSelectMotorLegSpeed"));
        rbnSelectMotorLegSpeed->setGeometry(QRect(160, 100, 21, 21));
        rbnSelectMotorLegClose = new QRadioButton(groupBox_2);
        rbnSelectMotorLegClose->setObjectName(QStringLiteral("rbnSelectMotorLegClose"));
        rbnSelectMotorLegClose->setGeometry(QRect(160, 20, 21, 21));
        groupBox_3 = new QGroupBox(SCParameter);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(440, 80, 211, 191));
        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(20, 100, 71, 20));
        label_17->setAlignment(Qt::AlignCenter);
        label_18 = new QLabel(groupBox_3);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(20, 20, 71, 20));
        leMotorFootClosePosition = new QLineEdit(groupBox_3);
        leMotorFootClosePosition->setObjectName(QStringLiteral("leMotorFootClosePosition"));
        leMotorFootClosePosition->setGeometry(QRect(100, 20, 61, 21));
        leMotorFootOpenPosition = new QLineEdit(groupBox_3);
        leMotorFootOpenPosition->setObjectName(QStringLiteral("leMotorFootOpenPosition"));
        leMotorFootOpenPosition->setGeometry(QRect(100, 60, 61, 21));
        leMotorFootSpeed = new QLineEdit(groupBox_3);
        leMotorFootSpeed->setObjectName(QStringLiteral("leMotorFootSpeed"));
        leMotorFootSpeed->setGeometry(QRect(100, 100, 61, 21));
        label_19 = new QLabel(groupBox_3);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(20, 60, 71, 20));
        cbMotorFootPositionIsReverse = new QCheckBox(groupBox_3);
        cbMotorFootPositionIsReverse->setObjectName(QStringLiteral("cbMotorFootPositionIsReverse"));
        cbMotorFootPositionIsReverse->setGeometry(QRect(40, 160, 121, 21));
        cbMotorFootDirectionIsReverse = new QCheckBox(groupBox_3);
        cbMotorFootDirectionIsReverse->setObjectName(QStringLiteral("cbMotorFootDirectionIsReverse"));
        cbMotorFootDirectionIsReverse->setGeometry(QRect(40, 130, 121, 21));
        rbnSelectMotorFootClose = new QRadioButton(groupBox_3);
        rbnSelectMotorFootClose->setObjectName(QStringLiteral("rbnSelectMotorFootClose"));
        rbnSelectMotorFootClose->setGeometry(QRect(170, 20, 21, 21));
        rbnSelectMotorFootSpeed = new QRadioButton(groupBox_3);
        rbnSelectMotorFootSpeed->setObjectName(QStringLiteral("rbnSelectMotorFootSpeed"));
        rbnSelectMotorFootSpeed->setGeometry(QRect(170, 100, 21, 21));
        rbnSelectMotorFootOpen = new QRadioButton(groupBox_3);
        rbnSelectMotorFootOpen->setObjectName(QStringLiteral("rbnSelectMotorFootOpen"));
        rbnSelectMotorFootOpen->setGeometry(QRect(170, 60, 21, 21));
        groupBox_6 = new QGroupBox(SCParameter);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 280, 641, 121));
        leMotorBackStart = new QLineEdit(groupBox_6);
        leMotorBackStart->setObjectName(QStringLiteral("leMotorBackStart"));
        leMotorBackStart->setGeometry(QRect(110, 20, 71, 21));
        label_23 = new QLabel(groupBox_6);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(30, 20, 81, 20));
        label_24 = new QLabel(groupBox_6);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(30, 50, 81, 20));
        leMotorBackEnd = new QLineEdit(groupBox_6);
        leMotorBackEnd->setObjectName(QStringLiteral("leMotorBackEnd"));
        leMotorBackEnd->setGeometry(QRect(110, 50, 71, 21));
        label_25 = new QLabel(groupBox_6);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(10, 80, 101, 20));
        leMotorBackMotorLegEnd = new QLineEdit(groupBox_6);
        leMotorBackMotorLegEnd->setObjectName(QStringLiteral("leMotorBackMotorLegEnd"));
        leMotorBackMotorLegEnd->setGeometry(QRect(110, 80, 71, 21));
        label_26 = new QLabel(groupBox_6);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(230, 30, 101, 20));
        leMotorLegStart = new QLineEdit(groupBox_6);
        leMotorLegStart->setObjectName(QStringLiteral("leMotorLegStart"));
        leMotorLegStart->setGeometry(QRect(330, 30, 71, 21));
        label_27 = new QLabel(groupBox_6);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(230, 70, 101, 20));
        leMotorLegEnd = new QLineEdit(groupBox_6);
        leMotorLegEnd->setObjectName(QStringLiteral("leMotorLegEnd"));
        leMotorLegEnd->setGeometry(QRect(330, 70, 71, 21));
        label_28 = new QLabel(groupBox_6);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(440, 20, 81, 20));
        leMotorBackHalf = new QLineEdit(groupBox_6);
        leMotorBackHalf->setObjectName(QStringLiteral("leMotorBackHalf"));
        leMotorBackHalf->setGeometry(QRect(520, 20, 71, 21));
        leMotorLegHalf = new QLineEdit(groupBox_6);
        leMotorLegHalf->setObjectName(QStringLiteral("leMotorLegHalf"));
        leMotorLegHalf->setGeometry(QRect(520, 50, 71, 21));
        label_29 = new QLabel(groupBox_6);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setGeometry(QRect(440, 50, 81, 20));
        leMotorFootHalf = new QLineEdit(groupBox_6);
        leMotorFootHalf->setObjectName(QStringLiteral("leMotorFootHalf"));
        leMotorFootHalf->setGeometry(QRect(520, 80, 71, 21));
        label_30 = new QLabel(groupBox_6);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setGeometry(QRect(440, 80, 81, 20));
        rbnSelectMotorBackStart = new QRadioButton(groupBox_6);
        rbnSelectMotorBackStart->setObjectName(QStringLiteral("rbnSelectMotorBackStart"));
        rbnSelectMotorBackStart->setGeometry(QRect(190, 20, 21, 21));
        rbnSelectMotorBackEnd = new QRadioButton(groupBox_6);
        rbnSelectMotorBackEnd->setObjectName(QStringLiteral("rbnSelectMotorBackEnd"));
        rbnSelectMotorBackEnd->setGeometry(QRect(190, 50, 21, 21));
        rbnSelectMotorBackMotorLegEnd = new QRadioButton(groupBox_6);
        rbnSelectMotorBackMotorLegEnd->setObjectName(QStringLiteral("rbnSelectMotorBackMotorLegEnd"));
        rbnSelectMotorBackMotorLegEnd->setGeometry(QRect(190, 80, 21, 21));
        rbnSelectMotorLegStart = new QRadioButton(groupBox_6);
        rbnSelectMotorLegStart->setObjectName(QStringLiteral("rbnSelectMotorLegStart"));
        rbnSelectMotorLegStart->setGeometry(QRect(410, 30, 21, 21));
        rbnSelectMotorLegEnd = new QRadioButton(groupBox_6);
        rbnSelectMotorLegEnd->setObjectName(QStringLiteral("rbnSelectMotorLegEnd"));
        rbnSelectMotorLegEnd->setGeometry(QRect(410, 70, 21, 21));
        rbnSelectMotorBackHalf = new QRadioButton(groupBox_6);
        rbnSelectMotorBackHalf->setObjectName(QStringLiteral("rbnSelectMotorBackHalf"));
        rbnSelectMotorBackHalf->setGeometry(QRect(600, 20, 21, 21));
        rbnSelectMotorLegHalf = new QRadioButton(groupBox_6);
        rbnSelectMotorLegHalf->setObjectName(QStringLiteral("rbnSelectMotorLegHalf"));
        rbnSelectMotorLegHalf->setGeometry(QRect(600, 50, 21, 21));
        rbnSelectMotorFootHalf = new QRadioButton(groupBox_6);
        rbnSelectMotorFootHalf->setObjectName(QStringLiteral("rbnSelectMotorFootHalf"));
        rbnSelectMotorFootHalf->setGeometry(QRect(600, 80, 21, 21));
        rbnBlack = new QRadioButton(groupBox_6);
        rbnBlack->setObjectName(QStringLiteral("rbnBlack"));
        rbnBlack->setGeometry(QRect(320, 90, 21, 21));
        tbnLoadPara = new QToolButton(SCParameter);
        tbnLoadPara->setObjectName(QStringLiteral("tbnLoadPara"));
        tbnLoadPara->setGeometry(QRect(10, 420, 111, 31));
        tbnSavePara = new QToolButton(SCParameter);
        tbnSavePara->setObjectName(QStringLiteral("tbnSavePara"));
        tbnSavePara->setGeometry(QRect(150, 420, 111, 31));
        tbnReadPara = new QToolButton(SCParameter);
        tbnReadPara->setObjectName(QStringLiteral("tbnReadPara"));
        tbnReadPara->setGeometry(QRect(380, 420, 111, 31));
        tbnWritePara = new QToolButton(SCParameter);
        tbnWritePara->setObjectName(QStringLiteral("tbnWritePara"));
        tbnWritePara->setGeometry(QRect(540, 420, 101, 31));
        comboBoxComPort = new QComboBox(SCParameter);
        comboBoxComPort->setObjectName(QStringLiteral("comboBoxComPort"));
        comboBoxComPort->setGeometry(QRect(130, 20, 131, 22));
        cbCalibrate = new QCheckBox(SCParameter);
        cbCalibrate->setObjectName(QStringLiteral("cbCalibrate"));
        cbCalibrate->setGeometry(QRect(120, 20, 101, 41));

        retranslateUi(SCParameter);

        QMetaObject::connectSlotsByName(SCParameter);
    } // setupUi

    void retranslateUi(QWidget *SCParameter)
    {
        SCParameter->setWindowTitle(QApplication::translate("SCParameter", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("SCParameter", "\350\203\214\351\235\240\347\224\265\346\234\272\350\256\276\347\275\256", nullptr));
        label->setText(QApplication::translate("SCParameter", "\346\224\266\350\265\267\346\236\201\351\231\220\344\275\215\347\275\256", nullptr));
        label_2->setText(QApplication::translate("SCParameter", "\346\211\223\345\274\200\346\236\201\351\231\220\344\275\215\347\275\256", nullptr));
        label_3->setText(QApplication::translate("SCParameter", "\347\224\265\346\234\272\351\200\237\345\272\246", nullptr));
        cbMotorBackDirectionIsReverse->setText(QApplication::translate("SCParameter", "\347\224\265\346\234\272\346\230\257\345\220\246\345\217\215\345\220\221", nullptr));
        cbMotorBackPositionIsReverse->setText(QApplication::translate("SCParameter", "\347\224\265\344\275\215\345\231\250\346\230\257\345\220\246\345\217\215\345\220\221", nullptr));
        label_22->setText(QApplication::translate("SCParameter", "\346\224\266\350\265\267\346\236\201\351\231\220\344\275\215\347\275\256", nullptr));
        rbnSelectMotorBackClose->setText(QString());
        rbnSelectMotorBackOpen->setText(QString());
        rbnSelectMotorBackSpeed->setText(QString());
        tbnConnect->setText(QApplication::translate("SCParameter", "\350\277\236\346\216\245", nullptr));
        groupBox_5->setTitle(QApplication::translate("SCParameter", "\347\224\265\344\275\215\345\231\250\345\200\274", nullptr));
        label_21->setText(QApplication::translate("SCParameter", "\350\205\277\351\235\240\347\224\265\346\234\272", nullptr));
        label_20->setText(QApplication::translate("SCParameter", "\350\204\232\350\270\217\347\224\265\346\234\272", nullptr));
        label_16->setText(QApplication::translate("SCParameter", "\350\203\214\351\235\240\347\224\265\346\234\272", nullptr));
        groupBox_2->setTitle(QApplication::translate("SCParameter", "\350\205\277\351\235\240\347\224\265\346\234\272\350\256\276\347\275\256", nullptr));
        label_13->setText(QApplication::translate("SCParameter", "\346\224\266\350\265\267\346\236\201\351\231\220\344\275\215\347\275\256", nullptr));
        label_14->setText(QApplication::translate("SCParameter", "\346\211\223\345\274\200\346\236\201\351\231\220\344\275\215\347\275\256", nullptr));
        label_15->setText(QApplication::translate("SCParameter", "\347\224\265\346\234\272\351\200\237\345\272\246", nullptr));
        cbMotorLegDirectionIsReverse->setText(QApplication::translate("SCParameter", "\347\224\265\346\234\272\346\230\257\345\220\246\345\217\215\345\220\221", nullptr));
        cbMotorLegPositionIsReverse->setText(QApplication::translate("SCParameter", "\347\224\265\344\275\215\345\231\250\346\230\257\345\220\246\345\217\215\345\220\221", nullptr));
        rbnSelectMotorLegOpen->setText(QString());
        rbnSelectMotorLegSpeed->setText(QString());
        rbnSelectMotorLegClose->setText(QString());
        groupBox_3->setTitle(QApplication::translate("SCParameter", "\350\204\232\350\270\217\347\224\265\346\234\272", nullptr));
        label_17->setText(QApplication::translate("SCParameter", "\347\224\265\346\234\272\351\200\237\345\272\246", nullptr));
        label_18->setText(QApplication::translate("SCParameter", "\346\224\266\350\265\267\346\236\201\351\231\220\344\275\215\347\275\256", nullptr));
        label_19->setText(QApplication::translate("SCParameter", "\346\211\223\345\274\200\346\236\201\351\231\220\344\275\215\347\275\256", nullptr));
        cbMotorFootPositionIsReverse->setText(QApplication::translate("SCParameter", "\347\224\265\344\275\215\345\231\250\346\230\257\345\220\246\345\217\215\345\220\221", nullptr));
        cbMotorFootDirectionIsReverse->setText(QApplication::translate("SCParameter", "\347\224\265\346\234\272\346\230\257\345\220\246\345\217\215\345\220\221", nullptr));
        rbnSelectMotorFootClose->setText(QString());
        rbnSelectMotorFootSpeed->setText(QString());
        rbnSelectMotorFootOpen->setText(QString());
        groupBox_6->setTitle(QApplication::translate("SCParameter", "\350\201\224\345\212\250\347\202\271\350\256\276\347\275\256", nullptr));
        label_23->setText(QApplication::translate("SCParameter", "\350\203\214\351\235\240\345\274\200\345\247\213\344\275\215\347\275\256", nullptr));
        label_24->setText(QApplication::translate("SCParameter", "\350\203\214\351\235\240\347\273\223\346\235\237\344\275\215\347\275\256", nullptr));
        label_25->setText(QApplication::translate("SCParameter", "\350\203\214\351\235\240\350\205\277\351\235\240\347\273\223\346\235\237\344\275\215\347\275\256", nullptr));
        label_26->setText(QApplication::translate("SCParameter", "\350\205\277\351\235\240\350\204\232\350\270\217\345\274\200\345\247\213\344\275\215\347\275\256", nullptr));
        label_27->setText(QApplication::translate("SCParameter", "\350\205\277\351\235\240\350\204\232\350\270\217\347\273\223\346\235\237\344\275\215\347\275\256", nullptr));
        label_28->setText(QApplication::translate("SCParameter", "\345\215\212\350\272\272\350\203\214\351\235\240\344\275\215\347\275\256", nullptr));
        label_29->setText(QApplication::translate("SCParameter", "\345\215\212\350\272\272\350\205\277\351\235\240\344\275\215\347\275\256", nullptr));
        label_30->setText(QApplication::translate("SCParameter", "\345\215\212\350\272\272\350\204\232\350\270\217\344\275\215\347\275\256", nullptr));
        rbnSelectMotorBackStart->setText(QString());
        rbnSelectMotorBackEnd->setText(QString());
        rbnSelectMotorBackMotorLegEnd->setText(QString());
        rbnSelectMotorLegStart->setText(QString());
        rbnSelectMotorLegEnd->setText(QString());
        rbnSelectMotorBackHalf->setText(QString());
        rbnSelectMotorLegHalf->setText(QString());
        rbnSelectMotorFootHalf->setText(QString());
        rbnBlack->setText(QString());
        tbnLoadPara->setText(QApplication::translate("SCParameter", "\345\217\202\346\225\260\350\275\275\345\205\245", nullptr));
        tbnSavePara->setText(QApplication::translate("SCParameter", "\345\217\202\346\225\260\344\277\235\345\255\230", nullptr));
        tbnReadPara->setText(QApplication::translate("SCParameter", "\345\217\202\346\225\260\350\257\273\345\217\226", nullptr));
        tbnWritePara->setText(QApplication::translate("SCParameter", "\345\217\202\346\225\260\345\206\231\345\205\245", nullptr));
        cbCalibrate->setText(QApplication::translate("SCParameter", "\346\240\207\345\256\232\346\250\241\345\274\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCParameter: public Ui_SCParameter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCPARAMETER_H
