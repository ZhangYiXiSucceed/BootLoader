#ifndef SCRECORDEVENT_H
#define SCRECORDEVENT_H

#include <QWidget>

namespace Ui {
class SCRecordEvent;
}

class SCRecordEvent : public QWidget
{
    Q_OBJECT

public:
    explicit SCRecordEvent(QWidget *parent = 0);
    ~SCRecordEvent();

private:
    Ui::SCRecordEvent *ui;
};

#endif // SCRECORDEVENT_H
