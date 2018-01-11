#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include "Core/saveproxy.h"

Q_DECLARE_METATYPE(SaveProxy::SaveFormat)

class SaveDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SaveDialog(QWidget *parent = nullptr);
    QGroupBox *initSaveContent();
    QComboBox *initSaveFormat();
    QString getFilename();
    SaveProxy::SaveFormat getSaveFormat();
    int getSaveContent();
private:
    QButtonGroup *saveContentBox;
    QComboBox *saveFormatBox;
    QLineEdit *saveFilename;
};



class CustomCheckBox: public QCheckBox
{
    Q_OBJECT
public:
    SaveProxy::SaveContent value;
    CustomCheckBox(QString label,SaveProxy::SaveContent value,QWidget* parent = nullptr):QCheckBox(label,parent),value(value)
    {}
};

#endif // SAVEDIALOG_H
