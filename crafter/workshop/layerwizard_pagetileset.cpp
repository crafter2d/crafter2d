#include "layerwizard_pagetileset.h"
#include "ui_layerwizard_pagetileset.h"

#include <QIntValidator>
#include <QLineEdit>

LayerWizard_PageTileSet::LayerWizard_PageTileSet(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LayerWizard_PageTileSet)
{
    ui->setupUi(this);

    registerField("tileset.create", ui->radioCreate);
    registerField("tileset.name", ui->editName);
    registerField("tileset.width", ui->editWidth);
    registerField("tileset.height", ui->editHeight);
    registerField("tileset.count", ui->editCount);
    registerField("tileset.image", ui->editImage);

    ui->editWidth->setValidator(new QIntValidator(1, 99999));
    ui->editHeight->setValidator(new QIntValidator(1, 99999));
    ui->editCount->setValidator(new QIntValidator(1, 99999));

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
