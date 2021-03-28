/************************************************************************************************************
界面名：固件升级界面
功能：固件的版本和固件的升级
************************************************************************************************************/
#include "SCFirmwareUpgrade.h"
#include "ui_SCFirmwareUpgrade.h"

SCFirmwareUpgrade::SCFirmwareUpgrade(QWidget *parent) :
    QWidget(parent),
    MyTimer(new QTimer(this)),
    ui(new Ui::SCFirmwareUpgrade)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

SCFirmwareUpgrade::~SCFirmwareUpgrade()
{
    delete MyTimer;
    delete ui;

}
/************************************************************************************************************
函数名：InitUI()
参数：无
函数功能：负责固件升级界面的初始化工作，包括按键的图标、字体和标志位等。
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::InitUI()
{
   #if Developer
     ui->tbnEnterAPP->setVisible(true);
     ui->tbnReturnBootLoader->setVisible(true);
   #else
    ui->tbnEnterAPP->setVisible(false);
    ui->tbnReturnBootLoader->setVisible(false);
   #endif

   QFont Ft("Microsoft YaHei");
   Ft.setPointSize(12);

   ui->leFileAddress->setFont(Ft);
   ui->lbDateTime->setFont(Ft);
   ui->lbFirmwareSize->setFont(Ft);

   ui->tbnOpenFile->setAutoRaise(true);
   ui->tbnOpenFile->setIcon(ImageOpenFile);
   ui->tbnOpenFile->setIconSize(ImageOpenFile.size());
   ui->tbnOpenFile->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
   ui->tbnOpenFile->setFont(Ft);

   ui->tbnReadFirmwareInfo->setAutoRaise(true);
   ui->tbnReadFirmwareInfo->setIcon(ImageReadFirmware);
   ui->tbnReadFirmwareInfo->setIconSize(ImageReadFirmware.size());
   ui->tbnReadFirmwareInfo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
   ui->tbnReadFirmwareInfo->setFont(Ft);

   ui->tbnDownload->setAutoRaise(true);
   ui->tbnDownload->setIcon(ImageDownload);
   ui->tbnDownload->setIconSize(ImageDownload.size());
   ui->tbnDownload->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
   ui->tbnDownload->setFont(Ft);

   ui->progressBar->setValue(0);

   ui->leBoot->setFont(Ft);
   ui->leFirmware->setFont(Ft);
   ui->leHardware->setFont(Ft);
   ui->leProgramState->setFont(Ft);
}
/************************************************************************************************************
函数名：InitConnect()
参数：无
函数功能：负责该TabWidget固件升级界面 信号与槽的连接。
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::InitConnect()
{
    connect(ui->tbnOpenFile,SIGNAL(clicked(bool)),this,SLOT(OpenFile()));
    connect(ui->tbnReadFirmwareInfo,SIGNAL(clicked(bool)),this,SLOT(ReadFirmwre()));
    connect(ui->tbnDownload,SIGNAL(clicked(bool)),this,SLOT(AutoDownload()));
    connect(ui->tbnEnterAPP,SIGNAL(clicked(bool)),this,SLOT(EnterAPP()));
    connect(ui->tbnReturnBootLoader,SIGNAL(clicked(bool)),this,SLOT(ReturnBootLoader()));
    connect(ui->tbnReadFirmwareInfo,SIGNAL(clicked(bool)),this,SLOT(ReadProgramerState()));
    connect(MyTimer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));
}
/************************************************************************************************************
函数名：OpenFile()
参数：无
函数功能：打开文件，获取二进制下载文件地址
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::OpenFile()
{
    FileAddress = QFileDialog::getOpenFileName(this,tr("Open File"),FileAddress,tr("(*.bin);;(*.hex);;(*.axf);;(*.txt);;All Files(*.*)"));
    ui->leFileAddress->setText(FileAddress);
    if(FileAddress != NULL)
    {
        QFileInfo *Temp = new QFileInfo(FileAddress);
        BinSize = Temp->size();
        //添加日期等
        QFile *BinFile = new QFile(FileAddress);
        if(BinFile->open(QIODevice::ReadOnly))
        {
            ShowMessage(tr("打开文件成功，并获取相关信息！"),1500);
            if(BinSize<2014)
                ui->lbFirmwareSize->setText(tr("%1B\n").arg(BinSize));
            else if(BinSize<1024*1024)
                ui->lbFirmwareSize->setText(tr("%1K %2B\n").arg(BinSize/1024).arg(BinSize%1024));
            else
                ui->lbFirmwareSize->setText(tr("%1M %2K %3B\n").arg(BinSize/1024/1024).arg(BinSize/1024%1024).arg(BinSize%1024));
            BinFile->close();
        }
        delete BinFile;
        delete Temp;
    }
    else
    {
        ShowMessage(tr("未打开任何文件！"),1500);
    }
}
/************************************************************************************************************
函数名：ReadFirmwre()
参数：无
函数功能：读取固件等版本
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::ReadFirmwre()
{
    QByteArray Command;
    Command.resize(5);
    Command[0]=0x68;
    Command[1]=0x06;
    Command[2]=0x1c;
    Command[3]=0x00;
    Command[4]=0x8A;
    emit SendCommand(Command);
}
/************************************************************************************************************
函数名：Download()
参数：无
函数功能：下载程序
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::Download()
{
    QByteArray Command;
    Command.resize(5);
    Command[0]=0x68;
    Command[1]=0x00;
    Command[2]=0x1c;
    Command[3]=0x00;
    Command[4]=0x84;
    emit SendCommand(Command);
    emit TransmitFileAddress(FileAddress,BinSize);
}
/************************************************************************************************************
函数名：RecProgressBar(uint8_t Value)
参数：uint8_t Value
函数功能：设置下载进度进度条
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::RecProgressBar(uint8_t Value)
{
    ui->progressBar->setValue(Value);
}
/************************************************************************************************************
函数名：EnterAPP()
参数：无
函数功能：进入APP应用程序
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::EnterAPP()
{
    QByteArray Command;
    Command.resize(5);
    Command[0]=0x68;
    Command[1]=0x04;
    Command[2]=0x1c;
    Command[3]=0x00;
    Command[4]=0x88;
    emit SendCommand(Command);
}
/************************************************************************************************************
函数名：ReturnBootLoader()
参数：无
函数功能：返回BootLoader
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::ReturnBootLoader()
{
    QByteArray Command;
    Command.resize(5);
    Command[0]=0x68;
    Command[1]=0x05;
    Command[2]=0x1c;
    Command[3]=0x00;
    Command[4]=0x89;
    emit SendCommand(Command);
}
/************************************************************************************************************
函数名：ReadProgramerState()
参数：无
函数功能：读取程序运行状态
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::ReadProgramerState()
{
    QByteArray Command;
    Command.resize(9);

    Command[0]=0x68;
    Command[1]=0x09;
    Command[2]=0x18;
    Command[3]=0x00;
    Command[4]=0x89;

    Command[5]=0x68;
    Command[6]=0x06;
    Command[7]=0x18;
    Command[8]=0x00;
    Command[9]=0x86;
    emit SendCommand(Command);
}
/************************************************************************************************************
函数名：RecFirmwareInfo(uint8_t CommandCode,QString Info)
参数：无
函数功能：接收固件程序
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::RecFirmwareInfo(uint8_t CommandCode,QString Info)
{
   QStringList TempData=Info.split('#');
  if(CommandCode==0x06)
 {
     QString VBoot=TempData.at(0);
     VBoot.insert(0,tr("V"));
     VBoot.insert(2,tr("."));
     ui->leBoot->setText(VBoot);

     QString VFirmware=TempData.at(1);
     VFirmware.insert(0,tr("V"));
     VFirmware.insert(2,tr("."));
     ui->leFirmware->setText(VFirmware);

     QString VHardware=TempData.at(2);
     VHardware.insert(0,tr("V"));
     VHardware.insert(2,tr("."));
     ui->leHardware->setText(VHardware);
 }
  else if(CommandCode==0x09)
  {
    ui->leProgramState->setText(Info);
  }
}
/************************************************************************************************************
函数名：AutoDownload()
参数：无
函数功能：一键下载程序
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::AutoDownload()
{
    MyTimer->start(500);
}
/************************************************************************************************************
函数名：TimerUpdate()
参数：无
函数功能：定时器更新函数，用于一键下载程序，返回BootLoader，程序的传输，APP的进入
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::TimerUpdate()
{
    static uint8_t Times=0;
    ui->tbnDownload->setEnabled(false);
    ui->tbnReadFirmwareInfo->setEnabled(false);
    if(Times==0)
    {
        emit ui->tbnReturnBootLoader->click();
        MyTimer->stop();
        MyTimer->start(500);
    }
    else if(Times==1)
    {
        MyTimer->stop();
        Download();

    }
    else if(Times==2)
    {
        emit ui->tbnEnterAPP->click();
        MyTimer->stop();

    }
    Times++;
    if(Times==3)
    {
       Times=0;
       ShowMessage(tr("更新已完成!"),1000);
       ui->tbnDownload->setEnabled(true);
       ui->tbnReadFirmwareInfo->setEnabled(true);
    }
}
/************************************************************************************************************
函数名：RecDownloadFinish()
参数：无
函数功能：程序接收完成
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::RecDownloadFinish()
{
    MyTimer->start(1000);
}
/************************************************************************************************************
函数名：SelectCurrentWidget()
参数：int WidgetNum，界面号
函数功能：进入界面和退出界面时的操作
返回值：无
************************************************************************************************************/
void SCFirmwareUpgrade::SelectCurrentWidget(int WidgetNum)
{
    if(WidgetNum==2)
    {
        ShowMessage(tr("进入固件升级界面！"),500);
    }
    else
    {

    }
}
