#include "worldwizard_pagelayer.h"
#include "ui_worldwizard_pagelayer.h"

#include <QFileInfo>

#include "project/projectmanager.h"
#include "world/tileset.h"

#include "project.h"

WorldWizard_PageLayer::WorldWizard_PageLayer(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WorldWizard_PageLayer)
{
    ui->setupUi(this);

    registerField("layer.create", ui->checkCreate);
    registerField("layer.name", ui->editName);
    registerField("layer.width", ui->editWidth);
    registerField("layer.height", ui->editHeight);
    registerField("layer.tileset", ui->cmbTileSet, "currentText", "currentTextChanged");

    connect(ui->checkCreate, SIGNAL(stateChanged(int)), SLOT(on_checkCreate_statechanged(int)));

    fillTileSets();
}

WorldWizard_PageLayer::~WorldWizard_PageLayer()
{
    delete ui;
}

// - Slots

void WorldWizard_PageLayer::on_checkCreate_statechanged(int state)
{
    bool create = state == Qt::Checked;
    ui->groupBox->setEnabled(create);
}

// - Operations

void WorldWizard_PageLayer::fillTileSets()
{
    auto& project = *ProjectManager::getInstance().getActiveProject();
    auto& tilesets = project.getTileSets();
    for ( auto ptileset : tilesets )
    {
        auto name = QFileInfo(project.getFilePath(ptileset->getResourceName())).baseName();
        ui->cmbTileSet->addItem(name);
    }
}
