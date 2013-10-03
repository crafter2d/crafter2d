#include "tiledelegate.h"

#include <QPainter>

#include "tile.h"

static const QSize sMargin(4, 4);

TileDelegate::TileDelegate(QWidget *parent):
    QStyledItemDelegate(parent)
{
}

QSize TileDelegate::sizeHint(const QStyleOptionViewItem & /* option */, const QModelIndex & index) const
{
    Tile tile = index.model()->data(index).value<Tile>();
    return tile.getSize() + sMargin;
}

void TileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant var = index.model()->data(index);
    if ( !var.isNull() )
    {
        Tile tile = var.value<Tile>();
        QSize tilesize = tile.getSize();

        painter->save();

        if ( option.state & QStyle::State_Selected )
            painter->fillRect(option.rect, option.palette.highlight());

        int offsetx = (option.rect.width() / 2) - (tilesize.width() / 2);
        int offsety = (option.rect.height() / 2) - (tilesize.height() / 2);

        tile.paint(*painter, option.rect.x() + offsetx, option.rect.y() + offsety);

        painter->restore();
    }
}
