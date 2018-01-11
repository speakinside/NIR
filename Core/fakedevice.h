#ifndef FAKEDEVICE_H
#define FAKEDEVICE_H

#include "deviceinterface.h"

class FakeDevice : public DeviceInterface
{
  Q_OBJECT
private:
  QSerialPort *device;
  QHash<QString, QString> deviceConf;
  QList<double> wavelength;
  bool DeviceConfChanged;

public:
  explicit FakeDevice(QObject *parent = nullptr);
  virtual void openDevice(const QString &portName = "COM3", QSerialPort::BaudRate baudRate=QSerialPort::Baud115200);
  virtual void getRef(QList<QPointF> *spectrum,int integration_time=8, int scan_times=32);
  virtual void getDark(QList<QPointF> *spectrum,int integration_time=8, int scan_times=32);
  virtual void getSpectrum(QList<QPointF> *spectrum,int integration_time=8, int scan_times=32);
  virtual double getGainFactor();
  virtual bool setGainFactor(double gainFactor);
  virtual bool getGainDetector(); //high = true ; low = false;
  virtual bool setGainDetector(bool high = false);
signals:
  void startMeasuring();
  void endMeasuring();
public slots:
};

#endif // FAKEDEVICE_H
