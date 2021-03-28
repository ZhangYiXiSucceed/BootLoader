#include "SCMainWindow.h"
#include "ui_SCMainWindow.h"

SCMainWindow::SCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SCMainWindow),
    MyTabWidget(new QTabWidget),
    MySCParameter(new SCParameter),
    MySCButton(new SCButton),
    MySCFirmwareUpgrade(new SCFirmwareUpgrade),
    MySCRecordEvent(new SCRecordEvent)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

SCMainWindow::~SCMainWindow()
{
    delete ui;
}
 void SCMainWindow::InitUI()
 {
     QHBoxLayout *Hlayout = new QHBoxLayout;
     Hlayout->addWidget(MyTabWidget);
     QVBoxLayout *Vlayout = new QVBoxLayout;
     Vlayout->addWidget(MyTabWidget);
     ui->centralWidget->setLayout(Vlayout);
     ui->centralWidget->setLayout(Hlayout);

     QFont Ft("Microsoft YaHei");
     Ft.setPointSize(12);

     this->MyTabWidget->setFont(Ft);
     this->MyTabWidget->setStyleSheet("color:rgb(0,0,0)");


     this->MyTabWidget->addTab(MySCParameter,tr("座椅参数设置"));
     this->MyTabWidget->addTab(MySCButton,tr("按键模拟"));
     this->MyTabWidget->addTab(MySCFirmwareUpgrade,tr("固件升级"));
     this->MyTabWidget->addTab(MySCRecordEvent,tr("日志记录"));

    // statusBar()->setStyleSheet("QStatusBar::item{border: 0px}");
     statusBar()->setFont(Ft);


     ComPort=new QLabel();
     ComPort->setText(tr("端口：无"));
     this->ui->statusBar->addPermanentWidget(ComPort);
     BradRate=new QLabel();
     BradRate->setText(tr("波特率：无"));
     this->ui->statusBar->addPermanentWidget(BradRate);

     TextMessage+=tr("提示消息：");
     TextMessage+=tr("界面创建成功!");
//     HintMessage = new QLabel();
//     HintMessage->setText(TextMessage);
//     HintMessage->setFont(Ft);
//     HintMessage->setStyleSheet("color:rgb(255,0,0)");
//     this->ui->statusBar->addWidget(HintMessage);
     statusBar()->showMessage(TextMessage,1500);
 }
 void SCMainWindow::InitConnect()
 {
     connect(MySCParameter,SIGNAL(ShowMessage(QString,uint16_t)),this,SLOT(ShowSystemMessage(QString,uint16_t)));
     connect(MySCParameter,SIGNAL(ShowParameter(QString,QString)),this,SLOT(ShowParameter(QString,QString)));
     connect(MySCParameter,SIGNAL(SendProgressBar(uint8_t)),MySCFirmwareUpgrade,SLOT(RecProgressBar(uint8_t)));
     connect(MySCParameter,SIGNAL(SendFirmwareInfo(uint8_t,QString)),MySCFirmwareUpgrade,SLOT(RecFirmwareInfo(uint8_t,QString)));

     connect(MySCFirmwareUpgrade,SIGNAL(ShowMessage(QString,uint16_t)),this,SLOT(ShowSystemMessage(QString,uint16_t)));
     connect(MySCFirmwareUpgrade,SIGNAL(SendCommand(QByteArray)),MySCParameter,SLOT(WriteBytes(QByteArray)));
     connect(MySCFirmwareUpgrade,SIGNAL(TransmitFileAddress(QString,quint64)),MySCParameter,SLOT(RecFileAddress(QString,quint64)));

 }
 void SCMainWindow::ShowSystemMessage(QString Message,uint16_t ShowTime)
 {
     statusBar()->showMessage(Message,ShowTime);
 }
void SCMainWindow::ShowParameter(QString ComPortData,QString BradRateData)
{
    ComPort->setText(tr("端口：")+ComPortData);
    this->ui->statusBar->addPermanentWidget(ComPort);
    BradRate->setText(tr("波特率：")+BradRateData);
    this->ui->statusBar->addPermanentWidget(BradRate);
}
