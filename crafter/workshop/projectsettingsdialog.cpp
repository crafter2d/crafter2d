#include "projectsettingsdialog.h"
#include "ui_projectsettingsdialog.h"

#include <QtGlobal>
#include <QDebug>
#include <QLineEdit>

#include <iostream>

#include "project.h"

ProjectSettingsDialog::ProjectSettingsDialog(QWidget* pparent) :
    QDialog(pparent),
    ui(new Ui::ProjectSettingsDialog()),
    mSettings(Project::getActiveProject().getSettings())
{
    ui->setupUi(this);

    for ( auto& path : mSettings.getBuildPaths() ) {
        auto listitem = new QListWidgetItem(path);
        listitem->setFlags(listitem->flags() | Qt::ItemIsEditable);
        ui->listBuildPaths->addItem(listitem);
    }

    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(on_ok()));
    connect(ui->btnAddPath, SIGNAL(clicked()), SLOT(on_addPath_triggered()));
    connect(ui->listBuildPaths->itemDelegate(), SIGNAL(commitData(QWidget*)), SLOT(on_commitData(QWidget*)));
}

void ProjectSettingsDialog::on_addPath_triggered()
{
    QString path;
    auto listitem = new QListWidgetItem(path);
    listitem->setFlags(listitem->flags() | Qt::ItemIsEditable);
    mSettings.addBuildPath(path);
    ui->listBuildPaths->addItem(listitem);
}

void ProjectSettingsDialog::on_commitData(QWidget* peditor)
{
    QString strNewText = reinterpret_cast<QLineEdit*>(peditor)->text();
    int row = ui->listBuildPaths->currentRow();
    mSettings.updateBuildPath(row, std::move(strNewText));
}

void ProjectSettingsDialog::on_ok()
{
    Project::getActiveProject().setSettings(std::move(mSettings));
    this->accept();
}
