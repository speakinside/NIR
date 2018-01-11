#include "savedialog.h"

#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QVariant>

SaveDialog::SaveDialog(QWidget *parent) : QDialog(parent)
{
    auto groupBox = initSaveContent();
    saveFormatBox = initSaveFormat();
    saveFilename = new QLineEdit("data.txt", this);
    auto dialogButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    auto fileButton = new QPushButton("...", this);

    auto layout = new QGridLayout();
    layout->setSpacing(50);

    layout->addWidget(new QLabel(tr("Save Path:"), this), 0, 0);
    layout->addWidget(saveFilename, 0, 1);
    layout->addWidget(fileButton, 0, 2);

    layout->addWidget(groupBox, 1, 0, 1, 3);

    layout->addWidget(new QLabel(tr("Save Format:"), this), 2, 0);
    layout->addWidget(saveFormatBox, 2, 1, 2, 3);

    layout->addWidget(dialogButtons, 3, 0, 3, 3);
    setLayout(layout);

    connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(fileButton, &QPushButton::clicked, [this] {
        auto filename = QFileDialog::getSaveFileName(this, tr("Save Flie Path"));
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

    auto ExclusiveBox = new QButtonGroup(this);
    ExclusiveBox->setExclusive(true);
    ExclusiveBox->addButton(box.at(0));
    ExclusiveBox->addButton(box.at(1));
    saveContentBox = new QButtonGroup(this);
    saveContentBox->setExclusive(false);
    saveContentBox->addButton(box.at(0));
    saveContentBox->addButton(box.at(1));
    saveContentBox->addButton(box.at(2));
    saveContentBox->addButton(box.at(3));

    auto groupBox = new QGroupBox(tr("Choose save data"),this);
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
    saveFormatBox->addItem("Json", SaveProxy::SaveFormat::Json);
    saveFormatBox->addItem("Tab Txt", SaveProxy::SaveFormat::TabTxt);
    saveFormatBox->addItem("Csv file", SaveProxy::SaveFormat::Csv);
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
