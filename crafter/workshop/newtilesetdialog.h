#ifndef NEWTILESETDIALOG_H
#define NEWTILESETDIALOG_H

#include <QDialog>

#include "world/tileset.h"
#include "tileselector.h"

namespace Ui {
class newtilesetdialog;
}


class NewTileSetDialog : public QDialog
{
    Q_OBJECT

public:
    static void NewTileSetDialog::edit(TileSet& tileset);

    explicit NewTileSetDialog(TileSet &tileset, QWidget *parent = 0);
    ~NewTileSetDialog();

private slots:
    void on_btnSelectPath_clicked();
    void on_contentMenuPopup_triggered(TileContextInfo &info);

    void on_actionProperties_triggered();

private:

  // operations
    void setupUi();
    void apply(TileSet& tileset);
    void refresh();

    Ui::newtilesetdialog *ui;
    TileSelector* mpSelector;
    TileSet       mTileSet;
    TileContextInfo mInfo;
};

#endif // NEWTILESETDIALOG_H
