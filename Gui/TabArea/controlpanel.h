#ifndef CONTALPANEL_H
#define CONTALPANEL_H

#include <QTabWidget>
#include <QGroupBox>

#include"deviceconditionbox.h"
#include"measurebox.h"


class ControlPanel : public QTabWidget
{
    Q_OBJECT
private:
        QWidget *createBasicConf();
    QWidget *createAdvancedConf();
public:
    explicit ControlPanel(QWidget *parent = nullptr);
    MeasureBox *measureBox;
    DeviceConditionBox *deviceConditionBox;
signals:

public slots:
};

#endif // CONTALPANEL_H
