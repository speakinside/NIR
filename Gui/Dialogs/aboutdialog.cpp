#include "aboutdialog.h"

#include <QPixmap>

AboutDialog::AboutDialog(QWidget *parent) : QMessageBox(parent)
{
    setIconPixmap(QPixmap(":/icons/res/githubIcon.svg"));
}
