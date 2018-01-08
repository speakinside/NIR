#ifndef MODELDELEGATE_H
#define MODELDELEGATE_H

#include <QStyledItemDelegate>

class ModelDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit ModelDelegate(QObject *parent = nullptr);
    /*
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
*/
signals:

public slots:
};

#endif // MODELDELEGATE_H
