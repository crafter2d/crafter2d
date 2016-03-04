#include "tilepropertiesdialog.h"
#include "ui_tilepropertiesdialog.h"

#include "world/tileset.h"

void TilePropertiesDialog::edit(TileContextInfo &info)
{
    TilePropertiesDialog dlg(info);
    if ( dlg.exec() == QDialog::Accepted )
    {
        dlg.apply();
    }
}

TilePropertiesDialog::TilePropertiesDialog(TileContextInfo &info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TilePropertiesDialog),
    mInfo(info)
{
    Q_ASSERT(info.tile != TileSet::INVALID_TILE);

    ui->setupUi(this);

    TileSet::Tile& tile = (*info.tileset)[info.tile];

    ui->spinOffsetX->setValue(tile.offset.x());
    ui->spinOffsetX->setMaximum(info.tileset->getTileSize().width());

    ui->spinOffsetY->setValue(tile.offset.y());
    ui->spinOffsetY->setMaximum(info.tileset->getTileSize().width());
}

TilePropertiesDialog::~TilePropertiesDialog()
{
    delete ui;
}

void TilePropertiesDialog::apply()
{
    TileSet::Tile& tile = (*mInfo.tileset)[mInfo.tile];

    tile.offset.setX(ui->spinOffsetX->value());
    tile.offset.setY(ui->spinOffsetY->value());
}
