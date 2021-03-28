/************************************************************************************************************
界面名：按键模拟界面
功能：模拟按键来运行电机，有一键指令和读取电流功能
************************************************************************************************************/

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
    delete MyTimer;
    delete ui;

}
/************************************************************************************************************
函数名：InitUI()
参数：无
函数功能：负责按键模拟界面的初始化工作，包括按键的图标、字体和标志位等。
返回值：无
************************************************************************************************************/
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

    ui->leMotorBackPosition->setFont(Ft);
    ui->leMotorLegPosition->setFont(Ft);
    ui->leMotorFootPosition->setFont(Ft);

    CurrentFlag=false;
    ButtonFlag=false;
    OpenCloseFlag=false;
    MotorOpenFlag=false;
    MotorCloseFlag=false;

    for(quint8 i=0;i<20;i++)
        MotorOperationFlag[i]=false;
}
/************************************************************************************************************
函数名：InitConnect()
参数：无
函数功能：负责该TabWidget按键模拟界面 信号与槽的连接。
返回值：无
************************************************************************************************************/
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

   connect(ui->tbnFastOpen,SIGNAL(pressed()),this,SLOT(FastOpen_Press()));
   connect(ui->tbnFastOpen,SIGNAL(released()),this,SLOT(FastOpen_Release()));

   connect(ui->tbnFastClose,SIGNAL(pressed()),this,SLOT(FastClose_Press()));
   connect(ui->tbnFastClose,SIGNAL(released()),this,SLOT(FastClose_Release()));

   connect(ui->tbnHalf,SIGNAL(pressed()),this,SLOT(Half_Press()));
   connect(ui->tbnHalf,SIGNAL(released()),this,SLOT(Half_Release()));

   connect(ui->tbnOpenCLose,SIGNAL(clicked(bool)),this,SLOT(OpenClose()));

   connect(MyTimer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));
}
/************************************************************************************************************
函数名：MotorBackOpen()
参数：无
函数功能：设置背靠电机的打开标志位
返回值：无
************************************************************************************************************/
void SCButton::MotorBackOpen()
{
    ui->tbnMotorBackOpen->setIcon(ImageFileEnd);
    //ui->tbnMotorBackOpen->setText(tr("背靠停止"));
    MotorOperationFlag[0]=true;
    MotorOperationFlag[2]=false;
    ButtonFlag=1;


}
/************************************************************************************************************
函数名：MotorBackClose()
参数：无
函数功能：设置背靠电机的收起标志位
返回值：无
************************************************************************************************************/
void SCButton::MotorBackClose()
{
    ui->tbnMotorBackClose->setIcon(ImageFileEnd);
    //ui->tbnMotorBackClose->setText(tr("背靠停止"));
    MotorOperationFlag[1]=true;
    MotorOperationFlag[2]=false;
    ButtonFlag=1;
}
/************************************************************************************************************
函数名：MotorBackStop()
参数：无
函数功能：设置背靠电机的停止标志位
返回值：无
************************************************************************************************************/
void SCButton::MotorBackStop()
{
    ui->tbnMotorBackOpen->setIcon(ImageFileStart);
    //ui->tbnMotorBackOpen->setText(tr("背靠打开"));
    ui->tbnMotorBackClose->setIcon(ImageFileStart);
    //->tbnMotorBackClose->setText(tr("背靠收起"));
    MotorOperationFlag[0]=false;
    MotorOperationFlag[1]=false;
    MotorOperationFlag[2]=true;
//    QByteArray Command;
//    Command[0]=0x68;
//    Command[1]=0x32;
//    Command[2]=0x0C;
//    Command[3]=0x00;
//    Command[4]=0xA6;
//    emit SendCommand(Command);
}


