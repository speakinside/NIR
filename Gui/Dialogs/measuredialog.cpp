#include "measuredialog.h"

#include <QLabel>
#include <QFormLayout>

MeasureDialog::MeasureDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    setWindowTitle(tr("Before Measurement"));
    auto layout = new QFormLayout();
    sampleNameField = new QLineEdit();
    repeatTimesField = new QSpinBox();
    repeatTimesField->setMinimum(1);
    layout->addRow(tr("Sample Name:"),sampleNameField);
    layout->addRow(tr("Measure Times:"),repeatTimesField);

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal);
    layout->addRow(buttons);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
    connect(buttons,&QDialogButtonBox::accepted,this,&QDialog::accept);
    connect(buttons,&QDialogButtonBox::rejected,this,&QDialog::reject);
}

QString MeasureDialog::getSampleName()
{
    return sampleNameField->text();
}

int MeasureDialog::getRepeatTimes()
{
    return repeatTimesField->value();
}
