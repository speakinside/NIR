#include "deviceconditionbox.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QLabel>

DeviceConditionBox::DeviceConditionBox(QWidget *parent): QGroupBox(parent)
{
    setTitle(tr("Device Condition"));
    auto layout = new QVBoxLayout();
    tableWidget = new QTableWidget(this);
    layout->addWidget(tableWidget);
    setLayout(layout);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tableWidget->horizontalHeader()->setMaximumHeight(0); //hide headers
    tableWidget->verticalHeader()->setMaximumWidth(0);
    tableWidget->setColumnCount(2);
    tableWidget->setRowCount(4);
    tableWidget->setCellWidget(0,0,new QLabel(tr("Device Condition:")));
    tableWidget->setCellWidget(0,1,new QLabel(tr("Disconnected")));
    tableWidget->setCellWidget(1,0,new QLabel(tr("Serial Port:")));
    tableWidget->setCellWidget(1,1,new QLabel(tr("None")));
    tableWidget->setCellWidget(2,0,new QLabel(tr("Dark Spectrum:")));
    tableWidget->setCellWidget(2,1,new QLabel(tr("Not Measured")));
    tableWidget->setCellWidget(3,0,new QLabel(tr("Refference Spectrum:")));
    tableWidget->setCellWidget(3,1,new QLabel(tr("Not Measured")));
}
