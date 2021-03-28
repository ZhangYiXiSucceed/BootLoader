#include "SCButton.h"
#include "ui_SCButton.h"

SCButton::SCButton(QWidget *parent) :
    QWidget(parent),
    MyTimer(new QTimer(this)),
    ui(new Ui::SCButton)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

SCButton::~SCButton()
{
    delete ui;
}

void SCButton::InitUI()
{
    QFont Ft("Microsoft YaHei");
    Ft.setPointSize(12);

    ui->tbnMotorBackClose->setFont(Ft);
    ui->tbnMotorBackClose->setAutoRaise(true);
    ui->tbnMotorBackClose->setIcon(ImageFileStart);
    ui->tbnMotorBackClose->setIconSize(ImageFileStart.size());
    ui->tbnMotorBackClose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnMotorBackOpen->setFont(Ft);
    ui->tbnMotorBackOpen->setAutoRaise(true);
    ui->tbnMotorBackOpen->setIcon(ImageFileStart);
    ui->tbnMotorBackOpen->setIconSize(ImageFileStart.size());
    ui->tbnMotorBackOpen->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnMotorLegOpen->setFont(Ft);
    ui->tbnMotorLegOpen->setAutoRaise(true);
    ui->tbnMotorLegOpen->setIcon(ImageFileStart);
    ui->tbnMotorLegOpen->setIconSize(ImageFileStart.size());
    ui->tbnMotorLegOpen->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnMotorLegClose->setFont(Ft);
    ui->tbnMotorLegClose->setAutoRaise(true);
    ui->tbnMotorLegClose->setIcon(ImageFileStart);
    ui->tbnMotorLegClose->setIconSize(ImageFileStart.size());
    ui->tbnMotorLegClose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnMotorFootOpen->setFont(Ft);
    ui->tbnMotorFootOpen->setAutoRaise(true);
    ui->tbnMotorFootOpen->setIcon(ImageFileStart);
    ui->tbnMotorFootOpen->setIconSize(ImageFileStart.size());
    ui->tbnMotorFootOpen->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnMotorFootClose->setFont(Ft);
    ui->tbnMotorFootClose->setAutoRaise(true);
    ui->tbnMotorFootClose->setIcon(ImageFileStart);
    ui->tbnMotorFootClose->setIconSize(ImageFileStart.size());
    ui->tbnMotorFootClose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnFastOpen->setFont(Ft);
    ui->tbnFastOpen->setAutoRaise(true);
    ui->tbnFastOpen->setIcon(ImageFileFastOpen);
    ui->tbnFastOpen->setIconSize(ImageFileFastOpen.size());
    ui->tbnFastOpen->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnFastClose->setFont(Ft);
    ui->tbnFastClose->setAutoRaise(true);
    ui->tbnFastClose->setIcon(ImageFileFastClose);
    ui->tbnFastClose->setIconSize(ImageFileFastClose.size());
    ui->tbnFastClose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnHalf->setFont(Ft);
    ui->tbnHalf->setAutoRaise(true);
    ui->tbnHalf->setIcon(ImageFileHalf);
    ui->tbnHalf->setIconSize(ImageFileHalf.size());
    ui->tbnHalf->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnOpenCLose->setFont(Ft);
    ui->tbnOpenCLose->setAutoRaise(true);
    ui->tbnOpenCLose->setIcon(ImageFileOpenClose);
    ui->tbnOpenCLose->setIconSize(ImageFileOpenClose.size());
    ui->tbnOpenCLose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnReadCurrent->setFont(Ft);
    ui->tbnReadCurrent->setAutoRaise(true);
    ui->tbnReadCurrent->setIcon(ImageFileStartReadCur);
    ui->tbnReadCurrent->setIconSize(ImageFileStartReadCur.size());
    ui->tbnReadCurrent->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->leMotorBackCurrent->setFont(Ft);
    ui->leMotorFootCurrent->setFont(Ft);
    ui->leMotorLegCurrent->setFont(Ft);

    CurrentFlag=false;
    ButtonFlag=false;
}
void SCButton::InitConnect()
{
   connect(ui->tbnMotorBackOpen,SIGNAL(pressed()),this,SLOT(MotorBackOpen()));
   connect(ui->tbnMotorBackOpen,SIGNAL(released()),this,SLOT(MotorBackStop()));

   connect(ui->tbnMotorBackClose,SIGNAL(pressed()),this,SLOT(MotorBackClose()));
   connect(ui->tbnMotorBackClose,SIGNAL(released()),this,SLOT(MotorBackStop()));

   connect(ui->tbnMotorLegOpen,SIGNAL(pressed()),this,SLOT(MotorLegOpen()));
   connect(ui->tbnMotorLegOpen,SIGNAL(released()),this,SLOT(MotorLegStop()));

   connect(ui->tbnMotorLegClose,SIGNAL(pressed()),this,SLOT(MotorLegClose()));
   connect(ui->tbnMotorLegClose,SIGNAL(released()),this,SLOT(MotorLegStop()));

   connect(ui->tbnMotorFootOpen,SIGNAL(pressed()),this,SLOT(MotorFootOpen()));
   connect(ui->tbnMotorFootOpen,SIGNAL(released()),this,SLOT(MotorFootStop()));

   connect(ui->tbnMotorFootClose,SIGNAL(pressed()),this,SLOT(MotorFootClose()));
   connect(ui->tbnMotorFootClose,SIGNAL(released()),this,SLOT(MotorFootStop()));

   connect(ui->tbnReadCurrent,SIGNAL(clicked(bool)),this,SLOT(StartReadCurrent()));

   connect(MyTimer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));
}
void SCButton::MotorBackOpen()
{
    ui->tbnMotorBackOpen->setIcon(ImageFileEnd);
    ui->tbnMotorBackOpen->setText(tr("背靠停止"));
    MotorOperationFlag[0]=true;
    ButtonFlag=1;

}
void SCButton::MotorBackClose()
{
    ui->tbnMotorBackClose->setIcon(ImageFileEnd);
    ui->tbnMotorBackClose->setText(tr("背靠停止"));
    MotorOperationFlag[1]=true;
    ButtonFlag=1;

}
void SCButton::MotorBackStop()
{
    ui->tbnMotorBackOpen->setIcon(ImageFileStart);
    ui->tbnMotorBackOpen->setText(tr("背靠打开"));
    ui->tbnMotorBackClose->setIcon(ImageFileStart);
    ui->tbnMotorBackClose->setText(tr("背靠收起"));
    MotorOperationFlag[0]=false;
    MotorOperationFlag[1]=false;
    ButtonFlag=0;
//    QByteArray Command;
//    Command[0]=0x68;
//    Command[1]=0x32;
//    Command[2]=0x0C;
//    Command[3]=0x00;
//    Command[4]=0xA6;
//    emit SendCommand(Command);
}



