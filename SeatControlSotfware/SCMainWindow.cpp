#include "SCMainWindow.h"
#include "ui_SCMainWindow.h"

SCMainWindow::SCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SCMainWindow),
    MyTabWidget(new QTabWidget),
    MySCParameter(new SCParameter(this)),
    MySCButton(new SCButton(this)),
    MySCFirmwareUpgrade(new SCFirmwareUpgrade(this)),
    MySCRecordEvent(new SCRecordEvent)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

SCMainWindow::~SCMainWindow()
{
    delete MySCButton;
    delete MySCFirmwareUpgrade;
    delete MySCParameter;
    delete MySCRecordEvent;
    delete MyTabWidget;
    delete ui;
}
/************************************************************************************************************
函数名：InitUI()
参数：无
函数功能：负责主界面（框架）下UI初始化，设置Tabwidget,statusBar等
返回值：无
************************************************************************************************************/
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
     //this->MyTabWidget->addTab(MySCRecordEvent,tr("日志记录"));

     statusBar()->setFont(Ft);


     ComPort=new QLabel();
     ComPort->setText(tr("端口：无"));
     this->ui->statusBar->addPermanentWidget(ComPort);
     BradRate=new QLabel();
     BradRate->setText(tr("波特率：无"));
     this->ui->statusBar->addPermanentWidget(BradRate);

     TextMessage+=tr("提示消息：");
     TextMessage+=tr("界面创建成功!");
     statusBar()->showMessage(TextMessage,1500);
 }
 /************************************************************************************************************
 函数名：InitConnect()
 参数：无
 函数功能：负责各界面之间的槽函数连接（兄弟窗口之间，槽函数只能在父窗口之间连接）
 返回值：无
 ************************************************************************************************************/
 void SCMainWindow::InitConnect()
 {
     connect(MyTabWidget,SIGNAL(currentChanged(int)),MySCParameter,SLOT(SelectCurrentWidget(int)));
     connect(MyTabWidget,SIGNAL(currentChanged(int)),MySCButton,SLOT(SelectCurrentWidget(int)));
     connect(MyTabWidget,SIGNAL(currentChanged(int)),MySCFirmwareUpgrade,SLOT(SelectCurrentWidget(int)));

     connect(MySCParameter,SIGNAL(ShowMessage(QString,uint16_t)),this,SLOT(ShowSystemMessage(QString,uint16_t)));
     connect(MySCParameter,SIGNAL(ShowParameter(QString,QString)),this,SLOT(ShowParameter(QString,QString)));
     connect(MySCParameter,SIGNAL(SendProgressBar(uint8_t)),MySCFirmwareUpgrade,SLOT(RecProgressBar(uint8_t)));
     connect(MySCParameter,SIGNAL(SendFirmwareInfo(uint8_t,QString)),MySCFirmwareUpgrade,SLOT(RecFirmwareInfo(uint8_t,QString)));
     connect(MySCParameter,SIGNAL(DownloadFinish()),MySCFirmwareUpgrade,SLOT(RecDownloadFinish()));
     connect(MySCParameter,SIGNAL(SendMotorPosition(QVector<quint16>)),MySCButton,SLOT(RecMotorPosition(QVector<quint16>)));

     connect(MySCFirmwareUpgrade,SIGNAL(ShowMessage(QString,uint16_t)),this,SLOT(ShowSystemMessage(QString,uint16_t)));
     connect(MySCFirmwareUpgrade,SIGNAL(SendCommand(QByteArray)),MySCParameter,SLOT(WriteBytes(QByteArray)));
     connect(MySCFirmwareUpgrade,SIGNAL(TransmitFileAddress(QString,quint64)),MySCParameter,SLOT(RecFileAddress(QString,quint64)));

     connect(MySCButton,SIGNAL(ShowMessage(QString,uint16_t)),this,SLOT(ShowSystemMessage(QString,uint16_t)));
     connect(MySCButton,SIGNAL(SendCommand(QByteArray)),MySCParameter,SLOT(WriteBytes(QByteArray)));

     connect(MySCParameter,SIGNAL(SendButtonInfo(quint8,quint8,quint16)),MySCButton,SLOT(RecParaData(quint8,quint8,quint16)));
 }
 /************************************************************************************************************
 函数名：ShowSystemMessage(QString Message,uint16_t ShowTime)
 参数：QString Message,uint16_t ShowTime，消息和显示消息的时间，单位ms
 函数功能：状态栏左侧显示消息
 返回值：无
 ************************************************************************************************************/
 void SCMainWindow::ShowSystemMessage(QString Message,uint16_t ShowTime)
 {
     statusBar()->showMessage(Message,ShowTime);
 }
 /************************************************************************************************************
 函数名：ShowParameter(QString ComPortData,QString BradRateData)
 参数：QString ComPortData,QString BradRateData  ，端口号和波特率
 函数功能：状态栏右侧永久显示波特率和端口号。
 返回值：无
 ************************************************************************************************************/
void SCMainWindow::ShowParameter(QString ComPortData,QString BradRateData)
{
    ComPort->setText(tr("端口：")+ComPortData);
    this->ui->statusBar->addPermanentWidget(ComPort);
    BradRate->setText(tr("波特率：")+BradRateData);
    this->ui->statusBar->addPermanentWidget(BradRate);
}
