#ifndef TILEDELEGATE_H
#define TILEDELEGATE_H

#include <QStyledItemDelegate>

class TileDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    TileDelegate(QWidget *parent = 0);

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TILEDELEGATE_H
