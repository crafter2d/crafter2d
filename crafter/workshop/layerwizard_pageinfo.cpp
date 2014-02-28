#include "layerwizard_pageinfo.h"
#include "ui_layerwizard_pageinfo.h"

LayerWizard_PageInfo::LayerWizard_PageInfo(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LayerWizard_PageInfo)
{
    ui->setupUi(this);

    registerField("layer.create", ui->checkCreate);

    connect(ui->checkCreate, SIGNAL(stateChanged(int)), SLOT(on_checkCreate_statechanged(int)));
}

LayerWizard_PageInfo::~LayerWizard_PageInfo()
{
    delete ui;
}

// - Slots

void LayerWizard_PageInfo::on_checkCreate_statechanged(int state)
{
    bool create = state == Qt::Checked;
    ui->groupBox->setEnabled(create);
}
