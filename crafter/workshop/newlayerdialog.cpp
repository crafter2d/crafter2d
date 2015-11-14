#include "newlayerdialog.h"
#include "ui_newlayerdialog.h"

#include "world/tilemap.h"
#include "world/tilemapdesc.h"

// static
TileMap* NewLayerDialog::getMap()
{
    TileMap* presult = NULL;

    NewLayerDialog dialog;
    if ( dialog.exec() == DialogCode::Accepted )
    {
        TileMapDesc desc;
        desc.name = dialog.getName();
        desc.effect = dialog.getEffect();
        desc.tileset = dialog.getTileset();
        desc.size = QSize(dialog.getWidth(), dialog.getHeight());

        presult = new TileMap(desc);
    }

    return presult;
}

// static
void NewLayerDialog::edit(TileMap& map)
{
    NewLayerDialog dialog;
    dialog.setDesc(map.getDesc());
    if ( dialog.exec() == DialogCode::Accepted )
    {
        TileMapDesc& desc = map.getDesc();
        desc.name = dialog.getName();
        desc.effect = dialog.getEffect();
        desc.tileset = dialog.getTileset();
        desc.size = QSize(dialog.getWidth(), dialog.getHeight());
    }
}

NewLayerDialog::NewLayerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewLayerDialog)
{
    ui->setupUi(this);
}

NewLayerDialog::~NewLayerDialog()
{
    delete ui;
}

// - Get/set

QString NewLayerDialog::getName() const
{
    return ui->editName->text();
}

int NewLayerDialog::getWidth() const
{
    return ui->spinWidth->value();
}

int NewLayerDialog::getHeight() const
{
    return ui->spinHeight->value();
}

QString NewLayerDialog::getEffect() const
{
    return ui->editEffect->text();
}

QString NewLayerDialog::getTileset() const
{
    if ( ui->TileRadioGroup->checkedId() == 0 )
    {
        return QString("");
    }
    else
    {
        QModelIndexList& selection = ui->listView->selectionModel()->selectedRows();
        Q_ASSERT( selection.size() == 1 );
        const QModelIndex& index = selection.at(0);
        return QString("images/block.xml");
    }
    //return ui->editTileset->text();
}

void NewLayerDialog::setDesc(const TileMapDesc& desc)
{
    ui->editName->setText(desc.name);
    ui->editEffect->setText(desc.effect);
    ui->spinWidth->setValue(desc.size.width());
    ui->spinHeight->setValue(desc.size.height());

    ui->radioExisting->setChecked(true);
}
