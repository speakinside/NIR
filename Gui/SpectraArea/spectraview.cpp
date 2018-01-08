#include "spectraview.h"

#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include <QPointF>
#include <QPainter>
#include <QApplication>
#include <QKeyEvent>
#include <QEvent>
#include <QMenu>
#include <algorithm>
#include <plog/Log.h>

SpectraView::SpectraView(QWidget *parent) : QChartView(parent)
{
    setRubberBand(QChartView::RectangleRubberBand);
    setRenderHint(QPainter::Antialiasing);
    setMinimumSize(600, 400);
    absorbance = initAbsChart();
    intensity = initIntChart();
    setChart(intensity);
    setFrameShape(QFrame::StyledPanel);
    setLineWidth(1);
}

SpectraView::~SpectraView(void)
{
    for (auto &chart : {absorbance, intensity})
        if (chart != this->chart())
            delete chart;
}

void SpectraView::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    switch (key)
    {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -10);
        break;
    case Qt::Key_Equal:
        chart()->zoomReset();
        break;
    default:
        QChartView::keyPressEvent(event);
    }
}

void SpectraView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        return; //Prevent RubberBand zoomOut function, because of the contextMenu
    QChartView::mouseReleaseEvent(event);
}

void SpectraView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    connect(menu.addAction(tr("Set to default scale")), &QAction::triggered, chart(), &QChart::zoomReset);
    menu.addAction(tr("Save to Image"));
    menu.exec(event->globalPos());
    QChartView::contextMenuEvent(event);
}

QChart *SpectraView::initAbsChart()
{
    auto wavelength = new QValueAxis();
    auto absorbance = new QValueAxis();
    auto chart = new QChart();
    wavelength->setRange(1200, 1900);
    absorbance->setRange(0, 1);
    wavelength->setMinorTickCount(4);
    absorbance->setMinorTickCount(1);
    wavelength->setTitleText("Wavelength(nm)");
    absorbance->setTitleText("Absorbance");
    wavelength->setTitleFont(QFont("Hack"));
    absorbance->setTitleFont(QFont("Hack"));
    wavelength->setLabelFormat("%d");
    absorbance->setLabelFormat("%.2f");
    chart->setAxisX(wavelength);
    chart->setAxisY(absorbance);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::SeriesAnimations);
    return chart;
}

QChart *SpectraView::initIntChart()
{
    auto wavelength = new QValueAxis();

    auto intensity = new QValueAxis();
    auto chart = new QChart();
    wavelength->setRange(1200, 1900);
    intensity->setRange(700, 800);
    wavelength->setMinorTickCount(4);
    intensity->setMinorTickCount(4);
    wavelength->setTitleText("Wavelength(nm)");
    intensity->setTitleText("Intensity");
    wavelength->setTitleFont(QFont("Hack"));
    intensity->setTitleFont(QFont("Hack"));
    wavelength->setLabelFormat("%d");
    intensity->setLabelFormat("%d");
    chart->setAxisX(wavelength);
    chart->setAxisY(intensity);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::SeriesAnimations);
    return chart;
}

void SpectraView::addNewSeriesToChart(QAbstractSeries *series, QString chart)
{
    LOG_DEBUG << "addNewSeriesToCurrentChart";
    QValueAxis *axisY, *axisX;
    if (chart == "intensity")
    {
        intensity->addSeries(series);
        axisX = static_cast<QValueAxis *>(intensity->axisX());
        axisY = static_cast<QValueAxis *>(intensity->axisY());
    }
    else if (chart == "absorbance")
    {
        absorbance->addSeries(series);
        axisX = static_cast<QValueAxis *>(absorbance->axisX());
        axisY = static_cast<QValueAxis *>(absorbance->axisY());
    }
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    //series->setUseOpenGL();
    auto points = static_cast<QXYSeries *>(series)->pointsVector();
    auto minmax_x = std::minmax_element(points.begin(), points.end(),
                                        [](const auto &a, const auto &b) { return a.x() < b.x(); });
    auto minmax_y = std::minmax_element(points.begin(), points.end(),
                                        [](const auto &a, const auto &b) { return a.y() < b.y(); });
    if (minmax_x.first->x() < axisX->min())
        axisX->setMin(minmax_x.first->x());
    if (minmax_x.second->x() > axisX->max())
        axisX->setMax(minmax_x.second->x());
    if (minmax_y.first->y() < axisY->min())
        axisY->setMin(minmax_y.first->y());
    if (minmax_y.second->y() > axisY->max())
        axisY->setMax(minmax_y.second->y());
    //axisX->applyNiceNumbers();
    //axisY->applyNiceNumbers();
    //axisX->applyNiceNumbers();
    //axisY->applyNiceNumbers();
}

void SpectraView::changeChart(const QString &chart)
{
    if (chart == "Absorbance")
    {
        LOG_INFO << "Change Chart to Absorbance";
        setChart(absorbance);
    }
    if (chart == "Intensity")
    {
        LOG_INFO << "Change Chart to Intensity";
        setChart(intensity);
    }
}

void SpectraView::hideAllSeries()
{
    for (auto &chart : {intensity, absorbance})
        for (auto &s : chart->series())
            s->hide();
    //intensity->axisX()->setRange(1200, 1900);
    //intensity->axisY()->setRange(700, 800);
    //absorbance->axisX()->setRange(1200, 1900);
    //absorbance->axisY()->setRange(0, 1);

    emit allSeriesRemoved();
}
