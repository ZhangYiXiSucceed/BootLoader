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
    delete ui;
}
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
void SCFirmwareUpgrade::RecProgressBar(uint8_t Value)
{
    ui->progressBar->setValue(Value);
}
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
void SCFirmwareUpgrade::AutoDownload()
{
    MyTimer->start(500);
}
void SCFirmwareUpgrade::TimerUpdate()
{
    static uint8_t Times=0;
    ui->tbnDownload->setEnabled(false);
    ui->tbnReadFirmwareInfo->setEnabled(false);
    if(Times==0)
    {
        emit ui->tbnReturnBootLoader->click();
        MyTimer->start(1000);
    }
    else if(Times==1)
    {
        Download();
        MyTimer->stop();
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
void SCFirmwareUpgrade::RecDownloadFinish()
{
    MyTimer->start(1000);
}
