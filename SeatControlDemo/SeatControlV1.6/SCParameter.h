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
#include <QButtonGroup>

#include "CRC_Cal.h"

#define   Developer    0          //开发者模式（0）还是用户模式（1）

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
    void PrintFrameData(QByteArray ,uint16_t);
    uint8_t FindFrameHead(QByteArray ,uint16_t);
    bool IsCheckSumRight(QByteArray ,uint16_t);
    void RunCommand(QByteArray ,uint16_t &);
    void TransmitBinData(uint8_t);
    void MotorADCCommand();
    void SendMotorReverse();
    void SendMotorPositionReverse();
    void InquiryDirection();
    void ClearCalibrationData();
signals:
    void ShowMessage(QString,uint16_t);
    void ShowParameter(QString,QString);
    void SendProgressBar(uint8_t);
    void SendFirmwareInfo(uint8_t,QString);
    void SendButtonInfo(quint8,quint8,quint16);
    void DownloadFinish();
    void SendMotorPosition(QVector<quint16>);
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
    void SetClearCalibrationFlag();

    void TimerCloseResponse();

    void FrameRecDataProcess();

    void SelectCurrentWidget(int);
private:
    Ui::SCParameter *ui;
    QSerialPort     *MySerialPort     = nullptr;                //串口类
    QTimer  *Timer                    = nullptr;                //定时器类
    QButtonGroup   *bgGroup           = nullptr;                //按键组，负责按键的互斥
    QTimer  *MyTimer                  = nullptr;                //
    QByteArray   FrameRecData;

    QString FileAddress;            //串口升级下载文件地址
    QString IniFileAddress;         //保存读取参数的文件地址
    quint64 BinSize;                //升级文件字节数
    uint8_t BinLoadCnt;             //分包传输下载文件内容次数

    uint8_t ConnectTimes;           //连接端口次数，确定

    uint8_t ConnectFlag;            //连接标志位
    uint8_t CalibrateFlag;          //标定模式标志位

    bool IsMotorReverse[3]={false,false,false};                //是否发送电机反向的标志位
    bool IsMotorPositionReverse[3]={false,false,false};        //是否发送电位器反向的标志位

    bool InquiryDirectionFlag;                                 //询问是否反向的标志位
    bool ClearCalibrationDataFlag;                             //清空标记数据的标志位

    QPixmap ImageFileOpen=tr(":/Image/Close.png");
    QPixmap ImageFileClose=tr(":/Image/Open.png");
    QPixmap ImageFileLoadPara=tr(":/Image/LoadPara.png");
    QPixmap ImageFileSavePara=tr(":/Image/SavePara.png");
    QPixmap ImageFileReadPara=tr(":/Image/ReadPara.png");
    QPixmap ImageFileWritePara=tr(":/Image/WritePara.png");
    QPixmap ImageFileClearCalibrationData=tr(":/Image/ClearCalibrationData.png");
};

#endif // SCPARAMETER_H
