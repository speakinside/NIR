#include "datamodel.h"
#include "deviceinterface.h"
#include <plog/Log.h>
#include <QPixmap>
#include <Qtime>
#include <QChart>
#include <cmath>

Sample::~Sample()
{
    LOG_WARNING << QString("Entry: %0 deleted").arg(name).toStdString();
    if (absorbance != nullptr)
    {
        if (absorbance->chart() != nullptr)
            absorbance->chart()->removeSeries(absorbance);
        delete absorbance;
    }
    if (intensity != nullptr)
    {
        if (intensity->chart() != nullptr)
            intensity->chart()->removeSeries(intensity);
        delete intensity;
    }
}

DataModel::DataModel(DeviceInterface *device, QObject *parent) : QAbstractTableModel(parent), device(device)
{
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return dataTable.size();
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
            pix.fill(dataTable.at(section)->intensity->color());
            return pix;
        }
        return QVariant();
    case Qt::UserRole: // return the Color of the entry using
        if (orientation == Qt::Vertical)
        {
            return dataTable.at(section)->intensity->color();
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
            return dataTable.at(index.row())->name;
        case 1:
            return QString::number(dataTable.at(index.row())->index);
        case 2:
            return dataTable.at(index.row())->dateTime.toString();
        case 3:
        {
            auto v = dataTable.at(index.row())->intensity->pointsVector();
            return tr("%1-%2").arg(v.front().x()).arg(v.back().x());
        }
        case 4:
            if (dataTable.at(index.row())->intensity->isVisible())
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
            return dataTable.at(index.row())->name;
        case 1:
            return dataTable.at(index.row())->index;
        default:
            return QVariant();
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
        dataTable.at(section)->intensity->setColor(value.value<QColor>());
        if (section > 1)
            dataTable.at(section)->absorbance->setColor(value.value<QColor>());
        emit headerDataChanged(Qt::Vertical, section, section);
        return true;
    }
    return false;
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole && index.column() == 0)
    {
        auto &name = dataTable[index.row()]->name;
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
            dataTable.at(index.row())->intensity->setVisible(false);
            if (index.row() > 1)
                dataTable.at(index.row())->absorbance->setVisible(false);
        }
        else
        {
            dataTable.at(index.row())->intensity->setVisible(true);
            if (index.row() > 1)
                dataTable.at(index.row())->absorbance->setVisible(true);
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
        // struct Sample self-manage the deletion of series
        delete dataTable[row];
        dataTable.removeAt(row);
    }
    endRemoveRows();
    return true;
}

QList<QPointF> *DataModel::intToAbs(const QList<QPointF> *intSpectra)
{
    // This function is mainly used in the creation of absorbance series.
    // As calling the series.pointsVector() is much more effecient than series.points()
    // So this function is rarely used outside the fuction of DataMode::getSpectrum
    // Because QList<QPoints> mainly used in that function.
    LOG_DEBUG << "Calculate the absorbance.";
    auto list = new QList<QPointF>();
    auto dark = dataTable[0]->intensity->pointsVector();
    auto ref = dataTable[1]->intensity->pointsVector();
    for (int c = 0; c < intSpectra->size(); c++)
        list->append(QPointF(intSpectra->at(c).x(),
                             std::log10((ref.at(c).y() - dark.at(c).y()) / (intSpectra->at(c).y() - dark.at(c).y()))));
    return list;
}

void DataModel::changeVisible(const QModelIndex &index)
{
    LOG_INFO << QString("The visibility of serie at row %0 changed").arg(index.row());
    if (dataTable[index.row()]->intensity->isVisible())
        setData(index, "Hide", Qt::DisplayRole);
    else
        setData(index, "Show", Qt::DisplayRole);
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
    if (!device->isConnected())
    {
        emit noConnection();
        return;
    }
    if (dataTable.size() < 2)
    {
        emit noRefAndDark();
        return;
    }
    auto spectrum = new QList<QPointF>;
    device->getSpectrum(spectrum);
    LOG_DEBUG << "Begin new data entry creation and insertion";
    dataTable << new Sample{name, index, QDateTime::currentDateTime(), dataToLine(spectrum), dataToLine(intToAbs(spectrum), "absorbance")};
    dataTable.last()->absorbance->setColor(dataTable.last()->intensity->color()); //synchronize the color
    insertRow(rowCount() - 1);
}

void DataModel::getRef(int index, QAbstractSeries::SeriesType type)
{
    if (!device->isConnected())
    {
        emit noConnection();
        return;
    }
    if (dataTable.empty())
    {
        emit noRefAndDark();
        return;
    }
    auto spectrum = new QList<QPointF>;
    device->getRef(spectrum);
    if (dataTable.size() < 2)
    {
        dataTable.append(new Sample{"Ref", index, QDateTime::currentDateTime(), dataToLine(spectrum, "intensity", type), nullptr});
        insertRow(rowCount() - 1);
    }
    else
    {
        LOG_WARNING << "Old reference is going to be replaced.";
        delete dataTable[1];
        dataTable[1] = new Sample{"Ref", index, QDateTime::currentDateTime(), dataToLine(spectrum, "intensity", type), nullptr};
        emit dataChanged(createIndex(1, 0), createIndex(1, columnCount() - 1), QVector<int>({Qt::DisplayRole}));
    }
}

void DataModel::getDark(int index, QAbstractSeries::SeriesType type)
{
    if (!device->isConnected())
    {
        emit noConnection();
        return;
    }
    auto spectrum = new QList<QPointF>;
    device->getDark(spectrum);
    if (dataTable.empty())
    {
        dataTable.append(new Sample{tr("Dark"), index, QDateTime::currentDateTime(), dataToLine(spectrum, "intensity", type), nullptr});
        insertRow(rowCount() - 1);
    }
    else
    {
        delete dataTable[0];
        dataTable[0] = new Sample{tr("Dark"), index, QDateTime::currentDateTime(), dataToLine(spectrum, "intensity", type), nullptr};
        emit dataChanged(createIndex(0, 0), createIndex(0, 4), QVector<int>({Qt::DisplayRole}));
    }
}

void DataModel::hideAllSeries()
{
    for (auto &sample : dataTable)
    {
        sample->intensity->hide();
        if (sample->absorbance != nullptr)
            sample->absorbance->hide();
    }
    emit dataChanged(createIndex(0, 4), createIndex(rowCount() - 1, 4), QVector<int>{Qt::DisplayRole});
}
