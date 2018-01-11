#include "controlpanel.h"

#include <QVBoxLayout>
#include <QLabel>

ControlPanel::ControlPanel(QWidget *parent):QTabWidget(parent)
{
    setMaximumWidth(400);
    auto basicConf = createBasicConf();
    auto advancedConf = createAdvancedConf();
    addTab(basicConf, tr("Basic Configure"));
    addTab(advancedConf, tr("Advanced Configure"));
}

QWidget *ControlPanel::createBasicConf()
{
    auto w = new QWidget();
    deviceConditionBox = new DeviceConditionBox(w);
    measureBox = new MeasureBox(w);
    auto layout = new QVBoxLayout();
    layout->addWidget(deviceConditionBox);
    layout->addWidget(measureBox);
    w->setLayout(layout);
    return w;
}

QWidget *ControlPanel::createAdvancedConf()
{
    auto w = new QLabel("<b>没有<\b>");
    w->setScaledContents(true);
    w->setAlignment(Qt::AlignCenter);
    return w;
}
