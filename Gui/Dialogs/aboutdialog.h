#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QMessageBox>

class AboutDialog : public QMessageBox
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // ABOUTDIALOG_H
