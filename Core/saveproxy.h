#ifndef SAVEPROXY_H
#define SAVEPROXY_H

#include <QObject>
#include <QSaveFile>
#include <QVariant>
#include <QStringList>
#include <QTextStream>
#include <QPointF>
#include "datamodel.h"

class SaveProxy : public QObject
{
    Q_OBJECT

  public:
    enum SaveFormat
    {
        TabTxt,
        Json, //TO DO
        Csv   //TO DO
    };

    enum SaveContent
    {
        REFERENCE = 0b1,
        DARK = 0b10,
        ABORBANCE = 0b100,
        INTENSITY = 0b1000,
        ALL = 0b111111
    };

    SaveProxy(QString filename, int saveContent = SaveContent::ALL, SaveFormat saveFormat = SaveFormat::TabTxt, QObject *parent = nullptr);
    void setModel(DataModel *model);
    void setSaveFormat(SaveFormat saveFormat);
    void setSaveContent(int saveContent = SaveContent::ALL);
    void open();
    void close();
    void saveAll();
    void addBasicInfo(QString basicConf);

  private:
    QSaveFile file;
    SaveFormat saveFormat;
    int saveContent;
    DataModel *m_model;
    QString formatLikeTab(QStringList &list);
    QString formatLikeJason(QStringList &list);
    QString formatLikeCsv(QStringList &list);
};

#endif // SAVEPROXY_H
