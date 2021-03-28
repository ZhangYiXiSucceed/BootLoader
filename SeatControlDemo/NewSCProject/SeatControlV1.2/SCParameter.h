#ifndef SCPARAMETER_H
#define SCPARAMETER_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <synchapi.h>
#include <QDebug>
#include <QTimer>
#include <QPixmap>
#include <QIcon>
#include <QFile>
#include <QFileDialog>
#include <QSettings>

#include "CRC_Cal.h"

#define   Developer    0
#define CommandCount   10
#define CommandLength1  5
#define CommandLength2  6
#define CommandLength3  7
namespace Ui {
class SCParameter;
}

class SCParameter : public QWidget
{
    Q_OBJECT

public:
    explicit SCParameter(QWidget *parent = 0);
    ~SCParameter();
    void InitUI();
    void Refresh();
    void InitConnect();
    void CommandStringInit();
    void PrintFrameData(QByteArray ,uint16_t);
    uint8_t FindFrameHead(QByteArray ,uint16_t);
    bool IsCheckSumRight(QByteArray ,uint16_t);
    void RunCommand(QByteArray ,uint16_t &);
    void TransmitBinData(uint8_t);
    void MotorADCCommand();
    void SendMotorReverse();
    void SendMotorPositionReverse();
signals:
    void ShowMessage(QString,uint16_t);
    void ShowParameter(QString,QString);
    void SendProgressBar(uint8_t);
    void SendFirmwareInfo(uint8_t,QString);
    void SendButtonInfo(uint8_t,uint16_t);
    void DownloadFinish();
private slots:
    void ConnectComPort();
    void TimerUpdate();
    void ReadRecData();
    void WriteBytes(QByteArray);
    void RecFileAddress(QString,quint64);
    void CalibrateMode();
    void MotorSpeed();


    void MotorBackReverse();
    void MotorLegReverse();
    void MotorFootReverse();
    void MotorBackPositionReverse();
    void MotorLegPositionReverse();
    void MotorFootPositionReverse();

    void ShowMotorBackClosePosition();
    void ShowMotorBackOpenPosition();

    void ShowMotorLegClosePosition();
    void ShowMotorLegOpenPosition();

    void ShowMotorFootClosePosition();
    void ShowMotorFootOpenPosition();

    void ShowLinkMotorBackStart();
    void ShowLinkMotorBackEnd();
    void ShowLinkMotorBackMotorLegEnd();
    void ShowLinkMotorLegStart();
    void ShowLinkMotorLegEnd();
    void ShowLinkMotorBackHalf();
    void ShowLinkMotorLegHalf();
    void ShowLinkMotorFootHalf();

    void ReadPara();
    void SavePara();
    void WritePara();
    void LoadPara();
private:
    Ui::SCParameter *ui;
    QSerialPort     *MySerialPort     = nullptr;
    QTimer  *Timer                    = nullptr;
    quint8 SendCommandString[100][10];
    quint8 RecCommandString[100][10];

    QPixmap ImageFileOpen=tr(":/Image/Close.png");
    QPixmap ImageFileClose=tr(":/Image/Open.png");
    QPixmap ImageFileLoadPara=tr(":/Image/LoadPara.png");
    QPixmap ImageFileSavePara=tr(":/Image/SavePara.png");
    QPixmap ImageFileReadPara=tr(":/Image/ReadPara.png");
    QPixmap ImageFileWritePara=tr(":/Image/WritePara.png");

    QString FileAddress;
    QString IniFileAddress;
    quint64 BinSize;
    CRC_Cal CRC;
    uint8_t BinLoadCnt;

    uint8_t ConnectTimes;

    uint8_t ConnectFlag;
    uint8_t CalibrateFlag;

    bool IsMotorReverse[3]={false,false,false};
    bool IsMotorPositionReverse[3]={false,false,false};
};

#endif // SCPARAMETER_H
