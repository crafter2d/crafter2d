#include "newworlddialog.h"
#include "ui_newworlddialog.h"

#include <engine/world/layer.h>
#include <engine/world/layertype.h>
#include <engine/world/world.h>

#include "newlayerdialog.h"
#include "tileworld.h"

// static
TileWorld* NewWorldDialog::getWorld()
{
    TileWorld* presult = NULL;

    NewWorldDialog worlddialog;

    if ( worlddialog.exec() == DialogCode::Accepted )
    {
        World* pworld = new World();
        pworld->setName(String::fromUtf8(worlddialog.ui->editName->text().toUtf8().data()));
        pworld->setLayerType(worlddialog.ui->radioTopDown->isChecked() ? ETopDown : EIsoDiamond);

        presult = TileWorld::fromWorld(pworld);

        if ( worlddialog.needsLayer() )
        {
            LayerDefinition* pdefinition = NewLayerDialog::getLayer();
            if ( pdefinition != NULL )
            {
                presult->addMap(pdefinition);
            }
        }
    }
    return presult;
}

NewWorldDialog::NewWorldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewWorldDialog)
{
    ui->setupUi(this);

    QButtonGroup* pgroup = new QButtonGroup(this);
    pgroup->addButton(ui->radioTopDown);
    pgroup->addButton(ui->radioIsoMetric);
}

NewWorldDialog::~NewWorldDialog()
{
    delete ui;
}

// - Get/set

bool NewWorldDialog::needsLayer() const
{
    return ui->checkBox->isChecked();
}
