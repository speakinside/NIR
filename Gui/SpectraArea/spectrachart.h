#ifndef SPECTRACHART_H
#define SPECTRACHART_H

#include <QChart>
#include <QPair>
using namespace QtCharts;

class SpectraChart : public QChart
{
    Q_OBJECT
  private:
    QPair<double, double> x, y;

  public:
    explicit SpectraChart(QGraphicsItem *parent = nullptr);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
  signals:

  public slots:
};

#endif // SPECTRACHART_H
