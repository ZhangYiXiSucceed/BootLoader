#include "SCRecordEvent.h"
#include "ui_SCRecordEvent.h"

SCRecordEvent::SCRecordEvent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SCRecordEvent)
{
    ui->setupUi(this);
}

SCRecordEvent::~SCRecordEvent()
{
    delete ui;
}
