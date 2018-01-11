#include "savedialog.h"

#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QDir>
#include <QVariant>

SaveDialog::SaveDialog(QWidget *parent) : QDialog(parent)
{
    auto groupBox = initSaveContent();
    saveFormatBox = initSaveFormat();
    saveFilename = new QLineEdit(QString("%1/data.txt").arg(QDir::currentPath()), this);
    auto dialogButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    auto fileButton = new QPushButton("Select Path", this);

    auto layout = new QGridLayout();
    layout->setSpacing(10);

    layout->addWidget(new QLabel(tr("Save Path:"), this), 0, 0);
    layout->addWidget(saveFilename, 0, 1);
    layout->addWidget(fileButton, 0, 2);

    layout->addWidget(groupBox, 1, 0, 1, -1);

    layout->addWidget(new QLabel(tr("Save Format:"), this), 2, 0);
    layout->addWidget(saveFormatBox, 2, 1, 1, -1);

    layout->addWidget(dialogButtons, 3, 0, 1, -1);
    setLayout(layout);

    connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(fileButton, &QPushButton::clicked, [this] {
        auto filename = QFileDialog::getSaveFileName(this, tr("Save Flie Path"),getFilename());
        if (!filename.isEmpty())
            saveFilename->setText(filename);
    });
}

QGroupBox *SaveDialog::initSaveContent()
{
    QList<CustomCheckBox *> box;

    box << new CustomCheckBox(tr("ABORBANCE"), SaveProxy::SaveContent::ABORBANCE)
        << new CustomCheckBox(tr("INTENSITY"), SaveProxy::SaveContent::INTENSITY)
        << new CustomCheckBox(tr("DARK"), SaveProxy::SaveContent::DARK)
        << new CustomCheckBox(tr("REFERENCE"), SaveProxy::SaveContent::REFERENCE);

    saveContentBox = new QButtonGroup(this);
    saveContentBox->setExclusive(false);
    saveContentBox->addButton(box.at(0));
    saveContentBox->addButton(box.at(1));
    saveContentBox->addButton(box.at(2));
    saveContentBox->addButton(box.at(3));
    for (int i : {0, 1, 2, 3})
        box.at(i)->setChecked(true);

    auto groupBox = new QGroupBox(tr("Choose save data"), this);
    auto layout = new QGridLayout();

    layout->addWidget(box.at(0), 0, 0);
    layout->addWidget(box.at(1), 0, 1);
    layout->addWidget(box.at(2), 1, 0);
    layout->addWidget(box.at(3), 1, 1);

    groupBox->setLayout(layout);
    return groupBox;
}

QComboBox *SaveDialog::initSaveFormat()
{
    saveFormatBox = new QComboBox(this);
    //saveFormatBox->addItem("Json", SaveProxy::SaveFormat::Json);
    saveFormatBox->addItem("Tab Txt", SaveProxy::SaveFormat::TabTxt);
    //saveFormatBox->addItem("Csv file", SaveProxy::SaveFormat::Csv);
    return saveFormatBox;
}

QString SaveDialog::getFilename()
{
    return saveFilename->text();
}

SaveProxy::SaveFormat SaveDialog::getSaveFormat()
{
    return saveFormatBox->currentData().value<SaveProxy::SaveFormat>();
}

int SaveDialog::getSaveContent()
{
    int result = 0;
    for (auto &button : saveContentBox->buttons())
    {
        result |= static_cast<CustomCheckBox *>(button)->value;
    }
    return result;
}
