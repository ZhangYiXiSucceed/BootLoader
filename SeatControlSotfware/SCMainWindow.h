#ifndef SCMAINWINDOW_H
#define SCMAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextCodec>

#include <SCParameter.h>
#include <SCButton.h>
#include <SCFirmwareUpgrade.h>
#include <SCRecordEvent.h>

namespace Ui {
class SCMainWindow;
}

class SCMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SCMainWindow(QWidget *parent = 0);
    ~SCMainWindow();
    void InitUI();
    void InitConnect();
private slots:
    void ShowSystemMessage(QString,uint16_t);
    void ShowParameter(QString,QString);
private:
    Ui::SCMainWindow *ui;
    QTabWidget *MyTabWidget;

    SCParameter *MySCParameter              =    nullptr;
    SCButton    *MySCButton                 =    nullptr;
    SCFirmwareUpgrade *MySCFirmwareUpgrade  =    nullptr;
    SCRecordEvent     *MySCRecordEvent      =    nullptr;

    QLabel *BradRate;
    QLabel *ComPort;
    QLabel *HintMessage;
    QString TextMessage;
};

#endif // SCMAINWINDOW_H
