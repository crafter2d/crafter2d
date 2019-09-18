#include "editentitydialog.h"
#include "ui_editentitydialog.h"

#include "world/tileentity.h"
#include "world/tilemap.h"
#include "world/tileworld.h"

// static
void EditEntityDialog::edit(TileWorld& world, TileEntity& entity)
{
    EditEntityDialog dlg(world, entity);
    if ( dlg.exec() == EditEntityDialog::Accepted )
    {
        TileMap* pmap = world.findMap(dlg.ui->comboBox->currentText());
        entity.setTileMap(pmap);
        entity.setSortOrder(dlg.ui->sbSortOrder->value());
    }
}

EditEntityDialog::EditEntityDialog(TileWorld& world, TileEntity& entity, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditEntityDialog),
    mEntity(entity)
{
    ui->setupUi(this);
    ui->sbSortOrder->setValue(entity.sortOrder());
    for ( auto pmap : world.getMaps() )
    {
        ui->comboBox->addItem(pmap->getName());
    }
    if ( entity.tileMap() )
    {
        ui->comboBox->setCurrentText(entity.tileMap()->getName());
    }
}

EditEntityDialog::~EditEntityDialog()
{
    delete ui;
}
