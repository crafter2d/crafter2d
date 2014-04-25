#include "tileview.h"
#include "ui_tileview.h"

#include <QDebug>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>

#include "world/tilebound.h"
#include "tileworld.h"
#include "undocleartile.h"
#include "undosettile.h"

TileView::TileView():
    QWidget(NULL),
    ui(new Ui::TileView),
    mTile(),
    mpWorld(NULL),
    mEditMode(eLayerMode),
    mpSelectedBound(NULL),
    mSelectedEdge(eNone),
    mUndoStack(),
    mLevel(TileField::eMid),
    mTransparency(1.0)
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

void TileView::setActiveTile(const Tile* tile)
{
    if ( tile != nullptr )
    {
        mTile = *tile;
    }
    else
    {
        mTile = Tile();
    }
}

TileField::Level TileView::getLevel() const
{
    return mLevel;
}

void TileView::setLevel(TileField::Level level)
{
    mLevel = level;
}

// - Operations

void TileView::undo()
{
    mUndoStack.undo();
}

void TileView::redo()
{
    mUndoStack.redo();
}

// - Painting

void TileView::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setOpacity(mTransparency);
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

    QPalette whitepalette(palette());
    whitepalette.setColor(QPalette::Background, Qt::white);

    this->setAutoFillBackground(true);
    this->setPalette(whitepalette);
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

    update();
}

void TileView::setTransparency(double value)
{
    mTransparency = value;

    update();
}

// - Events

void TileView::keyPressEvent(QKeyEvent *pevent)
{
    switch ( mEditMode )
    {
    case eBoundMode:
        {
            if ( mpWorld->hasSelectedBound() )
            {
                switch ( pevent->key() )
                {
                case Qt::Key_F:
                    mpWorld->getSelectedBound().flip();
                    break;
                case Qt::Key_S:
                    mpWorld->straightenBounds();
                    break;
                }

                repaint();
            }
        }
        break;
    default:
        QWidget::keyPressEvent(pevent);
        break;
    }
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
                if ( mTile.isValid() )
                {
                    mEditMode = ePaintMode;

                    UndoSetTile* pundo = new UndoSetTile(*mpWorld, pevent->pos(), mLevel, mTile);
                    mUndoStack.push(pundo);
                }
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
            UndoClearTile* pundo = new UndoClearTile(*mpWorld, pevent->pos(), mLevel);
            mUndoStack.push(pundo);
            //mpWorld->clearTile(pevent->pos(), mLevel);
            //repaint();
        }
        break;
    }
}

void TileView::mouseReleaseEvent(QMouseEvent *)
{
    if ( mEditMode == ePaintMode )
    {
        // leave paint mode and return to default
        mEditMode = eLayerMode;
    }

    mpSelectedBound = NULL;
}

void TileView::mouseMoveEvent(QMouseEvent* pevent)
{
    switch ( mEditMode )
    {
    case ePaintMode:
        if ( mpWorld->setTile(pevent->pos(), mLevel, mTile) )
        {
            repaint();
        }
        break;

    case eBoundMode:
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
        break;
    }
}
