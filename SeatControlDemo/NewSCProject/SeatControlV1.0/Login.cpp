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
    delete ui;
}
void Login::InitUI()
{
    //QIntValidator *aIntValidator = new QIntValidator;
    // ui->lineEditRegistrationCode->setValidator(aIntValidator);
    ui->lineEditRegistrationCode->setEchoMode(QLineEdit::Password);
    ui->lineEditRegistrationCode->setMaxLength(20);
    ui->lineEditRegistrationCode->setText(tr("123456"));

}
void Login::InitConnect()
{
    connect(ui->tbnOk,SIGNAL(clicked(bool)),this,SLOT(OnOk()));
    connect(ui->tbnCancel,SIGNAL(clicked(bool)),this,SLOT(OnCancel()));
}
int Login::exec()
{
    this->setWindowModality(Qt::ApplicationModal);//类似于阻塞
    show();
    m_eventLoop = new QEventLoop(this);
    m_eventLoop->exec();
    return m_ChooseResult;
}
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
void Login::OnCancel()
{
     m_ChooseResult = Rejected;
     this->close();
}
void Login::closeEvent(QCloseEvent *event)
{
    if(m_eventLoop!=NULL)
    {
        m_eventLoop->exit();
    }
    event->accept();
}
