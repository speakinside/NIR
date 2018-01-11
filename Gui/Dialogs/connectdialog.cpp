#include "connectdialog.h"

#include <QSerialPortInfo>
#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>

ConnectDialog::ConnectDialog(QWidget *parent , Qt::WindowFlags f ):QDialog(parent,f)
{
    baudrate = new QComboBox(this);
    comSerial = new QComboBox(this);
    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
    connect(buttons->addButton(tr("Refresh"),QDialogButtonBox::ActionRole),&QPushButton::clicked,this,&ConnectDialog::refreshPorts);

    for(auto &i:{1200,2400,4800,9600,19200,38400,57600,115200})
        baudrate->addItem(QString::number(i));
    baudrate->setCurrentIndex(7);
    refreshPorts();
    auto layout = new QFormLayout;
    layout->addRow(tr("Serial Port:"),comSerial);
    layout->addRow(tr("BaudRate:"),baudrate);
    layout->addRow(buttons);
    layout->setSpacing(10);
    setLayout(layout);

    connect(buttons,&QDialogButtonBox::accepted,this,&QDialog::accept);
    connect(buttons,&QDialogButtonBox::rejected,this,&QDialog::reject);
}

void ConnectDialog::refreshPorts()
{
    comSerial->clear();

    comSerial->addItem(QString("COM3"));

    auto list = QSerialPortInfo::availablePorts();
    for(auto &i:list)
    comSerial->addItem(i.portName());

}

QSerialPort::BaudRate ConnectDialog::getBaudRate()
{
    return static_cast<QSerialPort::BaudRate>(baudrate->currentText().toInt());
}

QString ConnectDialog::getPortName()
{
    return comSerial->currentText();
}
