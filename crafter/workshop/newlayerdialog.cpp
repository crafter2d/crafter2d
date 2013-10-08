#include "newlayerdialog.h"
#include "ui_newlayerdialog.h"

#include <engine/world/layer.h>

// static
LayerDefinition* NewLayerDialog::getLayer()
{
    LayerDefinition* presult = NULL;
    NewLayerDialog dialog;
    if ( dialog.exec() == DialogCode::Accepted )
    {
        presult = new LayerDefinition();
        presult->name = String::fromUtf8(dialog.getName().toUtf8().data());
        presult->width = dialog.getWidth();
        presult->height = dialog.getHeight();
        presult->tileset = String::fromUtf8(dialog.getTileset().toUtf8().data());
        presult->effect = String::fromUtf8(dialog.getEffect().toUtf8().data());
    }

    return presult;
}

// static
void NewLayerDialog::edit(LayerDefinition& definition)
{

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
    return ui->editTileset->text();
}
