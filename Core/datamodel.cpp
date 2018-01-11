#include "datamodel.h"
#include "deviceinterface.h"
#include <plog/Log.h>
#include <QPixmap>
#include <Qtime>
#include <QChart>
#include <cmath>

DataModel::DataModel(DeviceInterface *device, QObject *parent) : QAbstractTableModel(parent), device(device)
{
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return data_table.size();
}

int DataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return tr("Sample Name");
            case 1:
                return tr("Index");
            case 2:
                return tr("Date & Time");
            case 3:
                return tr("Wavelength");
            case 4:
                return tr("Show");
            }
        }
        return QVariant();
    case Qt::DecorationRole:
        if (orientation == Qt::Vertical)
        {
            auto pix = QPixmap(50, 50);
            pix.fill(std::get<4>(data_table.at(section))->color());
            return pix;
        }
        return QVariant();
    default:
        return QVariant();
    }
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case 0:
            return std::get<0>(data_table.at(index.row()));
        case 1:
            return QString::number(std::get<1>(data_table.at(index.row())));
        case 2:
            return std::get<2>(data_table.at(index.row())).toString();
        case 3:
        {
            auto &v = std::get<3>(data_table.at(index.row()));
            return tr("%1-%2").arg(v->front().x()).arg(v->back().x());
        }
        case 4:
            if (std::get<4>(data_table.at(index.row()))->isVisible())
                return tr("Show");
            else
                return tr("Hide");
        default:
            return QVariant();
        }
    case Qt::EditRole:
        switch (index.column())
        {
        case 0:
            return std::get<0>(data_table.at(index.row()));
        case 1:
            return QString::number(std::get<1>(data_table.at(index.row())));
        default:
            return QVariant();
        }
    case Qt::StatusTipRole:
        switch (index.column())
        {
        //TO DO: Every colunms status tips
        default:
            return tr("Status tips");
        }
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    default:
        return QVariant();
    }
}

bool DataModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Vertical && role == Qt::DecorationRole)
    {
        std::get<4>(data_table.at(section))->setColor(value.value<QColor>());
        if (section > 1)
            std::get<5>(data_table.at(section))->setColor(value.value<QColor>());
        emit headerDataChanged(Qt::Vertical, section, section);
        return true;
    }
    return false;
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole && index.column() == 0)
    {
        auto &name = std::get<0>(data_table[index.row()]);
        auto new_name = value.toString();
        if (new_name.isEmpty())
            return false;
        LOG_INFO << QString("Change Sample at Row %0's name: From \"%1\" To \"%2\"").arg(QString::number(index.row()), name, new_name);
        name = new_name;
        emit dataChanged(index, index, QVector<int>({Qt::DisplayRole}));
        return true;
    }
    if (role == Qt::DisplayRole && index.column() == 4)
    {
        if (value.toString() == "Hide")
        {
            std::get<4>(data_table[index.row()])->setVisible(false);
            if (index.row() > 1)
                std::get<5>(data_table[index.row()])->setVisible(false);
        }
        else
        {
            std::get<4>(data_table[index.row()])->setVisible(true);
            if (index.row() > 1)
                std::get<5>(data_table[index.row()])->setVisible(true);
        }
        emit dataChanged(index, index, QVector<int>({Qt::DisplayRole}));
    }
    return false;
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    else
        return QAbstractTableModel::flags(index) & (~Qt::ItemIsEditable);
}

bool DataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

bool DataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return false;
    beginRemoveRows(parent, row, row + count - 1);
    while (count--)
    {
        std::get<4>(data_table[row])->chart()->removeSeries(std::get<4>(data_table[row]));
        std::get<5>(data_table[row])->chart()->removeSeries(std::get<5>(data_table[row]));
        delete std::get<3>(data_table[row]); // delete spectrum intensity data
        delete std::get<4>(data_table[row]); // delete Intensity series
        delete std::get<5>(data_table[row]); // delete Absorbance series
        data_table.removeAt(row);
    }
    endRemoveRows();
    return true;
}

QList<QPointF> *DataModel::intToAbs(const QList<QPointF> *intSpectra)
{
    auto list = new QList<QPointF>();
    auto dark = std::get<3>(data_table[0]);
    auto ref = std::get<3>(data_table[1]);
    for (int c = 0; c < intSpectra->size(); c++)
        list->append(QPointF(intSpectra->at(c).x(),
                             std::log10((ref->at(c).y() - dark->at(c).y()) / (intSpectra->at(c).y() - dark->at(c).y()))));
    return list;
}

