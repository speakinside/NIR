#ifndef SAVEPROXY_H
#define SAVEPROXY_H

#include <QObject>
#include <QSaveFile>
#include <QVariant>
#include <QStringList>
#include <QTextStream>
#include <QPointF>

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
    SaveProxy(QString filename, SaveContent saveContent = SaveContent::ALL, SaveFormat saveFormat = SaveFormat::TabTxt, QObject *parent = nullptr);
    void setSaveFormat(SaveFormat saveFormat);
    void setSaveContent(int saveContent = SaveContent::ALL);
    void open();
    void addAnEntry(QString &name, QList<QPointF> spectrum);
    void addBasicInfo(QList<QPointF> ref, QList<QPointF> dark, QString basicConf);

  private:
    QSaveFile file;
    QTextStream stream;
    SaveFormat saveFormat;
    int saveContent;
    QString formatBasicInfo(QStringList &list);
    QString formatLikeTab(QStringList &list);
    QString formatLikeJason(QStringList &list);
    QString formatLikeCsv(QStringList &list);
};

#endif // SAVEPROXY_H
