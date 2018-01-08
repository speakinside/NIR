#ifndef DEVICECONDITIONBOX_H
#define DEVICECONDITIONBOX_H

#include<QGroupBox>
#include<QTableWidget>
class DeviceConditionBox : public QGroupBox
{
    Q_OBJECT
   private:
    QTableWidget *tableWidget;
public:
    explicit DeviceConditionBox(QWidget *parent=nullptr);

signals:

public slots:
};

#endif // DEVICECONDITIONBOX_H