void SCButton::MotorLegOpen()
{
    ui->tbnMotorLegOpen->setIcon(ImageFileEnd);
    ui->tbnMotorLegOpen->setText(tr("腿靠停止"));
    MotorOperationFlag[2]=true;
    ButtonFlag=1;

}
void SCButton::MotorLegClose()
{
    ui->tbnMotorLegClose->setIcon(ImageFileEnd);
    ui->tbnMotorLegClose->setText(tr("腿靠停止"));
    MotorOperationFlag[3]=true;
    ButtonFlag=1;
}
void SCButton::MotorLegStop()
{
    ui->tbnMotorLegOpen->setIcon(ImageFileStart);
    ui->tbnMotorLegOpen->setText(tr("腿靠打开"));
    ui->tbnMotorLegClose->setIcon(ImageFileStart);
    ui->tbnMotorLegClose->setText(tr("腿靠收起"));
    MotorOperationFlag[2]=false;
    MotorOperationFlag[3]=false;
    ButtonFlag=0;
//    QByteArray Command;
//    Command[0]=0x68;
//    Command[1]=0x35;
//    Command[2]=0x0C;
//    Command[3]=0x00;
//    Command[4]=0xA9;
//    emit SendCommand(Command);
}



void SCButton::MotorFootOpen()
{
    ui->tbnMotorFootOpen->setIcon(ImageFileEnd);
    ui->tbnMotorFootOpen->setText(tr("脚踏停止"));
    MotorOperationFlag[4]=true;
    ButtonFlag=1;
}
void SCButton::MotorFootClose()
{
    ui->tbnMotorFootClose->setIcon(ImageFileEnd);
    ui->tbnMotorFootClose->setText(tr("脚踏停止"));
    MotorOperationFlag[5]=true;
    ButtonFlag=1;
}
void SCButton::MotorFootStop()
{
    ui->tbnMotorFootOpen->setIcon(ImageFileStart);
    ui->tbnMotorFootOpen->setText(tr("脚踏打开"));
    ui->tbnMotorFootClose->setIcon(ImageFileStart);
    ui->tbnMotorFootClose->setText(tr("脚踏收起"));
    MotorOperationFlag[4]=false;
    MotorOperationFlag[5]=false;
    ButtonFlag=0;
//    QByteArray Command;
//    Command[0]=0x68;
//    Command[1]=0x38;
//    Command[2]=0x0C;
//    Command[3]=0x00;
//    Command[4]=0xAC;
//    emit SendCommand(Command);
}
void SCButton::SendMotorOpeation()
{
    if(MotorOperationFlag[0]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x30;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xA4;
        emit SendCommand(Command);
    }
    else if(MotorOperationFlag[1]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x31;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xA5;
        emit SendCommand(Command);
    }
    else if(MotorOperationFlag[2]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x33;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xA7;
        emit SendCommand(Command);
    }
    else if(MotorOperationFlag[3]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x34;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xA8;
        emit SendCommand(Command);
    }
    else if(MotorOperationFlag[4]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x36;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAA;
        emit SendCommand(Command);
    }
    else if(MotorOperationFlag[5]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x37;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAB;
        emit SendCommand(Command);
    }
}
void SCButton::ReadCurrent()
{
    QByteArray Command;
    Command[0]=0x68;
    Command[1]=0x1E;
    Command[2]=0x08;
    Command[3]=0x00;
    Command[4]=0x8E;

    Command[5]=0x68;
    Command[6]=0x1E;
    Command[7]=0x09;
    Command[8]=0x00;
    Command[9]=0x8F;

    Command[10]=0x68;
    Command[11]=0x1E;
    Command[12]=0x0A;
    Command[13]=0x00;
    Command[14]=0x90;
    emit SendCommand(Command);

}
void SCButton::RecParaData(uint8_t ControlCode,uint16_t MotorCurrent)
{
    if(ControlCode==0x00)
        ui->leMotorBackCurrent->setText(QString::number(MotorCurrent));
    else if(ControlCode==0x01)
        ui->leMotorLegCurrent->setText(QString::number(MotorCurrent));
    else if(ControlCode==0x02)
        ui->leMotorFootCurrent->setText(QString::number(MotorCurrent));
}
void SCButton::FastOpen()
{
    MotorOperationFlag[6]=true;
}
void SCButton::FastClose()
{
    MotorOperationFlag[7]=true;
}
void SCButton::OpenClose()
{
    MotorOperationFlag[8]=true;
}
void SCButton::Half()
{
    MotorOperationFlag[9]=true;
}
void SCButton::SendMotorMeasure()
{
    if(MotorOperationFlag[6]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x39;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAD;
        emit SendCommand(Command);
        MotorOperationFlag[6]=false;
    }
    if(MotorOperationFlag[7]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x3A;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAE;
        emit SendCommand(Command);
        MotorOperationFlag[7]=false;
    }
    if(MotorOperationFlag[8]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x3C;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xB0;
        emit SendCommand(Command);
        MotorOperationFlag[8]=false;
    }
    if(MotorOperationFlag[9]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x3B;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAF;
        emit SendCommand(Command);
        MotorOperationFlag[9]=false;
    }
}
void SCButton::TimerUpdate()
{
    if(CurrentFlag)
      ReadCurrent();
    if(ButtonFlag)
      SendMotorOpeation();
    SendMotorMeasure();
}
 void SCButton::StartReadCurrent()
 {
     static bool IsStartRead=false;
     IsStartRead=!IsStartRead;
     if(IsStartRead)
     {
       CurrentFlag=true;
       MyTimer->start(100);
       ui->tbnReadCurrent->setIcon(ImageFileStopReadCur);
     }
     else
     {
       CurrentFlag=false;
       MyTimer->stop();
       ui->tbnReadCurrent->setIcon(ImageFileStartReadCur);
     }
 }
