#ifndef FAKEDEVICE_H
#define FAKEDEVICE_H

#include "deviceinterface.h"

class FakeDevice : public DeviceInterface
{
  Q_OBJECT
private:
  QSerialPort *device;
  QHash<QString, QString> deviceConf;
  bool DeviceConfChanged;
  QList<double> wavelength;

public:
  explicit FakeDevice(QObject *parent = nullptr);
  bool openDevice();
  void changeConf(const QString &name, const QString &value);
  void sendConf();
  void getRef(QList<QPointF> *spectrum);
  void getDark(QList<QPointF> *spectrum);
  void getSpectrum(QList<QPointF> *spectrum);
signals:
  void startMeasuring();
  void endMeasuring();
public slots:
};

#endif // FAKEDEVICE_H
