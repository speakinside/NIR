#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QPointF>
#include <QLabel>
#include <QDatetime>
#include <QVariant>
#include <QLineSeries>
#include <QSplineSeries>
#include <QList>
#include <tuple>

using namespace QtCharts;

class DeviceInterface;

class DataModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit DataModel(DeviceInterface *device, QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

  QXYSeries *dataToLine(QList<QPointF> *spectrum, QString chart = "intensity", QAbstractSeries::SeriesType type = QAbstractSeries::SeriesTypeLine);
  QList<QPointF> *intToAbs(const QList<QPointF> *intSpectra);
  void changeVisible(const QModelIndex &index);
  DeviceInterface *device;
  //tuple<name,index,time,data,intensity series,absorbance series>
  QList<std::tuple<QString, int, QDateTime, QList<QPointF> *, QXYSeries *, QXYSeries *>> data_table;
signals:
  void newSeriesCreated(QAbstractSeries *, QString);
  void noRefAndDark();
public slots:
  void getSpectrum(QString name, int index, QAbstractSeries::SeriesType type = QAbstractSeries::SeriesTypeLine);
  void getRef(int index, QAbstractSeries::SeriesType type = QAbstractSeries::SeriesTypeLine);
  void getDark(int index, QAbstractSeries::SeriesType type = QAbstractSeries::SeriesTypeLine);
};

#endif // DATAMODEL_H
