#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QSerialPort>
#include <QString>

class ConnectDialog :public QDialog
{
private:
    QComboBox *comSerial;
    QComboBox *baudrate;
public:
    ConnectDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void refreshPorts();
    QSerialPort::BaudRate getBaudRate();
    QString getPortName();
};

#endif // CONNECTDIALOG_H
