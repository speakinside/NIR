#include "sampletable.h"

#include <QStringList>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QContextMenuEvent>
#include <QMenu>
#include <Qt>

SampleTable::SampleTable(QAbstractTableModel *model, QWidget *parent) : QTableView(parent)
{
    setMaximumHeight(300);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setModel(model);

    contextMenu = new QMenu(this);
    //contextMenu->addAction()
}

QModelIndexList SampleTable::selectedIndexes() const
{
    return QTableView::selectedIndexes();
}

void SampleTable::contextMenuEvent(QContextMenuEvent *event)
{
    auto index = indexAt( event->pos());
    if(!index.isValid())
        return;
    //TO DO: Sample table right-click Menu
}
