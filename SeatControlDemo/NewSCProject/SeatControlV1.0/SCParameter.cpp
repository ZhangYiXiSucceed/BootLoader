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
    BinLoadCnt=0;
}
void SCParameter::InitConnect()
{
    connect(ui->tbnConnect,SIGNAL(clicked(bool)),this,SLOT(ConnectComPort()));
    connect(Timer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));
    connect(MySerialPort,SIGNAL(readyRead()),this,SLOT(ReadRecData()));
    connect(ui->cbCalibrate,SIGNAL(clicked(bool)),this,SLOT(CalibrateMode()));
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
   if(ui->tbnConnect->text()==tr("连接"))
   {
       if(PortCount>=ui->comboBoxComPort->count())
       {
            PortCount=0;
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
                Timer->start(50);
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
    while(MySerialPort->waitForReadyRead(5));
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
            PrintFrameData(RecData,DataIndex);       //打印接收指令
            IsCheckSumSucceed=IsCheckSumRight(RecData,DataIndex);    //校验是否正确

            if(IsCheckSumSucceed)
            {
                 RunCommand(RecData,DataIndex);      //运行相应指令
            }
            else
            {
                ShowMessage(tr("校验错误!"),1000);
                break;
            }
        }
        else
        {
            ShowMessage(tr("数据错误，没有找到包头！"),1000);
            break;
        }
    }
    qDebug()<<"当前数据处理完成！";
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
    }
        break;
    case 0x03:
    {
        qDebug()<<"连接成功！";
        Timer->stop();
        ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口打开成功！"),5000);
        ShowParameter(ui->comboBoxComPort->currentText().split(' ').first(),tr("115200"));
        ui->tbnConnect->setText(tr("断开"));
        ui->tbnConnect->setIcon(ImageFileClose);
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
         emit SendFirmwareInfo(0x06,tr("%1%2%3").arg(RecData[4]).arg(RecData[5]).arg(RecData[6]));
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
        if(RecData[4]==0x00)
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
    case 0x1D:
    {
         quint16 MotorAdValue=0;
         MotorAdValue=(RecData[DataIndex+4]<<8)+(RecData[DataIndex+5]);
         if(MotorAdValue<4095)
         {
             if(RecData[DataIndex+2]==0x00)
             {
                 ui->leMotorBackPosition->setText(QString::number(MotorAdValue));
             }
             else if(RecData[DataIndex+2]==0x01)
             {
                 ui->leMotorLegPosition->setText(QString::number(MotorAdValue));
             }
             else if(RecData[DataIndex+2]==0x02)
             {
                 ui->leMotorFootPosition->setText(QString::number(MotorAdValue));
             }
         }
         DataIndex+=(RecData[DataIndex+3]+5);
    }
        break;
    default:
        break;
    }
}
void SCParameter::TimerUpdate()
{
    ShowMessage(ui->comboBoxComPort->currentText().split(' ').first()+tr("端口未响应!"),2000);
    MySerialPort->close();
    emit ui->tbnConnect->click();
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
        qDebug()<<"cnt="<<cnt;
        qDebug()<<"i="<<i;
        qDebug()<<"Temp="<<Temp<<"BinSize="<<BinSize;
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
    }
    else
    {
        Command[0]=0x68;
        Command[1]=0x08;
        Command[2]=0x0C;
        Command[3]=0x00;
        Command[4]=0x7C;
    }
    WriteBytes(Command);
}
