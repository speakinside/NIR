#ifndef MEASUREBOX_H
#define MEASUREBOX_H

#include <QGroupBox>
#include <QTableWidget>

class MeasureBox : public QGroupBox
{
    Q_OBJECT
   private:
    QTableWidget *tableWidget;
public:
    explicit MeasureBox(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // MEASUREBOX_H
