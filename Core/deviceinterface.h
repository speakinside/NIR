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
private:
   QSerialPort *device;
   QHash<QString, QString> deviceConf;
  bool DeviceConfChanged;
  QList<double> wavelength;

public:
  explicit DeviceInterface(QObject *parent = nullptr);
  virtual bool openDevice();
  virtual void changeConf(const QString &name, const QString &value);
  virtual void sendConf();
  virtual void getRef(QList<QPointF> *spectrum);
  virtual void getDark(QList<QPointF> *spectrum);
  virtual void getSpectrum(QList<QPointF> *spectrum);
signals:
  void startMeasuring();
  void endMeasuring();
public slots:
};

#endif // DEVICEINTERFACE_H
