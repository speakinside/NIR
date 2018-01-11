#ifndef DEVICEINTERFACE_H
#define DEVICEINTERFACE_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QHash>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

class DeviceInterface : public QObject
{
  Q_OBJECT
public:
  explicit DeviceInterface(QObject *parent = nullptr);
  virtual void openDevice(const QString &portName = "COM3", QSerialPort::BaudRate baudRate = QSerialPort::Baud115200);
  virtual void getRef(QList<QPointF> *spectrum, int integration_time = 8, int scan_times = 32);
  virtual void getDark(QList<QPointF> *spectrum, int integration_time = 8, int scan_times = 32);
  virtual void getSpectrum(QList<QPointF> *spectrum, int integration_time = 8, int scan_times = 32);
  virtual double getGainFactor();
  virtual bool setGainFactor(double gainFactor);
  virtual bool getGainDetector(); //high = true ; low = false;
  virtual bool setGainDetector(bool high = false);
  virtual bool isConnected();
signals:
  void portDisconnected();
  void portConnected(QString);
  void errorOccur(QSerialPort::SerialPortError);
  void startMeasuring();
  void endMeasuring();
public slots:

private:
  QSerialPort *device;
  bool deviceConnectivity;

private slots:
  void setLost();
  void setConnectted();
};

#endif // DEVICEINTERFACE_H
