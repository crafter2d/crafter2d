#include "tileview.h"
#include "ui_tileview.h"

#include <QDebug>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>

#include "world/tilebound.h"
#include "tileworld.h"

TileView::TileView():
    QWidget(NULL),
    ui(new Ui::TileView),
    mTile(),
    mpWorld(NULL),
    mEditMode(eLayerMode),
    mpSelectedBound(NULL),
    mSelectedEdge(eNone),
    mLevel(QTileField::eMid)
{
    ui->setupUi(this);

    setFocusPolicy(Qt::StrongFocus);
}

TileView::~TileView()
{
    delete ui;
}

// - Get/set

bool TileView::hasWorld() const
{
    return mpWorld != NULL;
}

TileWorld& TileView::getWorld()
{
    Q_ASSERT(mpWorld != NULL);
    return *mpWorld;
}

void TileView::setWorld(TileWorld& world)
{
    if ( mpWorld != &world )
    {
        mpWorld = &world;
        prepare();

        connect(mpWorld, SIGNAL(worldDirty()), SLOT(worldUpdated()));
    }
}

TileView::EditMode TileView::getEditMode() const
{
    return mEditMode;
}

void TileView::setEditMode(EditMode mode)
{
    mEditMode = mode;
}

void TileView::setActiveTile(const Tile& tile)
{
    mTile = tile;
}

QTileField::Level TileView::getLevel() const
{
    return mLevel;
}

void TileView::setLevel(QTileField::Level level)
{
    mLevel = level;
}

// - Painting

void TileView::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    mpWorld->paint(painter);
}

// - Operations

void TileView::prepare()
{
    if ( mpWorld != NULL )
    {
        QSize minsize = mpWorld->getMinimumSize();
        setMinimumSize(minsize);
    }
}

bool TileView::selectBoundEdge(QMouseEvent *pevent)
{
    if ( QVector2D(mpSelectedBound->left() - pevent->pos()).lengthSquared() <= 3 )
    {
        // selected the edge for dragging
        mSelectedEdge = eLeft;
        return true;
    }
    else if ( QVector2D(mpSelectedBound->right() - pevent->pos()).lengthSquared() <= 3 )
    {
        // selected the edge for dragging
        mSelectedEdge = eRight;
        return true;
    }

    return false;
}

void TileView::straightenBounds()
{

}

// - Slots

void TileView::worldUpdated()
{
    prepare();

    repaint();
}

// - Events

void TileView::keyPressEvent(QKeyEvent *pevent)
{
    switch ( mEditMode )
    {
    case eBoundMode:
        {
            switch ( pevent->key() )
            {
            case Qt::Key_F:
                if ( mpWorld->hasSelectedBound() )
                {
                    mpWorld->getSelectedBound().flip();
                    repaint();
                    return;
                }
                break;
            case Qt::Key_S:
                mpWorld->straightenBounds();
                break;
            }

        }
        break;
    }

    QWidget::keyPressEvent(pevent);
}

void TileView::mousePressEvent(QMouseEvent *pevent)
{
    if ( pevent == NULL )
    {
        return;
    }

    switch ( pevent->button() )
    {
    case Qt::LeftButton:
        switch ( mEditMode )
        {
        case eLayerMode:
            if ( mpWorld->setTile(pevent->pos(), mLevel, mTile) )
            {
                repaint();
            }
            break;
        case eBoundMode:
            {
                if ( pevent->modifiers() & Qt::CTRL )
                {
                    TileBound& bound = mpWorld->addBound(pevent->pos());

                    mpSelectedBound = &bound;
                    mSelectedEdge = eRight;

                    mpWorld->setSelectedBound(&bound);
                }
                else
                {
                    TileBound* pbound = mpWorld->findBound(pevent->pos());
                    if ( pbound != NULL )
                    {
                        mpSelectedBound = pbound;

                        selectBoundEdge(pevent);

                        mpWorld->setSelectedBound(pbound);
                    }
                }

                repaint();
            }
            break;
        }
        break;
    case Qt::RightButton:
        if ( mEditMode == eLayerMode )
        {
            mpWorld->clearTile(pevent->pos(), mLevel);
            repaint();
        }
        break;
    }
}

void TileView::mouseReleaseEvent(QMouseEvent *)
{
    mpSelectedBound = NULL;
}

void TileView::mouseMoveEvent(QMouseEvent* pevent)
{
    if ( mpSelectedBound != NULL )
    {
        switch ( mSelectedEdge )
        {
        case eLeft:
            mpSelectedBound->setLeft(pevent->localPos());
            break;
        case eRight:
            mpSelectedBound->setRight(pevent->localPos());
            break;
        case eNone:
            // need offset of previous pos and this pevent->pos()
            // mpSelectedBound->move();
            break;
        }

        repaint();
    }
}
