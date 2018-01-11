#include "saveproxy.h"

#include <QTextStream>

SaveProxy::SaveProxy(QString filename, SaveContent saveContent, SaveFormat saveFormat, QObject *parent)
    : QObject(parent), file(filename), saveContent(saveContent), saveFormat(saveFormat), stream(&file)
{
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
    file.open(QIODevice::ReadWrite);
    //stream->setDevice(&file);
}

void SaveProxy::addAnEntry(QString &name, QList<QPointF> spectrum)
{
    if (saveFormat == SaveFormat::Json)
    {
    }
    QStringList strList(name);
    for (auto &p : spectrum)
        strList << QString::number(p.y());
    stream << formatLikeTab(strList) << endl;
    file.commit();
}

void SaveProxy::addBasicInfo(QList<QPointF> ref, QList<QPointF> dark, QString basicConf)
{
    if (saveFormat == SaveFormat::Json)
    {
    }
    stream << basicConf << endl;
    QStringList strList("Wavelength");
    for (auto &p : ref)
        strList << QString::number(p.x());
    stream << formatLikeTab(strList) << endl;
    strList.clear();
    if (saveContent & SaveContent::DARK)
    {
        for (auto &p : dark)
            strList << QString::number(p.y());
        stream << formatLikeTab(strList) << endl;
    }
    if (saveContent & SaveContent::REFERENCE)
    {
        for (auto &p : ref)
            strList << QString::number(p.y());
        stream << formatLikeTab(strList) << endl;
    }
    file.commit();
}

QString SaveProxy::formatBasicInfo(QStringList &list)
{
    //For Later extension
    return QString();
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
