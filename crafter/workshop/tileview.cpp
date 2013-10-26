#include "tileview.h"
#include "ui_tileview.h"

#include <QDebug>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>

#include <engine/world/bound.h>
#include <engine/world/layer.h>
#include <engine/world/world.h>

#include "tileworld.h"
#include "undosettile.h"

TileView::TileView():
    QWidget(NULL),
    ui(new Ui::TileView),
    mTile(),
    mpWorld(NULL),
    mEditMode(eLayerMode),
    mpSelectedBound(NULL),
    mSelectedEdge(eNone),
    mLevel(LayerLevel::eMid)
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

LayerLevel TileView::getLevel() const
{
    return mLevel;
}

void TileView::setLevel(LayerLevel level)
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
    Vector pos(pevent->pos().x(), pevent->pos().y());
    if ( mpSelectedBound->getLeft().distance(pos) <= 3 )
    {
        // selected the edge for dragging
        mSelectedEdge = eLeft;
        return true;
    }
    else if ( mpSelectedBound->getRight().distance(pos) <= 3 )
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

void TileView::undo()
{
    mUndoStack.undo();
}

void TileView::redo()
{
    mUndoStack.redo();
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
            {
                UndoSetTile* pundo = new UndoSetTile(*mpWorld, pevent->pos(), mLevel, mTile);
                mUndoStack.push(pundo);

                repaint();
            }
            break;
        case eBoundMode:
            {
                if ( pevent->modifiers() & Qt::CTRL )
                {
                    Bound& bound = mpWorld->addBound(pevent->pos());

                    mpSelectedBound = &bound;
                    mSelectedEdge = eRight;

                    mpWorld->setSelectedBound(&bound);
                }
                else
                {
                    Bound* pbound = mpWorld->findBound(pevent->pos());
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
        Vector pos(pevent->localPos().x(), pevent->localPos().y());

        switch ( mSelectedEdge )
        {
        case eLeft:
            mpSelectedBound->setLeft(pos);
            break;
        case eRight:
            mpSelectedBound->setRight(pos);
            break;
        case eNone:
            // need offset of previous pos and this pevent->pos()
            // mpSelectedBound->move();
            break;
        }

        repaint();
    }
}
