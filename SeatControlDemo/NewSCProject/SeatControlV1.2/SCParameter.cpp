#include "SCParameter.h"
#include "ui_SCParameter.h"

SCParameter::SCParameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SCParameter),
    MySerialPort(new QSerialPort),
    Timer(new QTimer(this))
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
    CommandStringInit();
}

SCParameter::~SCParameter()
{
    delete ui;
}
void SCParameter::InitUI()
{
    #if Developer
      ui->comboBoxComPort->setVisible(true);
    #else
      ui->comboBoxComPort->setVisible(false);
    #endif
    Refresh();
    ui->tbnConnect->setAutoRaise(true);
    ui->tbnConnect->setIcon(ImageFileOpen);
    ui->tbnConnect->setIconSize(ImageFileOpen.size());
    ui->tbnConnect->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QFont Ft("Microsoft YaHei");
    Ft.setPointSize(12);
    ui->tbnConnect->setFont(Ft);

    ui->cbCalibrate->setFont(Ft);

    ui->leMotorBackPosition->setFont(Ft);
    ui->leMotorLegPosition->setFont(Ft);
    ui->leMotorFootPosition->setFont(Ft);

    ui->tbnLoadPara->setFont(Ft);
    ui->tbnLoadPara->setAutoRaise(true);
    ui->tbnLoadPara->setIcon(ImageFileLoadPara);
    ui->tbnLoadPara->setIconSize(ImageFileLoadPara.size());
    ui->tbnLoadPara->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnReadPara->setFont(Ft);
    ui->tbnReadPara->setAutoRaise(true);
    ui->tbnReadPara->setIcon(ImageFileReadPara);
    ui->tbnReadPara->setIconSize(ImageFileReadPara.size());
    ui->tbnReadPara->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnWritePara->setFont(Ft);
    ui->tbnWritePara->setAutoRaise(true);
    ui->tbnWritePara->setIcon(ImageFileWritePara);
    ui->tbnWritePara->setIconSize(ImageFileWritePara.size());
    ui->tbnWritePara->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnSavePara->setFont(Ft);
    ui->tbnSavePara->setAutoRaise(true);
    ui->tbnSavePara->setIcon(ImageFileSavePara);
    ui->tbnSavePara->setIconSize(ImageFileSavePara.size());
    ui->tbnSavePara->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->leMotorBackClosePosition->setFont(Ft);
    ui->leMotorBackOpenPosition->setFont(Ft);
    ui->leMotorBackSpeed->setFont(Ft);

    ui->leMotorLegClosePosition->setFont(Ft);
    ui->leMotorLegOpenPosition->setFont(Ft);
    ui->leMotorLegSpeed->setFont(Ft);

    ui->leMotorFootClosePosition->setFont(Ft);
    ui->leMotorFootOpenPosition->setFont(Ft);
    ui->leMotorFootSpeed->setFont(Ft);

    ui->leMotorBackStart->setFont(Ft);
    ui->leMotorBackEnd->setFont(Ft);
    ui->leMotorBackMotorLegEnd->setFont(Ft);

    ui->leMotorLegStart->setFont(Ft);
    ui->leMotorLegEnd->setFont(Ft);


    ui->leMotorBackHalf->setFont(Ft);
    ui->leMotorLegHalf->setFont(Ft);
    ui->leMotorFootHalf->setFont(Ft);

    BinLoadCnt=0;
    ConnectTimes=0;
    ConnectFlag=0;
    CalibrateFlag=0;

//    IsMotorReverse[0]=false;
//    IsMotorReverse[1]=false;
//    IsMotorReverse[2]=false;

//    IsMotorPositionReverse[0]=false;
//    IsMotorPositionReverse[1]=false;
//    IsMotorPositionReverse[2]=false;
}
void SCParameter::InitConnect()
{
    connect(ui->tbnConnect,SIGNAL(clicked(bool)),this,SLOT(ConnectComPort()));
    connect(Timer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));
    connect(MySerialPort,SIGNAL(readyRead()),this,SLOT(ReadRecData()));
    connect(ui->cbCalibrate,SIGNAL(clicked(bool)),this,SLOT(CalibrateMode()));

    //connect(ui->rbnSelectMotorBackSpeed,SIGNAL(clicked(bool)),this,SLOT(MotorSpeed()));
    //connect(ui->rbnSelectMotorLegSpeed,SIGNAL(clicked(bool)),this,SLOT(MotorSpeed()));
    //connect(ui->rbnSelectMotorFootSpeed,SIGNAL(clicked(bool)),this,SLOT(MotorSpeed()));

    connect(ui->cbMotorBackDirectionIsReverse,SIGNAL(clicked(bool)),this,SLOT(MotorBackReverse()));
    connect(ui->cbMotorLegDirectionIsReverse,SIGNAL(clicked(bool)),this,SLOT(MotorLegReverse()));
    connect(ui->cbMotorFootDirectionIsReverse,SIGNAL(clicked(bool)),this,SLOT(MotorFootReverse()));

    connect(ui->cbMotorBackPositionIsReverse,SIGNAL(clicked(bool)),this,SLOT(MotorBackPositionReverse()));
    connect(ui->cbMotorLegPositionIsReverse,SIGNAL(clicked(bool)),this,SLOT(MotorLegPositionReverse()));
    connect(ui->cbMotorFootPositionIsReverse,SIGNAL(clicked(bool)),this,SLOT(MotorFootPositionReverse()));

    connect(ui->tbnReadPara,SIGNAL(clicked(bool)),this,SLOT(ReadPara()));
    connect(ui->tbnWritePara,SIGNAL(clicked(bool)),this,SLOT(WritePara()));
    connect(ui->tbnLoadPara,SIGNAL(clicked(bool)),this,SLOT(LoadPara()));
    connect(ui->tbnSavePara,SIGNAL(clicked(bool)),this,SLOT(SavePara()));

    connect(ui->rbnSelectMotorBackClose,SIGNAL(clicked(bool)),this,SLOT(ShowMotorBackClosePosition()));
    connect(ui->rbnSelectMotorBackOpen,SIGNAL(clicked(bool)),this,SLOT(ShowMotorBackOpenPosition()));

    connect(ui->rbnSelectMotorLegClose,SIGNAL(clicked(bool)),this,SLOT(ShowMotorLegClosePosition()));
    connect(ui->rbnSelectMotorLegOpen,SIGNAL(clicked(bool)),this,SLOT(ShowMotorLegOpenPosition()));

    connect(ui->rbnSelectMotorFootClose,SIGNAL(clicked(bool)),this,SLOT(ShowMotorFootClosePosition()));
    connect(ui->rbnSelectMotorFootOpen,SIGNAL(clicked(bool)),this,SLOT(ShowMotorFootOpenPosition()));

    connect(ui->rbnSelectMotorBackStart,SIGNAL(clicked(bool)),this,SLOT(ShowLinkMotorBackStart()));
    connect(ui->rbnSelectMotorBackEnd,SIGNAL(clicked(bool)),this,SLOT(ShowLinkMotorBackEnd()));
    connect(ui->rbnSelectMotorBackMotorLegEnd,SIGNAL(clicked(bool)),this,SLOT(ShowLinkMotorBackMotorLegEnd()));

    connect(ui->rbnSelectMotorLegStart,SIGNAL(clicked(bool)),this,SLOT(ShowLinkMotorLegStart()));
    connect(ui->rbnSelectMotorLegEnd,SIGNAL(clicked(bool)),this,SLOT(ShowLinkMotorLegEnd()));

    connect(ui->rbnSelectMotorBackHalf,SIGNAL(clicked(bool)),this,SLOT(ShowLinkMotorBackHalf()));
    connect(ui->rbnSelectMotorLegHalf,SIGNAL(clicked(bool)),this,SLOT(ShowLinkMotorLegHalf()));
    connect(ui->rbnSelectMotorFootHalf,SIGNAL(clicked(bool)),this,SLOT(ShowLinkMotorFootHalf()));
}
void SCParameter::Refresh()
{
    QList <QSerialPortInfo> InfoList = QSerialPortInfo::availablePorts();
    ui->comboBoxComPort->clear();
    if(InfoList.isEmpty())
        ShowMessage(tr("未找到可用的端口"),1500);
    else
    {
        foreach(QSerialPortInfo Info,InfoList)
            ui->comboBoxComPort->addItem(Info.portName() + ' ' + Info.description());
    }
}
void SCParameter::ConnectComPort()
{
   QString PortName;
   static uint8_t PortCount=0;
   if(ConnectTimes<=3)
   {
       if(ui->tbnConnect->text()==tr("连接"))
       {
           if(PortCount>=ui->comboBoxComPort->count())
           {
                PortCount=0;
                Refresh();
                ConnectTimes++;
           }
           ui->comboBoxComPort->setCurrentIndex(PortCount);
           PortName=ui->comboBoxComPort->currentText().split(' ').first();
           if(MySerialPort->isOpen())
           {
               MySerialPort->close();
                 ShowMessage(MySerialPort->portName()+tr("端口已关闭"),2000);
               return;
           }
           else
           {
               MySerialPort->setPortName(PortName);
               PortCount++;
               if(MySerialPort->open(QIODevice::ReadWrite))
               {
                    //ShowMessage(PortName+tr("端口状态:")+QString::number(MySerialPort->isOpen())+tr(" ")+tr("SerialPort Open Successfully!"),3000);
                    MySerialPort->setBaudRate(115200);
                    MySerialPort->setDataBits(QSerialPort::Data8);
                    MySerialPort->setParity(QSerialPort::NoParity);
                    MySerialPort->setStopBits(QSerialPort::OneStop);
                    MySerialPort->setFlowControl(QSerialPort::NoFlowControl);
                    QByteArray Indentifier;
                    Indentifier.resize(5);
                    Indentifier[0]=0x68;
                    Indentifier[1]=0x03;
                    Indentifier[2]=0x1C;
                    Indentifier[3]=0x00;
                    Indentifier[4]=0x87;
                    WriteBytes(Indentifier);
                    Timer->start(250);
                    ConnectFlag=1;
               }
               else
               {
                   ShowMessage(PortName+tr("打开失败，端口可能被占用！")+tr("串口状态:")+QString::number(MySerialPort->isOpen()),2000);
                   return;
               }
           }
       }
       else
       {

           MySerialPort->close();
           ui->tbnConnect->setText(tr("连接"));
           ui->tbnConnect->setIcon(ImageFileOpen);
           PortCount=0;
           ShowMessage(MySerialPort->portName()+tr("已断开"),1500);
           ShowParameter(tr("无"),tr("无"));
       }
   }
   else
   {
       Timer->stop();
       ConnectFlag=0;
       ConnectTimes=0;
       ShowMessage(tr("PC端口未响应，请检查设备是否插入！"),1000);
   }

}
void SCParameter::WriteBytes(QByteArray Data)
{
    if(MySerialPort->isOpen())
    {
        MySerialPort->write(Data);
    }
    else
    {
        ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口未打开，不能发送数据！"),2000);
    }
}
void SCParameter::ReadRecData()
{
    while(MySerialPort->waitForReadyRead(20));
    QByteArray RecData=MySerialPort->readAll();  //读取接收数据

    uint16_t DataIndex=0;               //数组移动索引
    bool IsCheckSumSucceed=false;

    while(1)
    {
        DataIndex=FindFrameHead(RecData,DataIndex);  //寻找包头
        if(DataIndex==200)                                //200索引用来指示是否接收完成，
        {
            break;
        }
        else if(DataIndex<100)                            //100索引用来指示是否出错，正常不会到100索引。
        {
            //PrintFrameData(RecData,DataIndex);       //打印接收指令
            IsCheckSumSucceed=IsCheckSumRight(RecData,DataIndex);    //校验是否正确

            if(IsCheckSumSucceed)
            {
                 RunCommand(RecData,DataIndex);      //运行相应指令
            }
            else
            {
                if((RecData[DataIndex+1])==0x04)
                {
                    ShowMessage(tr("进入APP成功!"),1000);
                }
                else if((RecData[DataIndex+1])==0x05)
                {
                    ShowMessage(tr("返回BootLoader成功!"),1000);
                }
                else
                {
                    ShowMessage(tr("校验错误!"),1000);
                }
                break;
            }
        }
        else
        {
            ShowMessage(tr("数据错误，没有找到包头！"),1000);
            break;
        }
    }
   // qDebug()<<"当前数据处理完成！";
}
uint8_t SCParameter::FindFrameHead(QByteArray RecData,uint16_t DataIndex)
{
    while(RecData[DataIndex]!=0x68)
    {
        if(RecData[DataIndex]==0x00)       //判断是否接收完成，即判断下面数组是否为0，如若为0，则接收完成。(默认数组初始化为0）
        {
            DataIndex=200;
            break;
        }
        else
        {
            DataIndex++;
            if(DataIndex>=100)
            {
                qDebug()<<"索引出错！";
                break;
            }
        }
    }
    return DataIndex;
}
void SCParameter::PrintFrameData(QByteArray RecData,uint16_t DataIndex)
{
  uint16_t DataLength=RecData[DataIndex+3]+4;   //计算帧数据长度，不包含校验位
  quint8 TempNum;
  for(int i=DataIndex;i<=DataLength+DataIndex;i++)
  {
      TempNum=RecData[i];
      qDebug("%x",TempNum);
  }
}
bool SCParameter::IsCheckSumRight(QByteArray RecData,uint16_t DataIndex)
{
    uint16_t DataLength=RecData[DataIndex+3]+4;//计算帧数据长度，不包含校验位
    uint8_t Sum=0;
    uint8_t CheckNum=0;
    for(uint8_t i=DataIndex;i<DataLength+DataIndex;i++)
    {
        Sum+=RecData[i];
    }
    CheckNum=RecData[DataLength+DataIndex];
    if((Sum%256)==CheckNum)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void SCParameter::RunCommand(QByteArray RecData,uint16_t &DataIndex)
{
    switch (RecData[DataIndex+1])
    {
    case 0x00:
    {
       ShowMessage(tr("下位机准备就绪"),1000);
       DataIndex+=(RecData[DataIndex+3]+5);
       TransmitBinData(0);
    }
        break;
    case 0x01:
    {
        BinLoadCnt++;
        ShowMessage(tr("成功下载%1KB").arg(BinLoadCnt),1000);
        DataIndex+=(RecData[DataIndex+3]+5);
        TransmitBinData(BinLoadCnt);
    }
        break;
    case 0x02:
    {
        BinLoadCnt++;
        ShowMessage(tr("成功下载%1KB").arg(BinLoadCnt),1000);
        DataIndex+=(RecData[DataIndex+3]+5);
        ShowMessage(tr("下载完成！"),1000);
        emit DownloadFinish();
    }
        break;
    case 0x03:
    {
        //qDebug()<<"连接成功！";
        Timer->stop();
        ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口打开成功！"),2000);
        ShowParameter(ui->comboBoxComPort->currentText().split(' ').first(),tr("115200"));
        ui->tbnConnect->setText(tr("断开"));
        ui->tbnConnect->setIcon(ImageFileClose);
        ConnectFlag=0;
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x04:
    {
        ShowMessage(tr("进入APP程序"),1000);
        ui->leMotorBackClosePosition->setText(tr("123"));
        DataIndex+=(RecData[DataIndex+3]+5);
    }
         break;
    case 0x05:
    {

    }
        break;
    case 0x06:
    {
         ShowMessage(tr("固件信息！"),1000);
         emit SendFirmwareInfo(0x06,QString::number(RecData[DataIndex+4],16)+tr("#")+QString::number(RecData[DataIndex+5],16)+tr("#")+QString::number(RecData[DataIndex+6],16));
         DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x07:
    {
         ShowMessage(tr("进入标定模式！"),1000);
         DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x08:
    {
        ShowMessage(tr("退出标定模式！"),1000);
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x09:
    {
        if(RecData[DataIndex+4]==0x00)
        {
            ShowMessage(tr("程序运行在APP"),1000);
            emit SendFirmwareInfo(0x09,tr("APP"));
        }
        else
        {
            ShowMessage(tr("程序运行在BootLoader"),1000);
            emit SendFirmwareInfo(0x09,tr("BootLoader"));
        }
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x10:
    {
        if(RecData[DataIndex+2]==0x00)
        {
            ui->leMotorBackSpeed->setText(QString::number(RecData[DataIndex+4]));
        }
        else if(RecData[DataIndex+2]==0x01)
        {
             ui->leMotorLegSpeed->setText(QString::number(RecData[DataIndex+4]));
        }
        else if(RecData[DataIndex+2]==0x02)
        {
             ui->leMotorFootSpeed->setText(QString::number(RecData[DataIndex+4]));
        }
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;

    case 0x12:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];
         quint16 MotorClosePosition = (High<<8)+(Low);
        if(RecData[DataIndex+2]==0x00)
        {
            ui->leMotorBackClosePosition->setText(QString::number(MotorClosePosition));
        }
        else if(RecData[DataIndex+2]==0x01)
        {
            ui->leMotorLegClosePosition->setText(QString::number(MotorClosePosition));
        }
        else if(RecData[DataIndex+2]==0x02)
        {
            ui->leMotorFootClosePosition->setText(QString::number(MotorClosePosition));
        }
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x13:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorOpenPosition = (High<<8)+(Low);

        if(RecData[DataIndex+2]==0x00)
        {
            ui->leMotorBackOpenPosition->setText(QString::number(MotorOpenPosition));
        }
        else if(RecData[DataIndex+2]==0x01)
        {
            ui->leMotorLegOpenPosition->setText(QString::number(MotorOpenPosition));
        }
        else if(RecData[DataIndex+2]==0x02)
        {
            ui->leMotorFootOpenPosition->setText(QString::number(MotorOpenPosition));
        }
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;

    case 0x15:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorPosition = (High<<8)+(Low);
        ui->leMotorBackStart->setText(QString::number(MotorPosition));
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x16:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorPosition = (High<<8)+(Low);
        ui->leMotorBackEnd->setText(QString::number(MotorPosition));
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x17:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorPosition = (High<<8)+(Low);
        ui->leMotorBackMotorLegEnd->setText(QString::number(MotorPosition));
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x18:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorPosition = (High<<8)+(Low);
        ui->leMotorLegStart->setText(QString::number(MotorPosition));
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x19:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorPosition = (High<<8)+(Low);
        ui->leMotorLegEnd->setText(QString::number(MotorPosition));
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x1A:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorPosition = (High<<8)+(Low);
        ui->leMotorBackHalf->setText(QString::number(MotorPosition));
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x1B:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorPosition = (High<<8)+(Low);
        ui->leMotorLegHalf->setText(QString::number(MotorPosition));
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x1C:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorPosition = (High<<8)+(Low);
        ui->leMotorFootHalf->setText(QString::number(MotorPosition));
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;

    case 0x1D:
    {
         static quint16 LastMotorAdValue[3];

         quint8 High=RecData[DataIndex+4];
         quint8 Low= RecData[DataIndex+5];

         quint16 MotorAdValue=MotorAdValue=(High<<8)+(Low);
         if(MotorAdValue<4095)
         {
             if(RecData[DataIndex+2]==0x00)
             {
                 if(fabs(MotorAdValue-LastMotorAdValue[0])>=4)
                 {
                     ui->leMotorBackPosition->setText(QString::number(MotorAdValue));
                     if(ui->rbnSelectMotorBackClose->isChecked())
                     {
                         ui->leMotorBackClosePosition->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorBackOpen->isChecked())
                     {
                         ui->leMotorBackOpenPosition->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorBackStart->isChecked())
                     {
                         ui->leMotorBackStart->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorBackEnd->isChecked())
                     {
                         ui->leMotorBackEnd->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorBackHalf->isChecked())
                     {
                         ui->leMotorBackHalf->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorBackMotorLegEnd->isChecked())
                     {
                         ui->leMotorBackMotorLegEnd->setText(QString::number(MotorAdValue));
                     }
                     LastMotorAdValue[0]=MotorAdValue;
                 }
             }
             else if(RecData[DataIndex+2]==0x01)
             {
                 if(fabs(MotorAdValue-LastMotorAdValue[1])>=4)
                 {
                     ui->leMotorLegPosition->setText(QString::number(MotorAdValue));
                     if(ui->rbnSelectMotorLegClose->isChecked())
                     {
                         ui->leMotorLegClosePosition->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorLegOpen->isChecked())
                     {
                         ui->leMotorLegOpenPosition->setText(QString::number(MotorAdValue));
                     }

                     if(ui->rbnSelectMotorLegStart->isChecked())
                     {
                         ui->leMotorLegStart->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorLegEnd->isChecked())
                     {
                         ui->leMotorLegEnd->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorLegHalf->isChecked())
                     {
                         ui->leMotorLegHalf->setText(QString::number(MotorAdValue));
                     }
                     LastMotorAdValue[1]=MotorAdValue;
                 }
             }
             else if(RecData[DataIndex+2]==0x02)
             {
                 if(fabs(MotorAdValue-LastMotorAdValue[2])>=4)
                 {
                     ui->leMotorFootPosition->setText(QString::number(MotorAdValue));
                     if(ui->rbnSelectMotorFootClose->isChecked())
                     {
                         ui->leMotorFootClosePosition->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorFootOpen->isChecked())
                     {
                         ui->leMotorFootOpenPosition->setText(QString::number(MotorAdValue));
                     }
                     if(ui->rbnSelectMotorFootHalf->isChecked())
                     {
                         ui->leMotorFootHalf->setText(QString::number(MotorAdValue));
                     }
                     LastMotorAdValue[2]=MotorAdValue;
                 }
             }
         }
         DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x1E:
    {
        quint8 High=RecData[DataIndex+4];
        quint8 Low= RecData[DataIndex+5];

        quint16 MotorCurrent = (High<<8)+(Low);
        emit SendButtonInfo(RecData[DataIndex+2],MotorCurrent);
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x20:
    {
        ShowMessage(tr("电机速度写入完成!"),1000);
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x21:
    {
        ShowMessage(tr("电机已反向！"),1000);
        if(RecData[DataIndex+2]==0x00)
        {
           IsMotorReverse[0]=false;
        }
        else if(RecData[DataIndex+2]==0x01)
        {
           IsMotorReverse[1]=false;
        }
        else if(RecData[DataIndex+2]==0x02)
        {
           IsMotorReverse[2]=false;
        }
        DataIndex+=(RecData[DataIndex+3]+5);
    }
         break;
    case 0x22:
    {
        ShowMessage(tr("收起极限位置写入完成!"),1000);
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x23:
    {
        ShowMessage(tr("打开极限位置写入完成!"),1000);
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x24:
    {
        ShowMessage(tr("电位器已反向！"),1000);
        if(RecData[DataIndex+2]==0x00)
        {
           IsMotorPositionReverse[0]=false;
        }
        else if(RecData[DataIndex+2]==0x01)
        {
           IsMotorPositionReverse[1]=false;
        }
        else if(RecData[DataIndex+2]==0x02)
        {
           IsMotorPositionReverse[2]=false;
        }
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x30:
    {
        ShowMessage(tr("背靠正在打开..."),500);
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x31:
    {
        ShowMessage(tr("背靠正在收起..."),500);
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    case 0x32:
    {
        ShowMessage(tr("背靠停止运行！"),500);
        DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    default:
        DataIndex+=(RecData[DataIndex+3]+5);
        break;

    }
}
void SCParameter::TimerUpdate()
{
    if(ConnectFlag)
    {
        ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口未响应!"),2000);
        MySerialPort->close();
        emit ui->tbnConnect->click();
    }
    if(CalibrateFlag)
    {
        MotorADCCommand();
    }
}

void SCParameter::RecFileAddress(QString TempFileAddress,quint64 TempBinsize)
{
     FileAddress = TempFileAddress;
     BinSize     = TempBinsize;
     BinLoadCnt=0;
}
void SCParameter::TransmitBinData(uint8_t cnt)
{
    qint64 Temp = 0;                    //记录剩余的下载字节数
    quint64 CheckCode = 0;

    char *binByte = new char[1024];     //存储1KB二进制下载文件字节数
    memset(binByte,0XFF,1024);

    QByteArray binTransmit;             //要打包发送的数据，包含包头校验等
    binTransmit.resize(8);

    QFile *binFile = new QFile(FileAddress);     //存储二进制下载文件
    binFile->open(QIODevice::ReadOnly);          //打开二进制文件
    binFile->seek(cnt*1024);                     //寻找对应要下载的二进制文件内容

    Temp = BinSize - cnt*1024;

    if(Temp<1024)
    {
        if(Temp>0)
        {
            binTransmit[0]=0x68;
            binTransmit[1]=0x02;
            binTransmit[2]=0x1C;
            binTransmit[3]=Temp%256;
            binTransmit[4]=Temp/256;

            binFile->read(binByte,Temp);
            binTransmit.insert(5,binByte,1024);

//            CRC.crc32(&binTransmit,binTransmit.size()-4);         //之后再细追究CRC校验
//            unsigned int CRCValue=CRC.getCrc32();

//            binTransmit[1024 + 5]=CRCValue;
//            binTransmit[1024 + 6]=CRCValue>>8;
//            binTransmit[1024 + 7]=CRCValue>>16;
//            binTransmit[1024 + 8]=CRCValue>>24;
            for(uint16_t i=5;i<Temp+5;i++)
                CheckCode+=binTransmit[i];
            binTransmit[1029]=CheckCode%256;
        }
        emit SendProgressBar(100);
    }
    else
    {
        binTransmit[0]=0x68;
        binTransmit[1]=0x01;
        binTransmit[2]=0x1C;
        binTransmit[3]=0x00;
        binTransmit[4]=0x04;

        binFile->read(binByte,1024);
        binTransmit.insert(5,binByte,1024);

//        CRC.crc32(&binTransmit,binTransmit.size()-5);
//        unsigned int CRCValue=CRC.getCrc32();

//        binTransmit[1024 + 5]=CRCValue;
//        binTransmit[1024 + 6]=CRCValue>>8;
//        binTransmit[1024 + 7]=CRCValue>>16;
//        binTransmit[1024 + 8]=CRCValue>>24;

        for(uint16_t i=5;i<1024+5;i++)
            CheckCode+=binTransmit[i];
        binTransmit[1029]=CheckCode%256;

        uint8_t i = (1-float(Temp)/float(BinSize))*100;
        emit SendProgressBar(i);
        //qDebug()<<"cnt="<<cnt;
        //qDebug()<<"i="<<i;
        //qDebug()<<"Temp="<<Temp<<"BinSize="<<BinSize;
    }
    if(Temp>0)
    {
         emit WriteBytes(binTransmit);
    }
    delete binFile;
    delete binByte;
}
void SCParameter::CommandStringInit()
{
     quint8 TempSendString[][10]=
    {
      {0x68,0x00,0x1c,0x00,0x00},  //更新指令

      {0x68,0x01,0x1c,0x80,0x00,0x00},  //更新程序
      {0x68,0x02,0x1c,0x80,0x00,0x00},  //更新程序

      {0x68,0x03,0x1c,0x00,0x00},  //确认连接

      {0x68,0x04,0x1c,0x00,0x00},  //进入APP
      {0x68,0x05,0x0c,0x00,0x00},  //返回BootLoader

      {0x68,0x06,0x1c,0x00,0x00},  //读取固件版本

      {0x68,0x07,0x0c,0x00,0x00},  //进入标定模式
      {0x68,0x08,0x0c,0x00,0x00},  //退出标定模式

      {0x68,0x09,0x1c,0x00,0x00},  //读取程序状态

    };
     quint8 i,j;
     quint8 TempSum=0;
     quint8 CommandLength = CommandLength1;
     for(i=0;i<CommandCount;i++)
     {
         TempSum=0;
         for(j=0;j<(CommandLength-1);j++)
         {
             TempSum+=TempSendString[i][j];
             SendCommandString[i][j]=TempSendString[i][j];
         }
         TempSendString[i][j]=TempSum;
         SendCommandString[i][j]=TempSum;
     }
}
void SCParameter::CalibrateMode()
{
    QByteArray Command;
    Command.resize(5);
    if(ui->cbCalibrate->isChecked())
    {
        Command[0]=0x68;
        Command[1]=0x07;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0x7B;

        CalibrateFlag=1;
        Timer->start(100);

        ui->tbnLoadPara->setEnabled(false);
        ui->tbnSavePara->setEnabled(false);
        ui->tbnReadPara->setEnabled(false);
    }
    else
    {
        Command[0]=0x68;
        Command[1]=0x08;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0x7C;

        Timer->stop();
        CalibrateFlag=0;

        ui->tbnLoadPara->setEnabled(true);
        ui->tbnSavePara->setEnabled(true);
        ui->tbnReadPara->setEnabled(true);
    }
    WriteBytes(Command);
}
void SCParameter::MotorADCCommand()
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

    WriteBytes(Command);
    MotorSpeed();
    SendMotorReverse();
    SendMotorPositionReverse();

}
void SCParameter::MotorSpeed()
{
    QByteArray Command;
    Command.resize(5);

    Command[0]=0x68;
    Command[1]=0x10;
    Command[3]=0x00;

    if(ui->rbnSelectMotorBackSpeed->isChecked())
    {
        Command[2]=0x08;
        Command[4]=0x80;
        WriteBytes(Command);
    }
    if(ui->rbnSelectMotorLegSpeed->isChecked())
    {
        Command[2]=0x09;
        Command[4]=0x81;
        WriteBytes(Command);
    }
    if(ui->rbnSelectMotorFootSpeed->isChecked())
    {
        Command[2]=0x0A;
        Command[4]=0x82;
        WriteBytes(Command);
    }

}
void SCParameter::MotorBackReverse()
{
     IsMotorReverse[0]=true;
}
void SCParameter::MotorLegReverse()
{
     IsMotorReverse[1]=true;
}
void SCParameter::MotorFootReverse()
{
    IsMotorReverse[2]=true;
}

void SCParameter::MotorBackPositionReverse()
{
    IsMotorPositionReverse[0]=true;
}
void SCParameter::MotorLegPositionReverse()
{
    IsMotorPositionReverse[1]=true;
}
void SCParameter::MotorFootPositionReverse()
{
     IsMotorPositionReverse[2]=true;
}
void SCParameter::SendMotorReverse()
{
    QByteArray Command;
    Command.resize(5);

    Command[0]=0x68;
    Command[1]=0x21;
    Command[3]=0x00;
    if(IsMotorReverse[0]==true)
    {
        Command[2]=0x08;
        Command[4]=0x91;
        WriteBytes(Command);
    }
    if(IsMotorReverse[1]==true)
    {
        Command[2]=0x09;
        Command[4]=0x92;
        WriteBytes(Command);
    }
    if(IsMotorReverse[2]==true)
    {
        Command[2]=0x0A;
        Command[4]=0x93;
        WriteBytes(Command);
    }
}
void SCParameter::SendMotorPositionReverse()
{
    QByteArray Command;
    Command.resize(5);
    Command[0]=0x68;
    Command[1]=0x24;
    Command[3]=0x00;
    if(IsMotorPositionReverse[0]==true)
    {
        Command[2]=0x08;
        Command[4]=0x94;
        WriteBytes(Command);
    }
    if(IsMotorPositionReverse[1]==true)
    {
        Command[2]=0x09;
        Command[4]=0x95;
        WriteBytes(Command);
    }
    if(IsMotorPositionReverse[2]==true)
    {
        Command[2]=0x0A;
        Command[4]=0x96;
        WriteBytes(Command);
    }
}



void SCParameter::ShowMotorBackClosePosition()
{
    if(!(ui->leMotorBackPosition->text().isEmpty()))
    {
        ui->leMotorBackClosePosition->setText(ui->leMotorBackPosition->text());
    }
}
void SCParameter::ShowMotorBackOpenPosition()
{
    if(!(ui->leMotorBackPosition->text().isEmpty()))
    {
        ui->leMotorBackOpenPosition->setText(ui->leMotorBackPosition->text());
    }
}

void SCParameter::ShowMotorLegClosePosition()
{
    if(!(ui->leMotorLegPosition->text().isEmpty()))
    {
        ui->leMotorLegClosePosition->setText(ui->leMotorLegPosition->text());
    }
}
void SCParameter::ShowMotorLegOpenPosition()
{
    if(!(ui->leMotorLegPosition->text().isEmpty()))
    {
        ui->leMotorLegOpenPosition->setText(ui->leMotorLegPosition->text());
    }
}

void SCParameter::ShowMotorFootClosePosition()
{
    if(!(ui->leMotorFootPosition->text().isEmpty()))
    {
        ui->leMotorFootClosePosition->setText(ui->leMotorFootPosition->text());
    }
}
void SCParameter::ShowMotorFootOpenPosition()
{
    if(!(ui->leMotorFootPosition->text().isEmpty()))
    {
        ui->leMotorFootOpenPosition->setText(ui->leMotorFootPosition->text());
    }
}

void SCParameter::ShowLinkMotorBackStart()
{
    if(!(ui->leMotorBackPosition->text().isEmpty()))
    {
        ui->leMotorBackStart->setText(ui->leMotorBackPosition->text());
    }
}
void SCParameter::ShowLinkMotorBackEnd()
{
    if(!(ui->leMotorBackPosition->text().isEmpty()))
    {
        ui->leMotorBackEnd->setText(ui->leMotorBackPosition->text());
    }
}
void SCParameter::ShowLinkMotorBackMotorLegEnd()
{
    if(!(ui->leMotorBackPosition->text().isEmpty()))
    {
        ui->leMotorBackMotorLegEnd->setText(ui->leMotorBackPosition->text());
    }
}
void SCParameter::ShowLinkMotorLegStart()
{
    if(!(ui->leMotorLegPosition->text().isEmpty()))
    {
        ui->leMotorLegStart->setText(ui->leMotorLegPosition->text());
    }
}
void SCParameter::ShowLinkMotorLegEnd()
{
    if(!(ui->leMotorLegPosition->text().isEmpty()))
    {
        ui->leMotorLegEnd->setText(ui->leMotorLegPosition->text());
    }
}
void SCParameter::ShowLinkMotorBackHalf()
{
    if(!(ui->leMotorBackPosition->text().isEmpty()))
    {
        ui->leMotorBackHalf->setText(ui->leMotorBackPosition->text());
    }
}
void SCParameter::ShowLinkMotorLegHalf()
{
    if(!(ui->leMotorLegPosition->text().isEmpty()))
    {
        ui->leMotorLegHalf->setText(ui->leMotorLegPosition->text());
    }
}
void SCParameter::ShowLinkMotorFootHalf()
{
    if(!(ui->leMotorFootPosition->text().isEmpty()))
    {
        ui->leMotorFootHalf->setText(ui->leMotorFootPosition->text());
    }
}

void SCParameter::ReadPara()
{
    QByteArray Command;
    static uint8_t i=0;
    if(i==0)
    {
        //收起极限位置
        Command[0]=0x68;
        Command[1]=0x12;
        Command[2]=0x08;
        Command[3]=0x00;
        Command[4]=0x82;

        Command[5]=0x68;
        Command[6]=0x12;
        Command[7]=0x09;
        Command[8]=0x00;
        Command[9]=0x83;

        Command[10]=0x68;
        Command[11]=0x12;
        Command[12]=0x0A;
        Command[13]=0x00;
        Command[14]=0x84;
        //打开极限位置
        Command[15]=0x68;
        Command[16]=0x13;
        Command[17]=0x08;
        Command[18]=0x00;
        Command[19]=0x83;

        Command[20]=0x68;
        Command[21]=0x13;
        Command[22]=0x09;
        Command[23]=0x00;
        Command[24]=0x84;

        Command[25]=0x68;
        Command[26]=0x13;
        Command[27]=0x0A;
        Command[28]=0x00;
        Command[29]=0x85;
        //电机速度
        Command[30]=0x68;
        Command[31]=0x10;
        Command[32]=0x08;
        Command[33]=0x00;
        Command[34]=0x80;

        Command[35]=0x68;
        Command[36]=0x10;
        Command[37]=0x09;
        Command[38]=0x00;
        Command[39]=0x81;

        Command[40]=0x68;
        Command[41]=0x10;
        Command[42]=0x0A;
        Command[43]=0x00;
        Command[44]=0x82;

        WriteBytes(Command);
    }
    else if(i==1)
    {
        Command[0]=0x68;
        Command[1]=0x15;
        Command[2]=0x08;
        Command[3]=0x00;
        Command[4]=0x85;

        Command[5]=0x68;
        Command[6]=0x16;
        Command[7]=0x08;
        Command[8]=0x00;
        Command[9]=0x86;

        Command[10]=0x68;
        Command[11]=0x17;
        Command[12]=0x08;
        Command[13]=0x00;
        Command[14]=0x87;

        Command[15]=0x68;
        Command[16]=0x18;
        Command[17]=0x08;
        Command[18]=0x00;
        Command[19]=0x88;

        Command[20]=0x68;
        Command[21]=0x19;
        Command[22]=0x08;
        Command[23]=0x00;
        Command[24]=0x89;

        Command[25]=0x68;
        Command[26]=0x1A;
        Command[27]=0x08;
        Command[28]=0x00;
        Command[29]=0x8A;

        Command[30]=0x68;
        Command[31]=0x1B;
        Command[32]=0x08;
        Command[33]=0x00;
        Command[34]=0x8B;

        Command[35]=0x68;
        Command[36]=0x1C;
        Command[37]=0x08;
        Command[38]=0x00;
        Command[39]=0x8C;
        WriteBytes(Command);
    }
    i++;
    if(i==2)
     i=0;
}
void SCParameter::SavePara()
{
   IniFileAddress = QFileDialog::getSaveFileName(this,tr("保存参数文件"),IniFileAddress+tr("Para.ini"),tr("(*.ini);;All Files(*.*)"));
   if(!IniFileAddress.isEmpty())
   {
       QSettings MyIniSetting(IniFileAddress,QSettings::IniFormat);
       if(!(ui->leMotorBackClosePosition->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorBackClosePosition"),ui->leMotorBackClosePosition->text());
       if(!(ui->leMotorLegClosePosition->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorLegClosePosition"),ui->leMotorLegClosePosition->text());
       if(!(ui->leMotorFootClosePosition->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorFootClosePosition"),ui->leMotorFootClosePosition->text());

       if(!(ui->leMotorBackOpenPosition->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorBackOpenPosition"),ui->leMotorBackOpenPosition->text());
       if(!(ui->leMotorLegOpenPosition->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorLegOpenPosition"),ui->leMotorLegOpenPosition->text());
       if(!(ui->leMotorFootOpenPosition->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorFootOpenPosition"),ui->leMotorFootOpenPosition->text());

       if(!(ui->leMotorBackSpeed->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorBackSpeed"),ui->leMotorBackSpeed->text());
       if(!(ui->leMotorLegSpeed->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorLegSpeed"),ui->leMotorLegSpeed->text());
       if(!(ui->leMotorFootSpeed->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorFootSpeed"),ui->leMotorFootSpeed->text());

       if(!(ui->leMotorBackStart->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorBackStart"),ui->leMotorBackStart->text());
       if(!(ui->leMotorBackEnd->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorBackEnd"),ui->leMotorBackEnd->text());
       if(!(ui->leMotorBackMotorLegEnd->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorBackMotorLegEnd"),ui->leMotorBackMotorLegEnd->text());

       if(!(ui->leMotorLegStart->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorLegStart"),ui->leMotorLegStart->text());
       if(!(ui->leMotorLegEnd->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorLegEnd"),ui->leMotorLegEnd->text());

       if(!(ui->leMotorBackHalf->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorBackHalf"),ui->leMotorBackHalf->text());
       if(!(ui->leMotorLegHalf->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorLegHalf"),ui->leMotorLegHalf->text());
       if(!(ui->leMotorFootHalf->text().isEmpty()))
          MyIniSetting.setValue(tr("leMotorFootHalf"),ui->leMotorFootHalf->text());
       ShowMessage(tr("参数保存成功！"),1000);
   }
  else
   {
       ShowMessage(tr("未保存任何文件！"),1000);
   }
}
void SCParameter::WritePara()
{
    QByteArray Command;
    quint8 CommandIndex=0;

    /****************电机收起极限位置***************************/
        if(ui->leMotorBackClosePosition->text()!=tr(""))
        {
            Command[0]=0x68;
            Command[1]=0x22;
            Command[2]=0x0C;
            Command[3]=0x02;
            Command[4]=ui->leMotorBackClosePosition->text().toInt()/256;
            Command[5]=ui->leMotorBackClosePosition->text().toInt()%256;
            Command[6]=0;
            for(quint8 i=0;i<6;i++)
                Command[6]=Command[6]+Command[i];
            CommandIndex+=7;
        }
        else
        {
            CommandIndex=CommandIndex;
        }

        if(ui->leMotorLegClosePosition->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x22;
            Command[CommandIndex+2]=0x0D;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorLegClosePosition->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorLegClosePosition->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]= Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;
        }
        else
        {
            CommandIndex=CommandIndex;
        }

        if(ui->leMotorFootClosePosition->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x22;
            Command[CommandIndex+2]=0x0E;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorFootClosePosition->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorFootClosePosition->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]=Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }

         /****************电机打开极限位置***************************/

        if(ui->leMotorBackOpenPosition->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x23;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorBackOpenPosition->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorBackOpenPosition->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]=Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }

        if(ui->leMotorLegOpenPosition->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x23;
            Command[CommandIndex+2]=0x0D;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorLegOpenPosition->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorLegOpenPosition->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]=Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }

        if(ui->leMotorFootOpenPosition->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x23;
            Command[CommandIndex+2]=0x0E;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorFootOpenPosition->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorFootOpenPosition->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]=Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }

        /****************电机速度***************************/

        if(ui->leMotorBackSpeed->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x20;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x01;
            Command[CommandIndex+4]=ui->leMotorBackSpeed->text().toInt()%256;
            Command[CommandIndex+5]=0;
            for(quint8 i=0;i<5;i++)
                Command[CommandIndex+5]=Command[CommandIndex+5]+Command[CommandIndex+i];

            CommandIndex+=6;

        }
        else
        {
            CommandIndex=CommandIndex;
        }

        if(ui->leMotorLegSpeed->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x20;
            Command[CommandIndex+2]=0x0D;
            Command[CommandIndex+3]=0x01;
            Command[CommandIndex+4]=ui->leMotorLegSpeed->text().toInt()%256;
            Command[CommandIndex+5]=0;
            for(quint8 i=0;i<5;i++)
                Command[CommandIndex+5]=Command[CommandIndex+5]+Command[CommandIndex+i];

            CommandIndex+=6;

        }
        else
        {
            CommandIndex=CommandIndex;
        }

        if(ui->leMotorFootSpeed->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x20;
            Command[CommandIndex+2]=0x0E;
            Command[CommandIndex+3]=0x01;
            Command[CommandIndex+4]=ui->leMotorFootSpeed->text().toInt()%256;
            Command[CommandIndex+5]=0;
            for(quint8 i=0;i<5;i++)
                Command[CommandIndex+5]=Command[CommandIndex+5]+Command[CommandIndex+i];
            CommandIndex+=6;

        }
        else
        {
            CommandIndex=CommandIndex;
        }
        /****************联动点位置***************************/
        if(ui->leMotorBackStart->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x25;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorBackStart->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorBackStart->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]= Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }

        if(ui->leMotorBackEnd->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x26;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorBackEnd->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorBackEnd->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]= Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }
        if(ui->leMotorBackMotorLegEnd->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x27;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorBackMotorLegEnd->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorBackMotorLegEnd->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]= Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }
        if(ui->leMotorLegStart->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x28;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorLegStart->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorLegStart->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]= Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }
        if(ui->leMotorLegEnd->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x29;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorLegEnd->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorLegEnd->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]= Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }
        if(ui->leMotorBackHalf->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x2A;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorBackHalf->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorBackHalf->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]= Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }
        if(ui->leMotorLegHalf->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x2B;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorLegHalf->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorLegHalf->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]= Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }
        if(ui->leMotorFootHalf->text()!=tr(""))
        {
            Command[CommandIndex]=0x68;
            Command[CommandIndex+1]=0x2C;
            Command[CommandIndex+2]=0x0C;
            Command[CommandIndex+3]=0x02;
            Command[CommandIndex+4]=ui->leMotorFootHalf->text().toInt()/256;
            Command[CommandIndex+5]=ui->leMotorFootHalf->text().toInt()%256;
            Command[CommandIndex+6]=0;
            for(quint8 i=0;i<6;i++)
                Command[CommandIndex+6]= Command[CommandIndex+6]+Command[CommandIndex+i];
            CommandIndex+=7;

        }
        else
        {
            CommandIndex=CommandIndex;
        }
        if(CommandIndex!=0)
           WriteBytes(Command);
        else
           ShowMessage(tr("参数不能为空，请输入后再写入！"),1000);
}
void SCParameter::LoadPara()
{
    IniFileAddress = QFileDialog::getOpenFileName(this,tr("打开参数文件"),IniFileAddress+tr("Para.ini"),tr("(*.ini);;All Files(*.*)"));
    if(!IniFileAddress.isEmpty())
    {
        QSettings MyIniSetting(IniFileAddress,QSettings::IniFormat);

        ui->leMotorBackClosePosition->setText(MyIniSetting.value("leMotorBackClosePosition").toString());
        ui->leMotorLegClosePosition->setText(MyIniSetting.value("leMotorLegClosePosition").toString());
        ui->leMotorFootClosePosition->setText(MyIniSetting.value("leMotorFootClosePosition").toString());

        ui->leMotorBackOpenPosition->setText(MyIniSetting.value("leMotorBackOpenPosition").toString());
        ui->leMotorLegOpenPosition->setText(MyIniSetting.value("leMotorLegOpenPosition").toString());
        ui->leMotorFootOpenPosition->setText(MyIniSetting.value("leMotorFootOpenPosition").toString());

        ui->leMotorBackSpeed->setText(MyIniSetting.value("leMotorBackSpeed").toString());
        ui->leMotorLegSpeed->setText(MyIniSetting.value("leMotorLegSpeed").toString());
        ui->leMotorFootSpeed->setText(MyIniSetting.value("leMotorFootSpeed").toString());

        ui->leMotorBackStart->setText(MyIniSetting.value("leMotorBackStart").toString());
        ui->leMotorBackEnd->setText(MyIniSetting.value("leMotorBackEnd").toString());
        ui->leMotorBackMotorLegEnd->setText(MyIniSetting.value("leMotorBackMotorLegEnd").toString());

        ui->leMotorLegStart->setText(MyIniSetting.value("leMotorBackStart").toString());
        ui->leMotorLegEnd->setText(MyIniSetting.value("leMotorLegEnd").toString());

        ui->leMotorBackHalf->setText(MyIniSetting.value("leMotorBackHalf").toString());
        ui->leMotorLegHalf->setText(MyIniSetting.value("leMotorLegHalf").toString());
        ui->leMotorFootHalf->setText(MyIniSetting.value("leMotorFootHalf").toString());
        ShowMessage(tr("参数载入成功！"),1000);
    }
    else
     {
         ShowMessage(tr("未载入任何文件！"),1000);
     }
}
