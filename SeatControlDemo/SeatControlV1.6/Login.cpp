#include "Login.h"
#include "ui_Login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

Login::~Login()
{
    delete m_eventLoop;
    delete ui;
}
/************************************************************************************************************
函数名：InitUI()
参数：无
函数功能：负责登录界面的初始化工作，包括按键的图标、字体等。
返回值：无
************************************************************************************************************/
void Login::InitUI()
{
    QFont Ft("Microsoft YaHei");
    Ft.setPointSize(12);
    ui->lineEditRegistrationCode->setEchoMode(QLineEdit::Password);
    ui->lineEditRegistrationCode->setMaxLength(20);
    ui->lineEditRegistrationCode->setText(tr("123456"));

    ui->tbnOk->setFont(Ft);
    ui->tbnOk->setAutoRaise(true);
    ui->tbnOk->setIcon(ImageFileLogin);
    ui->tbnOk->setIconSize(ImageFileLogin.size());
    ui->tbnOk->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->tbnCancel->setFont(Ft);
    ui->tbnCancel->setAutoRaise(true);
    ui->tbnCancel->setIcon(ImageFileOut);
    ui->tbnCancel->setIconSize(ImageFileOut.size());
    ui->tbnCancel->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}
/************************************************************************************************************
函数名：InitConnect()
参数：无
函数功能：负责该对话框的信号与槽的连接。
返回值：无
************************************************************************************************************/
void Login::InitConnect()
{
    connect(ui->tbnOk,SIGNAL(clicked(bool)),this,SLOT(OnOk()));
    connect(ui->tbnCancel,SIGNAL(clicked(bool)),this,SLOT(OnCancel()));
}
/************************************************************************************************************
函数名：exec()
参数：无
函数功能：界面消息循环函数的重载，主要是为了增加返回值
返回值：布尔类型，选择结果
************************************************************************************************************/
int Login::exec()
{
    this->setWindowModality(Qt::ApplicationModal);//类似于阻塞
    show();
    m_eventLoop = new QEventLoop(this);
    m_eventLoop->exec();                    //保持消息循环，直到对话框退出
    return m_ChooseResult;
}
/************************************************************************************************************
函数名：OnOk()
参数：无
函数功能：按键登录的槽函数，负责登录注册密码的确认。
返回值：无
************************************************************************************************************/
void Login::OnOk()
{
    if(ui->lineEditRegistrationCode->text()==tr(""))
    {
        QMessageBox::critical(this,tr("提示"),tr("注册码不能为空!"));
    }
    else
    {
        if(ui->lineEditRegistrationCode->text()==tr("123456"))
        {
          m_ChooseResult = Accepted;
          this->close();
        }
        else
        {
            QMessageBox::critical(this,tr("提示"),tr("注册码错误！"));
        }
    }
}
/************************************************************************************************************
函数名：OnCancel()
参数：无
函数功能：取消按键的槽函数，退出程序。
返回值：无
************************************************************************************************************/
void Login::OnCancel()
{
     m_ChooseResult = Rejected;
     this->close();
}
/************************************************************************************************************
函数名：closeEvent（QCloseEvent *event）
参数：QCloseEvent 类型，关闭事件
函数功能：重载对话框关闭函数，用于QEeventLoop消息循环的退出。
返回值：无
************************************************************************************************************/
void Login::closeEvent(QCloseEvent *event)
{
    if(m_eventLoop!=NULL)
    {
        m_eventLoop->exit();
    }
    event->accept();            //程序退出，界面关闭
}
