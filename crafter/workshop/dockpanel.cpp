#include "dockpanel.h"

#include <QMdiArea>

#include "mainwindow.h"
#include "tileview.h"
#include "tileviewwindow.h"

DockPanel::DockPanel(MainWindow &parent) :
    QDockWidget(&parent)
{
    connect(&parent.getMdiArea(),
            SIGNAL(subWindowActivated(QMdiSubWindow*)),
            SLOT(on_subWindowActivated(QMdiSubWindow*)));
}

// - Get/set

MainWindow& DockPanel::getMainWindow()
{
    return *static_cast<MainWindow*>(parent());
}

// - Slots

void DockPanel::on_subWindowActivated(QMdiSubWindow *psubwindow)
{
    TileWorld* pworld = NULL;
    if ( psubwindow != NULL )
    {
        TileViewWindow* pwindow = dynamic_cast<TileViewWindow*>(psubwindow->widget());
        if ( pwindow != NULL )
        {
            TileView& view = pwindow->getTileView();
            pworld = &view.getWorld();
        }
    }

    worldActivated(pworld);
}
