#ifndef SPECTRAVIEW_H
#define SPECTRAVIEW_H

#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QVector>
#include <QPointF>
#include <QSplineSeries>

using namespace QtCharts;

class SpectraView : public QChartView
{
  Q_OBJECT
public:
  explicit SpectraView(QWidget *parent = nullptr);
  virtual ~SpectraView(void);
  void keyPressEvent(QKeyEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void contextMenuEvent(QContextMenuEvent *event);

private:
  QChart *absorbance;
  QChart *intensity;
  QChart *initAbsChart(); //Absorbance chart
  QChart *initIntChart(); //Intensity chart

signals:
  void allSeriesRemoved();

public slots:
  void addNewSeriesToChart(QAbstractSeries *series, QString chart = "intesity");
  void changeChart(const QString &chart);
  void hideAllSeries();
};

#endif // SPECTRAVIEW_H
