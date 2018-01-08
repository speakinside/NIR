#ifndef SAVEMETHODS_H
#define SAVEMETHODS_H

#include <QObject>

class SaveMethods : public QObject
{
  Q_OBJECT
public:
  explicit SaveMethods(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SAVEMETHODS_H