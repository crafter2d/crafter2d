#include "layerwizard_pagetileset.h"
#include "ui_layerwizard_pagetileset.h"

LayerWizard_PageTileSet::LayerWizard_PageTileSet(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LayerWizard_PageTileSet)
{
    ui->setupUi(this);

    registerField("tileset.create", ui->radioCreate);

    connect(ui->radioCreate, SIGNAL(toggled(bool)), SLOT(on_action_toggled(bool)));
}

LayerWizard_PageTileSet::~LayerWizard_PageTileSet()
{
    delete ui;
}

// - Slots

void LayerWizard_PageTileSet::on_action_toggled(bool checked)
{
    ui->groupBox->setEnabled(checked);
    ui->listView->setEnabled(!checked);
}
