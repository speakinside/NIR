#ifndef CONTALPANEL_H
#define CONTALPANEL_H

#include <QTabWidget>
#include <QGroupBox>

class DeviceConditionBox;
class MeasureBox;

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
