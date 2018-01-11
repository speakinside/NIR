#include "saveproxy.h"

#include <QTextStream>

SaveProxy::SaveProxy(QString filename, int saveContent, SaveFormat saveFormat, QObject *parent)
    : QObject(parent), file(filename), saveContent(saveContent), saveFormat(saveFormat)
{
    // Attention : the sequence of class' initialization list follows
    //             the sequence of variants' defined sequence in the class declaration
}

void SaveProxy::setModel(DataModel *model)
{
    m_model = model;
}

void SaveProxy::setSaveFormat(SaveFormat saveFormat)
{
    saveFormat = saveFormat;
}

void SaveProxy::setSaveContent(int saveContent)
{
    saveContent = saveContent;

}

void SaveProxy::open()
{
    file.open(QIODevice::WriteOnly);
}

void SaveProxy::close()
{
    file.commit();
}

void SaveProxy::saveAll()
{
    QTextStream stream(&file);
    switch (saveFormat)
    {
    case SaveFormat::TabTxt:
        if (saveContent & SaveContent::INTENSITY)
            for (auto i = m_model->dataTable.begin() + 2; i != m_model->dataTable.end(); i++)
            {
                stream << (*i)->name <<"-"<<(*i)->index<< "\t";
                for (auto &point : (*i)->intensity->pointsVector())
                    stream << point.y() << "\t";
                stream << (*i)->dateTime.toString() << "\tIntensity" << endl;
            }
        if (saveContent & SaveContent::ABORBANCE)
            for (auto i = m_model->dataTable.begin() + 2; i != m_model->dataTable.end(); i++)
            {
                stream << (*i)->name <<"-"<<(*i)->index<< "\t";
                for (auto &point : (*i)->absorbance->pointsVector())
                    stream << point.y() << "\t";
                stream << (*i)->dateTime.toString() << "\tAbsorbance" << endl;
            }
        break;
    case SaveFormat::Csv:
        // TO DO
        break;
    case SaveFormat::Json:
        // TO DO
        break;
    default:
        break;
    }
}

void SaveProxy::addBasicInfo(QString basicConf)
{
    QTextStream stream(&file);
    if (saveFormat == SaveFormat::Json)
    {
    }

    stream << basicConf << endl;
    stream << "Wavelength\t";
    for (auto &point : m_model->dataTable.at(0)->intensity->pointsVector())
        stream << point.x() << "\t";
    stream << "DateTime\tKind" << endl;
    if (saveContent & SaveContent::DARK)
    {
        stream << m_model->dataTable.at(0)->name << "\t";
        for (auto &point : m_model->dataTable.at(0)->intensity->pointsVector())
            stream << point.y() << "\t";
        stream << m_model->dataTable.at(0)->dateTime.toString() << "\tIntensity" << endl;
    }
    if (saveContent & SaveContent::REFERENCE)
    {
        stream << m_model->dataTable.at(1)->name << "\t";
        for (auto &point : m_model->dataTable.at(1)->intensity->pointsVector())
            stream << point.y() << "\t";
        stream << m_model->dataTable.at(1)->dateTime.toString() << "\tIntensity" << endl;
    }
}


QString SaveProxy::formatLikeTab(QStringList &list)
{
    return list.join('\t');
}

QString SaveProxy::formatLikeJason(QStringList &list)
{
    return QString();
}

QString SaveProxy::formatLikeCsv(QStringList &list)
{
    return list.join(",,");
}
