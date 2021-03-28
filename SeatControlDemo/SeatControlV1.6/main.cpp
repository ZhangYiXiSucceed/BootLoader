#include "SCMainWindow.h"
#include "Login.h"

#include <QApplication>
/************************************************************************************************************
函数名：main
参数：int argc, char *argv[]
函数功能：主函数，负责主界面的显示和消息循环。
返回值：0或1，不过是在主程序退出的时候才返回，期间一直处于消息循环之中。
************************************************************************************************************/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login dlglogin;
    dlglogin.setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    int result = dlglogin.exec();
    if(result==QDialog::Accepted)
    {
        SCMainWindow w;
        w.setWindowFlags((w.windowFlags()|Qt::WindowMinimizeButtonHint) & ~Qt::WindowMaximizeButtonHint);
        w.show();
        return a.exec();
    }
}
