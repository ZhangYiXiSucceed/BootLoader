#ifndef SCFIRMWAREUPGRADE_H
#define SCFIRMWAREUPGRADE_H

#include <QWidget>
#include <QFileDialog>
#include <QPixmap>

namespace Ui {
class SCFirmwareUpgrade;
}

class SCFirmwareUpgrade : public QWidget
{
    Q_OBJECT

public:
    explicit SCFirmwareUpgrade(QWidget *parent = 0);
    ~SCFirmwareUpgrade();
    void InitUI();
    void InitConnect();

private slots:
    void OpenFile();
    void ReadFirmwre();
    void Download();
    void RecProgressBar(uint8_t);
    void EnterAPP();
    void ReturnBootLoader();
    void ReadProgramerState();
    void RecFirmwareInfo(uint8_t,QString);
signals:
    void ShowMessage(QString,uint16_t);
    void SendCommand(QByteArray);
    void TransmitFileAddress(QString,quint64);
private:
    Ui::SCFirmwareUpgrade *ui;
    QString FileAddress;
    quint64 BinSize;

    QPixmap ImageOpenFile=tr(":/Image/OpenFile.png");
    QPixmap ImageReadFirmware=tr(":/Image/ReadFirmware.png");
    QPixmap ImageDownload=tr(":/Image/Download.png");
};

#endif // SCFIRMWAREUPGRADE_H
