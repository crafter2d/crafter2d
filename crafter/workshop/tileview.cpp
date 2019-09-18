#include "tileview.h"
#include "ui_tileview.h"

#include <QDebug>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>

#include "world/tilebound.h"
#include "world/tileentity.h"
#include "world/tileset.h"
#include "world/tileworld.h"

#include "editentitydialog.h"
#include "undocleartile.h"
#include "undosettile.h"
#include "mainwindow.h"

TileView::TileView():
    QWidget(nullptr),
    ui(new Ui::TileView),
    mTile(TileSet::INVALID_TILE),
    mpWorld(nullptr),
    mEditMode(eLayerMode),
    mpSelectedBound(nullptr),
    mpSelectedEntity(nullptr),
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
    return mpWorld != nullptr;
}

TileWorld& TileView::getWorld()
{
    Q_ASSERT(mpWorld != nullptr);
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

void TileView::setActiveTile(int tile)
{
    mTile = tile;
}

TileField::Level TileView::getLevel() const
{
    return mLevel;
}

void TileView::setLevel(TileField::Level level)
{
    mLevel = level;
}

// - Query

MainWindow* TileView::getMainWindow()
{
    QWidget* pparent = parentWidget();
    while ( pparent != nullptr )
    {
        MainWindow* pwindow = dynamic_cast<MainWindow*>(pparent);
        if ( pwindow != nullptr )
        {
            return pwindow;
        }
        pparent = pparent->parentWidget();
    }
    return nullptr;
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
    if ( mpWorld != nullptr )
    {
        QSize minsize = mpWorld->getMinimumSize();
        setMinimumSize(minsize);
    }

    QPalette whitepalette(palette());
    whitepalette.setColor(QPalette::Background, Qt::white);

    this->setAutoFillBackground(true);
    this->setPalette(whitepalette);
}

float distance(const QPointF& p1, const QPointF& p2)
{
    return QVector2D(p1).distanceToPoint(QVector2D(p2));
}

bool TileView::selectBoundEdge(QMouseEvent *pevent)
{
    if ( distance(mpSelectedBound->left(), pevent->pos()) <= 3.0f )
    {
        // selected the edge for dragging
        mSelectedEdge = eLeft;
        return true;
    }
    else if ( distance(mpSelectedBound->right(), pevent->pos()) <= 3.0f )
    {
        // selected the edge for dragging
        mSelectedEdge = eRight;
        return true;
    }
    else
    {
        mSelectedEdge = eNone;
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
                case Qt::Key_Delete:
                    mpWorld->deleteSelectedBound();
                    break;
                }

                repaint();
            }
        }
        break;
    case eObjectMode:
        {
            switch ( pevent->key() )
            {
            case Qt::Key_I:
                // insert the new entity
                MainWindow* pwindow = getMainWindow();
                if ( pwindow != nullptr )
                {
                    Entity* pentity = pwindow->getSelectedEntity();
                    if ( pentity )
                    {
                        QPoint pos = mapFromGlobal(QCursor::pos());
                        auto& entity = mpWorld->addEntity(*pentity);
                        entity.setPosition(pos);
                        repaint();
                    }
                }
                break;
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
    if ( pevent == nullptr )
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
                if ( mTile != TileSet::INVALID_TILE )
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
                    if ( pbound != nullptr )
                    {
                        mpSelectedBound = pbound;

                        selectBoundEdge(pevent);

                        mpWorld->setSelectedBound(pbound);
                    }
                    else
                    {
                        mpSelectedBound = nullptr;
                        mSelectedEdge = eNone;
                    }
                }

                repaint();
            }
            break;
        case eObjectMode:
            {
                TileEntity* pentity = mpWorld->findEntity(pevent->pos());
                if ( pentity != nullptr )
                {
                    mpSelectedEntity = pentity;
                    mStartPos = pevent->pos();
                }
            }
        }
        break;
    case Qt::RightButton:
        if ( mEditMode == eLayerMode )
        {
            mEditMode = eEraseMode;

            UndoClearTile* pundo = new UndoClearTile(*mpWorld, pevent->pos(), mLevel);
            mUndoStack.push(pundo);
        }
        break;
    }
}

void TileView::mouseReleaseEvent(QMouseEvent *)
{
    if ( mEditMode == ePaintMode || mEditMode == eEraseMode )
    {
        // leave paint mode and return to default
        mEditMode = eLayerMode;
    }

    mpSelectedBound = nullptr;
    mpSelectedEntity = nullptr;
}

void TileView::mouseMoveEvent(QMouseEvent* pevent)
{
    switch ( mEditMode )
    {
    case ePaintMode:
        {
            int tile = mpWorld->getTile(pevent->pos(), mLevel);
            if ( tile != mTile )
            {
                UndoSetTile* pundo = new UndoSetTile(*mpWorld, pevent->pos(), mLevel, mTile);
                mUndoStack.push(pundo);
            }
        }
        break;
    case eEraseMode:
        {
            int tile = mpWorld->getTile(pevent->pos(), mLevel);
            if ( tile != TileSet::INVALID_TILE )
            {
                UndoClearTile* pundo = new UndoClearTile(*mpWorld, pevent->pos(), mLevel);
                mUndoStack.push(pundo);
            }
        }
        break;

    case eBoundMode:
        if ( mpSelectedBound != nullptr )
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
    case eObjectMode:
        {
            if ( mpSelectedEntity )
            {
                QPoint offset = pevent->pos() - mStartPos;
                mStartPos = pevent->pos();
                mpSelectedEntity->move(offset);
                repaint();
            }
        }
        break;
    }
}

void TileView::mouseDoubleClickEvent(QMouseEvent *event)
{
    switch ( mEditMode )
    {
    case eObjectMode:
        TileEntity* pentity = mpWorld->findEntity(event->pos());
        if ( pentity != nullptr )
        {
            EditEntityDialog::edit(*mpWorld, *pentity);
        }
        break;
    }
}
