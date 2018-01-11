#include "measurebox.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSpinBox>
#include <QLabel>

MeasureBox::MeasureBox(QWidget *parent) : QGroupBox(parent)
{
    setTitle(tr("Measuren Condition"));
    auto layout = new QVBoxLayout();
    tableWidget = new QTableWidget(this);
    layout->addWidget(tableWidget);
    setLayout(layout);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setMaximumHeight(0); //hide headers
    tableWidget->verticalHeader()->setMaximumWidth(0);
    tableWidget->setColumnCount(2);
    tableWidget->setRowCount(2);
    tableWidget->setCellWidget(0, 0, new QLabel(tr("Integration Time(ms):")));
    tableWidget->setCellWidget(0, 1, new QSpinBox(this));
    tableWidget->setCellWidget(1, 0, new QLabel(tr("Scan Times:")));
    tableWidget->setCellWidget(1, 1, new QSpinBox(this));

    static_cast<QSpinBox *>(tableWidget->cellWidget(0, 1))->setRange(2,40000);
    static_cast<QSpinBox *>(tableWidget->cellWidget(1, 1))->setRange(1,10000);
    static_cast<QSpinBox *>(tableWidget->cellWidget(0, 1))->setValue(8);
    static_cast<QSpinBox *>(tableWidget->cellWidget(1, 1))->setValue(32);
    static_cast<QLabel *>(tableWidget->cellWidget(1, 0))->adjustSize();
}

int MeasureBox::getIntTime()
{
    return static_cast<QSpinBox *>(tableWidget->cellWidget(0, 1))->value();
}

int MeasureBox::getScanTimes()
{
    return static_cast<QSpinBox *>(tableWidget->cellWidget(1, 1))->value();
}
