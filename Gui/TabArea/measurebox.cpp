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
    tableWidget->setRowCount(5);
    tableWidget->setCellWidget(0, 0, new QLabel(tr("Max Wavelength(nm):")));
    tableWidget->setCellWidget(0, 1, new QSpinBox(this));
    tableWidget->setCellWidget(1, 0, new QLabel(tr("Min Wavelength(nm):")));
    tableWidget->setCellWidget(1, 1, new QSpinBox(this));
    tableWidget->setCellWidget(2, 0, new QLabel(tr("Integration Time(ms):")));
    tableWidget->setCellWidget(2, 1, new QSpinBox(this));
    tableWidget->setCellWidget(3, 0, new QLabel(tr("Scan Times:")));
    tableWidget->setCellWidget(3, 1, new QSpinBox(this));
    tableWidget->setCellWidget(4, 0, new QLabel(tr("Continuous Measure Times:")));
    tableWidget->setCellWidget(4, 1, new QSpinBox(this));
    static_cast<QSpinBox *>(tableWidget->cellWidget(0, 1))->setRange(700, 2500);
    static_cast<QSpinBox *>(tableWidget->cellWidget(1, 1))->setRange(700, 2500);
    static_cast<QSpinBox *>(tableWidget->cellWidget(0, 1))->setValue(2500);
    static_cast<QSpinBox *>(tableWidget->cellWidget(1, 1))->setValue(700);
    static_cast<QLabel *>(tableWidget->cellWidget(4, 0))->adjustSize();
    static_cast<QLabel *>(tableWidget->cellWidget(4, 0))->setWordWrap(true);
}
