#ifndef SCBUTTON_H
#define SCBUTTON_H

#include <QWidget>

namespace Ui {
class SCButton;
}

class SCButton : public QWidget
{
    Q_OBJECT

public:
    explicit SCButton(QWidget *parent = 0);
    ~SCButton();

private:
    Ui::SCButton *ui;
};

#endif // SCBUTTON_H
