#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include <QTableView>
#include <QTableWidget>
#include <QMenu>
class SampleTable : public QTableView
{
  Q_OBJECT
private:
    QMenu *contextMenu;
public:
  explicit SampleTable(QAbstractTableModel *model, QWidget *parent = nullptr);
    QModelIndexList selectedIndexes() const;
    void contextMenuEvent(QContextMenuEvent *event);
signals:

public slots:
};

#endif // SAMPLETABLE_H