void DataModel::changeVisible(const QModelIndex &index)
{
    if (std::get<4>(data_table.at(index.row()))->isVisible())
        setData(index, "Hide", Qt::DisplayRole);
    else
        setData(index, "Show", Qt::DisplayRole);
}

void DataModel::save()
{

}

QXYSeries *DataModel::dataToLine(QList<QPointF> *spectrum, QString chart, QAbstractSeries::SeriesType type)
{
    LOG_INFO << "Create Serise into " << chart << " chart";
    QXYSeries *series;
    switch (type)
    {
    case QAbstractSeries::SeriesTypeSpline:
        LOG_DEBUG << "Create QSplineSeries";
        series = new QSplineSeries();
        break;
    case QAbstractSeries::SeriesTypeLine:
        LOG_DEBUG << "Create QLineSeries";
        series = new QLineSeries();
        break;
    default:
        LOG_WARNING << "No matching series type. Create QLineSeries by default.";
        series = new QLineSeries();
    }
    series->append(*spectrum);
    emit newSeriesCreated(series, chart);
    return series;
}

void DataModel::getSpectrum(QString name, int index, QAbstractSeries::SeriesType type)
{
    if(!device->isConnected())
    {
        emit noConnection();
        return;
    }
    if (data_table.size() < 2)
    {
        emit noRefAndDark();
        return;
    }
    auto spectrum = new QList<QPointF>;
    device->getSpectrum(spectrum);
    data_table.append(std::make_tuple(name, index, QDateTime::currentDateTime(), spectrum, dataToLine(spectrum, "intensity", type), dataToLine(intToAbs(spectrum), "absorbance", type)));
    std::get<5>(data_table.last())->setColor(std::get<4>(data_table.last())->color()); //synchronize the color
    insertRow(rowCount() - 1);
}

void DataModel::getRef(int index, QAbstractSeries::SeriesType type)
{
    if(!device->isConnected())
    {
        emit noConnection();
        return;
    }
    if (data_table.empty())
    {
        emit noRefAndDark();
        return;
    }
    auto spectrum = new QList<QPointF>;
    device->getRef(spectrum);
    if (data_table.size() < 2)
    {
        data_table.append(std::make_tuple(tr("Ref"), index, QDateTime::currentDateTime(), spectrum, dataToLine(spectrum, "intensity", type), nullptr));
        insertRow(rowCount() - 1);
    }
    else
    {
        auto &old_ref = data_table[1];
        delete std::get<3>(old_ref);
        auto chart = std::get<4>(old_ref)->chart();
        if (chart != nullptr)
            chart->removeSeries(std::get<4>(old_ref));
        delete std::get<4>(old_ref);
        data_table[1] = std::make_tuple(tr("Ref"), index, QDateTime::currentDateTime(), spectrum, dataToLine(spectrum, "intensity", type), nullptr);
        emit dataChanged(createIndex(1, 0), createIndex(1, columnCount() - 1), QVector<int>({Qt::DisplayRole}));
    }
}

void DataModel::getDark(int index, QAbstractSeries::SeriesType type)
{
    if(!device->isConnected())
    {
        emit noConnection();
        return;
    }
    auto spectrum = new QList<QPointF>;
    device->getDark(spectrum);
    if (data_table.empty())
    {
        data_table.append(std::make_tuple(tr("Dark"), index, QDateTime::currentDateTime(), spectrum, dataToLine(spectrum, "intensity", type), nullptr));
        insertRow(rowCount() - 1);
    }
    else
    {
        auto &old_dark = data_table[0];
        delete std::get<3>(old_dark);
        auto chart = std::get<4>(old_dark)->chart();
        if (chart != nullptr)
            chart->removeSeries(std::get<4>(old_dark));
        delete std::get<4>(old_dark);
        data_table[0] = std::make_tuple(tr("Dark"), index, QDateTime::currentDateTime(), spectrum, dataToLine(spectrum, "intensity", type), nullptr);
        emit dataChanged(createIndex(0, 0), createIndex(0, 4), QVector<int>({Qt::DisplayRole}));
    }
}
