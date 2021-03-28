#ifndef SCBUTTON_H
#define SCBUTTON_H

#include <QWidget>
#include <QTimer>

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

    void FastOpen();
    void FastClose();
    void OpenClose();
    void Half();

    void ReadCurrent();

    void RecParaData(uint8_t,uint16_t);

    void TimerUpdate();
    void StartReadCurrent();

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

    bool MotorOperationFlag[10]={false,false,false,false,false,false,false,false,false,false};
    bool CurrentFlag;
    bool ButtonFlag;
};

#endif // SCBUTTON_H
