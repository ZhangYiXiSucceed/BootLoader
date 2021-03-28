#include "SCButton.h"
#include "ui_SCButton.h"

SCButton::SCButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SCButton)
{
    ui->setupUi(this);
}

SCButton::~SCButton()
{
    delete ui;
}