/************************************************************************************************************
函数名：MotorLegOpen()
参数：无
函数功能：设置腿靠电机的打开标志位
返回值：无
************************************************************************************************************/
void SCButton::MotorLegOpen()
{
    ui->tbnMotorLegOpen->setIcon(ImageFileEnd);
    //ui->tbnMotorLegOpen->setText(tr("腿靠停止"));
    MotorOperationFlag[3]=true;
    MotorOperationFlag[5]=false;
    ButtonFlag=1;

}
/************************************************************************************************************
函数名：MotorLegClose()
参数：无
函数功能：设置腿靠电机的收起标志位
返回值：无
************************************************************************************************************/
void SCButton::MotorLegClose()
{
    ui->tbnMotorLegClose->setIcon(ImageFileEnd);
   // ui->tbnMotorLegClose->setText(tr("腿靠停止"));
    MotorOperationFlag[4]=true;
    MotorOperationFlag[5]=false;
    ButtonFlag=1;
}
/************************************************************************************************************
函数名：MotorLegStop()
参数：无
函数功能：设置腿靠电机的停止标志位
返回值：无
************************************************************************************************************/
void SCButton::MotorLegStop()
{
    ui->tbnMotorLegOpen->setIcon(ImageFileStart);
    //ui->tbnMotorLegOpen->setText(tr("腿靠打开"));
    ui->tbnMotorLegClose->setIcon(ImageFileStart);
    //ui->tbnMotorLegClose->setText(tr("腿靠收起"));
    MotorOperationFlag[3]=false;
    MotorOperationFlag[4]=false;
    MotorOperationFlag[5]=true;
//    QByteArray Command;
//    Command[0]=0x68;
//    Command[1]=0x35;
//    Command[2]=0x0C;
//    Command[3]=0x00;
//    Command[4]=0xA9;
//    emit SendCommand(Command);
}
/************************************************************************************************************
函数名：MotorFootOpen()
参数：无
函数功能：设置脚踏电机的打开标志位
返回值：无
************************************************************************************************************/
void SCButton::MotorFootOpen()
{
    ui->tbnMotorFootOpen->setIcon(ImageFileEnd);
    //ui->tbnMotorFootOpen->setText(tr("脚踏停止"));
    MotorOperationFlag[6]=true;
    MotorOperationFlag[8]=false;
    ButtonFlag=1;
}
/************************************************************************************************************
函数名：MotorFootClose()
参数：无
函数功能：设置脚踏电机的收起标志位
返回值：无
************************************************************************************************************/
void SCButton::MotorFootClose()
{
    ui->tbnMotorFootClose->setIcon(ImageFileEnd);
    //ui->tbnMotorFootClose->setText(tr("脚踏停止"));
    MotorOperationFlag[7]=true;
    MotorOperationFlag[8]=false;
    ButtonFlag=1;
}
/************************************************************************************************************
函数名：MotorFootStop()
参数：无
函数功能：设置脚踏电机的停止标志位
返回值：无
************************************************************************************************************/
void SCButton::MotorFootStop()
{
    ui->tbnMotorFootOpen->setIcon(ImageFileStart);
    //ui->tbnMotorFootOpen->setText(tr("脚踏打开"));
    ui->tbnMotorFootClose->setIcon(ImageFileStart);
    //ui->tbnMotorFootClose->setText(tr("脚踏收起"));
    MotorOperationFlag[6]=false;
    MotorOperationFlag[7]=false;
    MotorOperationFlag[8]=true;
}
/************************************************************************************************************
函数名：SendMotorOpeation()
参数：无
函数功能：发送电机操作指令，一键指令
返回值：无
************************************************************************************************************/
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
        Command[1]=0x32;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xA6;
        emit SendCommand(Command);
        MotorOperationFlag[2]=false;
    }
    else if(MotorOperationFlag[3]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x33;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xA7;
        emit SendCommand(Command);
    }
    else if(MotorOperationFlag[4]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x34;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xA8;
        emit SendCommand(Command);
    }
    else if(MotorOperationFlag[5]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x35;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xA9;
        emit SendCommand(Command);
        MotorOperationFlag[5]=false;
    }
    else if(MotorOperationFlag[6]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x36;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAA;
        emit SendCommand(Command);
    }
    else if(MotorOperationFlag[7]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x37;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAB;
        emit SendCommand(Command);
    }
    else if(MotorOperationFlag[8]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x38;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAC;
        emit SendCommand(Command);
        MotorOperationFlag[8]=false;
    }
}
/************************************************************************************************************
函数名：ReadCurrent()
参数：无
函数功能：读电机电流指令
返回值：无
************************************************************************************************************/
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
void SCButton::ReadMotorPosition()
{
    QByteArray Command;
    Command.resize(15);

    Command[0]=0x68;
    Command[1]=0x1D;
    Command[2]=0x08;
    Command[3]=0x00;
    Command[4]=0x8D;

    Command[5]=0x68;
    Command[6]=0x1D;
    Command[7]=0x09;
    Command[8]=0x00;
    Command[9]=0x8E;

    Command[10]=0x68;
    Command[11]=0x1D;
    Command[12]=0x0A;
    Command[13]=0x00;
    Command[14]=0x8F;

    emit SendCommand(Command);
}
/************************************************************************************************************
函数名：RecParaData(uint8_t ControlCode,uint16_t MotorCurrent)
参数：uint8_t ControlCode,uint16_t MotorCurrent
函数功能：接收到电流数据
返回值：无
************************************************************************************************************/
void SCButton::RecParaData(quint8 FunctionCode,quint8 ControlCode,quint16 MotorParameter)
{
    if(FunctionCode==0x1E)
    {
        if(ControlCode==0x00)
            ui->leMotorBackCurrent->setText(QString::number(MotorParameter));
        else if(ControlCode==0x01)
            ui->leMotorLegCurrent->setText(QString::number(MotorParameter));
        else if(ControlCode==0x02)
            ui->leMotorFootCurrent->setText(QString::number(MotorParameter));
    }
    else if(FunctionCode==0x1D)
    {
        if(ControlCode==0x00)
            ui->leMotorBackPosition->setText(QString::number(MotorParameter));
        else if(ControlCode==0x01)
            ui->leMotorLegPosition->setText(QString::number(MotorParameter));
        else if(ControlCode==0x02)
            ui->leMotorFootPosition->setText(QString::number(MotorParameter));
    }
}
/************************************************************************************************************
函数名：FastOpen()，FastClose()，OpenClose()，Half()
参数：无
函数功能：设置一键指令标志位
返回值：无
************************************************************************************************************/
void SCButton::FastOpen_Press()
{
    MotorOperationFlag[9]=true;
}
void SCButton::FastOpen_Release()
{
    MotorOperationFlag[10]=true;
}
void SCButton::FastClose_Press()
{
    MotorOperationFlag[11]=true;
}
void SCButton::FastClose_Release()
{
    MotorOperationFlag[12]=true;
}
void SCButton::Half_Press()
{
    MotorOperationFlag[13]=true;
}
void SCButton::Half_Release()
{
    MotorOperationFlag[14]=true;
}
void SCButton::OpenClose()
{
    MotorOperationFlag[15]=true;
}

