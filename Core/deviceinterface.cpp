#include "deviceinterface.h"

#include <QTime>
#include <QTimer>
#include <regex>
#include <QTextStream>
#include <QByteArray>
#include <plog/Log.h>

DeviceInterface::DeviceInterface(QObject *parent) : QObject(parent), deviceConnectivity(false)
{
    //TO DO:More human interface
    device = new QSerialPort(this);
    connect(this, &DeviceInterface::portDisconnected, this, &DeviceInterface::setLost);
    connect(this, &DeviceInterface::portConnected, this, &DeviceInterface::setConnectted);
}

void DeviceInterface::openDevice(const QString &portName, QSerialPort::BaudRate baudRate)
{
    if (device->isOpen())
        emit errorOccur(QSerialPort::OpenError);
    device->setPortName(portName);
    device->setBaudRate(baudRate);
    if (!device->open(QIODevice::ReadWrite))
    {
        emit errorOccur(device->error());
        device->clearError();
    }
    else
    {
        emit portConnected(portName);
    }
}

void DeviceInterface::getRef(QList<QPointF> *spectrum, int integration_time, int scan_times)
{
    LOG_INFO << "Try to get Ref";
    emit startMeasuring();
    device->flush();
    device->write(QString("*meas:light %0 %1 7\r").arg(integration_time).arg(scan_times).toLocal8Bit());
    QByteArray data("\0");
    for (int i = 0; i < 6; i++)
    {
        data += device->readAll();
        LOG_DEBUG << "Data Size:" << data.size();
        device->waitForReadyRead(integration_time * scan_times);
    }
    LOG_DEBUG << "Data:" << data;
    data = data.trimmed();
    data.remove(0, 2);
    auto strList = data.split('\r');
    for (auto &str : strList)
    {
        auto split = str.split('\t');
        spectrum->append(QPointF(split[0].trimmed().toDouble(), split[1].trimmed().toDouble()));
    }
    emit endMeasuring();
}

void DeviceInterface::getDark(QList<QPointF> *spectrum, int integration_time, int scan_times)
{
    LOG_INFO << "Try to get Dark";
    emit startMeasuring();
    device->flush();
    device->write(QString("*meas:dark %0 %1 7\r").arg(integration_time).arg(scan_times).toLocal8Bit());
    QByteArray data("\0");
    for (int i = 0; i < 6; i++)
    {
        data += device->readAll();
        LOG_DEBUG << "Data Size:" << data.size();
        device->waitForReadyRead(integration_time * scan_times);
    }
    LOG_DEBUG << "Data:" << data;
    data = data.trimmed();
    data.remove(0, 2);
    auto strList = data.split('\r');
    for (auto &str : strList)
    {
        auto split = str.split('\t');
        spectrum->append(QPointF(split[0].trimmed().toDouble(), split[1].trimmed().toDouble()));
    }
    emit endMeasuring();
}

void DeviceInterface::getSpectrum(QList<QPointF> *spectrum, int integration_time, int scan_times)
{
    LOG_INFO << "Try to get Spectrum";
    emit startMeasuring();
    device->flush();
    device->write(QString("*meas:light %0 %1 7\r").arg(integration_time).arg(scan_times).toLocal8Bit());
    QByteArray data("\0");
    for (int i = 0; i < 6; i++)
    {
        data += device->readAll();
        LOG_DEBUG << "Data Size:" << data.size();
        device->waitForReadyRead(integration_time * scan_times);
    }
    LOG_DEBUG << "Data:" << data;
    data = data.trimmed();
    data.remove(0, 2);
    auto strList = data.split('\r');
    for (auto &str : strList)
    {
        auto split = str.split('\t');
        spectrum->append(QPointF(split[0].trimmed().toDouble(), split[1].trimmed().toDouble()));
    }
    emit endMeasuring();
}

double DeviceInterface::getGainFactor()
{
    // Return the amplification factor of the AD-converter (Range = 1.5 till 5.5).
    // This value is factory adjusted to 1.5
    device->flush();
    device->write(QByteArray("*para:gain?\r"));
    device->waitForReadyRead(100);
    QByteArray res = device->readAll();
    auto gainFactor = res.split(':').back().trimmed().toDouble();
    return gainFactor;
}

bool DeviceInterface::setGainFactor(double gainFactor)
{
    device->flush();
    device->write(QByteArray("*para:gain ").append(QByteArray::number(gainFactor)).append('\r'));
    device->waitForBytesWritten();
    return true;
}

bool DeviceInterface::getGainDetector()
{
    device->flush();
    device->write(QByteArray("*para:pdag?\r"));
    device->waitForReadyRead(100);
    QByteArray res = device->readAll();
    int gainDetector = res.split(':').back().trimmed().toInt();
    return gainDetector == 0 ? false : true;
}

bool DeviceInterface::setGainDetector(bool high)
{
    device->flush();
    device->write(QByteArray("*para:pdag ").append(high ? "1\r" : "0\r"));
    device->waitForBytesWritten();
    return true;
}


bool DeviceInterface::isConnected()
{
    return deviceConnectivity;
}

void DeviceInterface::setLost()
{
    deviceConnectivity = false;
}

void DeviceInterface::setConnectted()
{
    deviceConnectivity = true;
}
