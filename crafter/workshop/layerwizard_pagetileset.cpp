#include "layerwizard_pagetileset.h"
#include "ui_layerwizard_pagetileset.h"

#include <QIntValidator>
#include <QLineEdit>

#include "tilesetmodel.h"

LayerWizard_PageTileSet::LayerWizard_PageTileSet(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LayerWizard_PageTileSet),
    mpTileSetModel(nullptr)
{
    ui->setupUi(this);

    registerField("tileset.create", ui->radioCreate);
    registerField("tileset.name", ui->editName);
    registerField("tileset.width", ui->editWidth);
    registerField("tileset.height", ui->editHeight);
    registerField("tileset.count", ui->editCount);
    registerField("tileset.image", ui->editImage);
    registerField("tileset.tilesetid", ui->listView);

    ui->editWidth->setValidator(new QIntValidator(1, 99999));
    ui->editHeight->setValidator(new QIntValidator(1, 99999));
    ui->editCount->setValidator(new QIntValidator(1, 99999));

    mpTileSetModel = new TileSetModel();
    ui->listView->setModel(mpTileSetModel);

    connect(ui->radioCreate, SIGNAL(toggled(bool)), SLOT(on_action_toggled(bool)));
    connect(ui->listView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(on_listSelectionChanged(QItemSelection,QItemSelection)));

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

void LayerWizard_PageTileSet::on_listSelectionChanged(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
    // we only support single row selection
    QModelIndexList items = selected.indexes();
    Q_ASSERT(items.size() == 1);

    const QModelIndex index = selected.indexes().at(0);
    setField("tileset.tilesetid", index.row());
}
