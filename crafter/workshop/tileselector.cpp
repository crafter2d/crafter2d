#include "tileselector.h"

#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QAbstractScrollArea>

#include <qmath.h>

#include "helpers/rotateimagepainter.h"
#include "world/tileset.h"

TileContextInfo::TileContextInfo():
    pos(),
    tileset(nullptr),
    tile(TileSet::INVALID_TILE)
{
}

TileSelector::TileSelector(QAbstractScrollArea *parea) :
    QWidget(nullptr),
    mpScrollArea(parea),
    mpTileSet(nullptr),
    mSelectedTile(-1),
    mHorizontalTiles(0)
{
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setPalette( QPalette( Qt::white ) );
}

TileSelector::~TileSelector()
{
    mpTileSet = nullptr;
}

// - Get/set

const TileSet& TileSelector::getTileSet() const
{
    Q_ASSERT(mpTileSet != nullptr);
    return *mpTileSet;
}

void TileSelector::setTileSet(TileSet* ptileset)
{
    if ( mpTileSet != ptileset )
    {
        mpTileSet = ptileset;
        connect(mpTileSet, SIGNAL(updated()), SLOT(on_tileset_updated()));
        tilesetChanged();
    }
}

// - Overrides

void TileSelector::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);

    if ( mpTileSet != nullptr )
    {
        const QColor grayColor(200, 200, 200);

        int count = mpTileSet->getTileCount();
        const QSize& tilesize = mpTileSet->getTileSize();

        int x = 2, y = 2;
        for ( int index = 0; index <= count; ++index )
        {
            if ( index == mSelectedTile )
            {
                RotateImagePainter rpainter(painter);

                QRectF bounds(x-2, y-2, tilesize.width() + 4, tilesize.height()+2);

                QLinearGradient gradient(QPointF(bounds.width() / 2, bounds.top()), QPointF(bounds.width()/2, bounds.bottom()));
                gradient.setColorAt(0, QColor(240, 240, 240));
                gradient.setColorAt(1, grayColor);

                QPen pen(grayColor);
                QBrush brush(gradient);
                painter.setBrush(brush);
                painter.setPen(pen);

                painter.drawRoundedRect(bounds, 2, 2);
            }

            mpTileSet->paintTile(painter, QPoint(x, y), index);

            if ( (index + 1) % mHorizontalTiles == 0 )
            {
                x = 2;
                y += tilesize.height() + 4;
            }
            else
            {
                x += tilesize.width() + 4;
            }
        }
    }
}

void TileSelector::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    tilesetChanged();
}

void TileSelector::contextMenuEvent(QContextMenuEvent *pevent)
{
    TileContextInfo info;
    info.pos = pevent->globalPos();
    info.tile = mSelectedTile;
    info.tileset = mpTileSet;

    emit contextMenuPopup(info);
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
            int tiley = mousepos.y() / (tilesize.height() + 4.0f);
            int tileindex = tiley * mHorizontalTiles + tilex;

            if ( tileindex < mpTileSet->getTileCount() )
            {
                setSelection(tileindex);
            }
        }

        update();
    }
}

// - Operations

void TileSelector::tilesetChanged()
{
    QScrollBar* vbar = mpScrollArea->verticalScrollBar();

    int height = 0;
    if ( mpTileSet != nullptr )
    {
        const QSize& tilesize = mpTileSet->getTileSize();
        const QSize& windowSize = contentsRect().size();

        mHorizontalTiles = qFloor(static_cast<float>(windowSize.width()) / (tilesize.width() + 2));

        int lines = qCeil(static_cast<float>(mpTileSet->getTileCount()) / mHorizontalTiles);
        height = lines * (tilesize.height() + 4);

        vbar->setPageStep(windowSize.height());
        vbar->setSingleStep(tilesize.height());
    }

    vbar->setRange(0, height - this->height());

    setMinimumHeight(height);
    update();
}

void TileSelector::setSelection(int tile)
{
    if ( mSelectedTile != tile )
    {
        mSelectedTile = tile;
        emit tileSelected(tile);
    }
}

void TileSelector::clearSelection()
{
    setSelection(TileSet::INVALID_TILE);
}

// - Slots

void TileSelector::on_tileset_updated()
{
    mSelectedTile = -1;
    tilesetChanged();
}
