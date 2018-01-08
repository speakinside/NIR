#ifndef MEASUREDIALOG_H
#define MEASUREDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include<QDialogButtonBox>
class MeasureDialog : public QDialog
{
    Q_OBJECT
private:
    QLineEdit *sampleNameField;
    QSpinBox *repeatTimesField;
    QDialogButtonBox *buttons;
public:
    explicit MeasureDialog(QWidget *parent = nullptr,Qt::WindowFlags f = Qt::WindowFlags());
    QString getSampleName();
    int getRepeatTimes();
signals:

public slots:
};

#endif // MEASUREDIALOG_H
