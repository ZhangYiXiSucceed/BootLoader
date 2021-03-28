#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QEventLoop>
#include <QCloseEvent>
#include <QMessageBox>
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    void InitUI();
    void InitConnect();
    int exec();
    void closeEvent(QCloseEvent *);
private:
    int m_ChooseResult;
    QEventLoop *m_eventLoop;
private slots:
    void OnOk();
    void OnCancel();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
