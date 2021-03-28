#ifndef SCBUTTON_H
#define SCBUTTON_H

#include <QWidget>
#include <QTimer>


#define MotorLimitValue 10
namespace Ui {
class SCButton;
}

class SCButton : public QWidget
{
    Q_OBJECT

public:
    explicit SCButton(QWidget *parent = 0);
    ~SCButton();
    void InitConnect();
    void InitUI();
    void SendMotorOpeation();
    void SendMotorMeasure();
    bool MotorIsOpen();
    bool MotorIsClose();
    void RunOpenOrClose();
private slots:

    void MotorBackOpen();
    void MotorBackClose();
    void MotorBackStop();

    void MotorLegOpen();
    void MotorLegClose();
    void MotorLegStop();

    void MotorFootOpen();
    void MotorFootClose();
    void MotorFootStop();

    void FastOpen_Press();
    void FastOpen_Release();
    void FastClose_Press();
    void FastClose_Release();
    void Half_Press();
    void Half_Release();
    void OpenClose();


    void ReadCurrent();
    void ReadMotorPosition();

    void RecParaData(quint8,quint8,quint16);

    void TimerUpdate();
    void StartReadCurrent();

    void SelectCurrentWidget(int);
    void RecMotorPosition(QVector<quint16>);

signals:
    void ShowMessage(QString,uint16_t);
    void SendCommand(QByteArray);

private:
    Ui::SCButton *ui;
    QTimer    *MyTimer     =      nullptr;

    QPixmap ImageFileStart=tr(":/Image/Start.png");
    QPixmap ImageFileEnd=tr(":/Image/End.png");

    QPixmap ImageFileFastOpen=tr(":/Image/FastOpen.png");
    QPixmap ImageFileFastClose=tr(":/Image/FastClose.png");
    QPixmap ImageFileHalf=tr(":/Image/Half.png");
    QPixmap ImageFileOpenClose=tr(":/Image/OpenClose.png");
    QPixmap ImageFileStartReadCur=tr(":/Image/StartReadCur.png");
    QPixmap ImageFileStopReadCur=tr(":/Image/StopReadCur.png");

    quint16 MotorPosition[6]={0,0,0,0,0,0};

    bool MotorOperationFlag[20];  //虚拟按键操作标志位
    bool CurrentFlag;           //读取电流标志位
    bool ButtonFlag;            //按键按下标志位
    bool OpenCloseFlag;         //往复测试标志位
    bool MotorOpenFlag;
    bool MotorCloseFlag;

};

#endif // SCBUTTON_H