/************************************************************************************************************
函数名：SendMotorMeasure()
参数：无
函数功能：电机一键操作指令
返回值：无
************************************************************************************************************/
void SCButton::SendMotorMeasure()
{
    if(MotorOperationFlag[9]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x39;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAD;
        emit SendCommand(Command);
        MotorOperationFlag[9]=false;
    }
    if(MotorOperationFlag[10]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x3A;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAE;
        emit SendCommand(Command);
        MotorOperationFlag[10]=false;
    }
    if(MotorOperationFlag[11]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x3B;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xAF;
        emit SendCommand(Command);

        MotorOperationFlag[11]=false;
    }
    if(MotorOperationFlag[12]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x3C;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xB0;
        emit SendCommand(Command);

        MotorOperationFlag[12]=false;
    }
    if(MotorOperationFlag[13]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x3D;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xB1;
        emit SendCommand(Command);

        MotorOperationFlag[13]=false;
    }
    if(MotorOperationFlag[14]==true)
    {
        QByteArray Command;
        Command[0]=0x68;
        Command[1]=0x3E;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0xB2;
        emit SendCommand(Command);

        MotorOperationFlag[14]=false;
    }
    if(MotorOperationFlag[15]==true)
    {
       OpenCloseFlag=!OpenCloseFlag;
       MotorOperationFlag[15]=false;

       MotorOperationFlag[11]=true;
       MotorOperationFlag[12]=true;
    }
}
void SCButton::RunOpenOrClose()
{
    if(OpenCloseFlag)
    {
        if((MotorIsOpen())&&(MotorOpenFlag==false))
        {
            MotorOperationFlag[11]=true;
            MotorOperationFlag[12]=true;
            MotorOpenFlag=true;
        }
        else
        {

        }
        if((MotorIsClose())&&(MotorCloseFlag==false))
        {
            MotorOperationFlag[9]=true;
            MotorOperationFlag[10]=true;
            MotorCloseFlag=true;
        }
        else
        {

        }
    }
}
void SCButton::RecMotorPosition(QVector<quint16> RecMotorPosition)
{
    for(quint8 i=0;i<6;i++)
        MotorPosition[i]=RecMotorPosition[i];
}
bool SCButton::MotorIsOpen()
{
   quint16 TempMotorBackOpen,TempMotorLegOpen,TempMotorFootOpen;
   TempMotorBackOpen=ui->leMotorBackPosition->text().toInt();
   TempMotorLegOpen=ui->leMotorLegPosition->text().toInt();
   TempMotorFootOpen=ui->leMotorFootPosition->text().toInt();

   if(abs(TempMotorBackOpen-MotorPosition[1])<MotorLimitValue)
   {
       if(abs(TempMotorLegOpen-MotorPosition[3])<MotorLimitValue)
       {
           if(abs(TempMotorFootOpen-MotorPosition[5])<MotorLimitValue)
           {
               return true;
           }
           else
           {
               return false;
           }
       }
       else
       {
            return false;
       }
   }
   else
   {
        return false;
   }
}
bool SCButton::MotorIsClose()
{
    quint16 TempMotorBackClose,TempMotorLegClose,TempMotorFootClose;
    TempMotorBackClose=ui->leMotorBackPosition->text().toInt();
    TempMotorLegClose=ui->leMotorLegPosition->text().toInt();
    TempMotorFootClose=ui->leMotorFootPosition->text().toInt();

    if(abs(TempMotorBackClose-MotorPosition[0])<MotorLimitValue)
    {
        if(abs(TempMotorLegClose-MotorPosition[2])<MotorLimitValue)
        {
            if(abs(TempMotorFootClose-MotorPosition[4])<MotorLimitValue)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
             return false;
        }
    }
    else
    {
         return false;
    }
}
/************************************************************************************************************
函数名：TimerUpdate()
参数：无
函数功能：定时器更新函数，循环发送读取数据
返回值：无
************************************************************************************************************/
void SCButton::TimerUpdate()
{
    if(CurrentFlag)
      ReadCurrent();
    if(ButtonFlag)
      SendMotorOpeation();
    SendMotorMeasure();
    ReadMotorPosition();
    RunOpenOrClose();

    static quint8 i=0;
    if((MotorCloseFlag==true)||(MotorOpenFlag==true))   //延时1s之后，再重新打开
    {
        i++;
        if(i==10)
        {
            MotorCloseFlag=false;
            MotorOpenFlag=false;
            i=0;
        }
    }
}
/************************************************************************************************************
函数名：StartReadCurrent()
参数：无
函数功能：启动定时器，开始读取电流。
返回值：无
************************************************************************************************************/
 void SCButton::StartReadCurrent()
 {
     static bool IsStartRead=false;
     IsStartRead=!IsStartRead;
     if(IsStartRead)
     {
       CurrentFlag=true;
       ui->tbnReadCurrent->setIcon(ImageFileStopReadCur);
     }
     else
     {
       CurrentFlag=false;

       ui->tbnReadCurrent->setIcon(ImageFileStartReadCur);
     }
 }
 /************************************************************************************************************
 函数名：SelectCurrentWidget()
 参数：int WidgetNum
 函数功能：进入界面和退出界面时的操作
 返回值：无
 ************************************************************************************************************/
void SCButton::SelectCurrentWidget(int WidgetNum)
{
    if(WidgetNum==1)
    {
        MyTimer->start(100);
        ShowMessage(tr("进入模拟按键界面！"),500);
        if(CurrentFlag)
            StartReadCurrent();
    }
    else
    {
        ButtonFlag=0;
        MyTimer->stop();
    }
}
