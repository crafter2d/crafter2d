#include "tileselector.h"

#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>

#include <qmath.h>

#include "world/tile.h"
#include "world/tileset.h"

TileSelector::TileSelector(QWidget *parent) :
    QAbstractScrollArea(parent),
    mpTileSet(nullptr),
    mSelectedIndex(-1),
    mHorizontalTiles(0),
    mScrollPos(0)
{
}

// - Get/set

const QTileSet& TileSelector::getTileSet() const
{
    Q_ASSERT(mpTileSet != nullptr);
    return *mpTileSet;
}

void TileSelector::setTileSet(const QTileSet *ptileset)
{
    if ( mpTileSet != ptileset )
    {
        mpTileSet = ptileset;
        tilesetChanged();
    }
}

const Tile& TileSelector::get(int index)
{
    return (*mpTileSet)[index];
}

// - Overrides

void TileSelector::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(viewport());
    painter.translate(0, mScrollPos);

    if ( mpTileSet != nullptr )
    {
        int count = mpTileSet->getTileCount();

        int x = 2, y = 2;
        for ( int index = 1; index <= count; ++index )
        {
            const Tile& tile = get(index-1);

            if ( (index - 1) == mSelectedIndex )
            {
                const QBrush& previousbrush = painter.brush();
                const QPen& previouspen = painter.pen();

                const QSize& tilesize = tile.getSize();
                QRectF bounds(x-2, y-2, tilesize.width() + 4, tilesize.height()+2);

                QColor color;
                color.setRgb(200, 200, 200);
                QLinearGradient gradient(QPointF(bounds.width() / 2, bounds.top()), QPointF(bounds.width()/2, bounds.bottom()));
                gradient.setColorAt(0, QColor(240, 240, 240));
                gradient.setColorAt(1, color);

                QPen pen(color);
                QBrush brush(gradient);
                painter.setBrush(brush);
                painter.setPen(pen);

                painter.drawRoundedRect(bounds, 2, 2);

                painter.setBrush(previousbrush);
                painter.setPen(previouspen);
            }

            tile.paint(painter, x, y);

            if ( index % mHorizontalTiles == 0 )
            {
                x = 2;
                y += mpTileSet->getTileSize().height() + 4;
            }
            else
            {
                x += mpTileSet->getTileSize().width() + 4;
            }
        }
    }
}

void TileSelector::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    tilesetChanged();
}

void TileSelector::scrollContentsBy(int /*dx*/, int dy)
{
    mScrollPos += dy;

    viewport()->scroll(0, dy);
}

void TileSelector::mousePressEvent(QMouseEvent *pevent)
{
    if ( mpTileSet != nullptr )
    {
        if ( pevent->modifiers() & Qt::CTRL )
        {
            clearSelection();
        }
        else
        {
            const QSize& tilesize = mpTileSet->getTileSize();
            QPoint mousepos = pevent->pos();
            int tilex = mousepos.x() / (tilesize.width() + 4.0f);
            int tiley = (mousepos.y() - mScrollPos) / (tilesize.height() + 4.0f);
            int tileindex = tiley * mHorizontalTiles + tilex;

            if ( tileindex < mpTileSet->getTileCount() )
            {
                setSelection(tileindex);
            }
        }

    }
}

// - Operations

void TileSelector::tilesetChanged()
{
    QScrollBar* vbar = verticalScrollBar();

    int height = 0;
    if ( mpTileSet != nullptr )
    {
        const QSize& tilesize = mpTileSet->getTileSize();
        const QSize& windowSize = contentsRect().size();

        mHorizontalTiles = qFloor(static_cast<float>(windowSize.width()) / (tilesize.width() + 2));

        int lines = qCeil(static_cast<float>(mpTileSet->getTileCount()) / mHorizontalTiles);
        height = lines * (tilesize.height() + 4);

        vbar->setPageStep(viewport()->height());
        vbar->setSingleStep(tilesize.height());
    }

    vbar->setRange(0, height - viewport()->height());

    viewport()->update();
}

void TileSelector::setSelection(int index)
{
    mSelectedIndex = index;

    emit tileSelected(index == -1 ? nullptr : &get(index));

    viewport()->update();
}

void TileSelector::clearSelection()
{
    setSelection(-1);
}
