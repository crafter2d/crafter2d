#include "newtilesetdialog.h"
#include "ui_newtilesetdialog.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "world/tileset.h"
#include "world/tilesetwriter.h"

#include "project.h"

void NewTileSetDialog::edit(TileSet& tileset)
{
    NewTileSetDialog dlg;
    dlg.set(tileset);
    if ( dlg.exec() == QDialog::Accepted )
    {
        dlg.apply(tileset);

        TileSetWriter writer;
        writer.write(tileset);
    }
}

NewTileSetDialog::NewTileSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newtilesetdialog)
{
    ui->setupUi(this);

    setupUp();
}

NewTileSetDialog::~NewTileSetDialog()
{
    delete ui;
}

void NewTileSetDialog::setupUp()
{
    QStringList filters;
    filters << "*.dds";
    QString path = Project::getActiveProject().getBasePath() + QDir::separator() + "Images";
    QDir dir(path);
    dir.setNameFilters(filters);

    QStringList images;
    QFileInfoList list = dir.entryInfoList();
    for ( auto& info : list )
    {
        images << info.baseName();
    }
    ui->cmbImage->addItems(images);
}

void NewTileSetDialog::set(const TileSet& tileset)
{
    ui->editName->setText(QFileInfo(tileset.getResourceName()).baseName());
    ui->editName->setReadOnly(true);

    ui->editCount->setText(QString::number(tileset.getTileCount()));
    ui->editWidth->setText(QString::number(tileset.getTileSize().width()));
    ui->editHeight->setText(QString::number(tileset.getTileSize().height()));

    QString path = Project::getActiveProject().getFilePath(tileset.getTileMap());
    ui->cmbImage->setCurrentText(QFileInfo(path).baseName());
}

void NewTileSetDialog::apply(TileSet& tileset)
{
    tileset.setTileCount(ui->editCount->text().toInt());
    tileset.setTileSize(QSize(ui->editWidth->text().toInt(), ui->editHeight->text().toInt()));
    tileset.setTileMap(tr("Images") + QDir::separator() + ui->cmbImage->currentText());
}

void NewTileSetDialog::on_btnImport_clicked()
{
    QString imagefile = QFileDialog::getOpenFileName(nullptr, tr("Select image"), "", "Images (*.dds *.png *.bmp");
    if ( imagefile.isNull() )
    {
        return;
    }

    QFileInfo info(imagefile);
    Project& project = Project::getActiveProject();
    QString finalpath = project.getBasePath() + QDir::separator() + "images" + QDir::separator() + info.fileName();
    if ( finalpath != imagefile )
    {
        bool copyit = true;
        if ( QFile::exists(finalpath) )
        {
            QMessageBox::StandardButton result =
                    QMessageBox::warning(this, "Crafter Workshop",
                                         "There is already an image with that name.\nDo you want to overwrite the existing file?",
                                         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if ( result == QMessageBox::Cancel )
            {
                return;
            }

            copyit = result == QMessageBox::Yes;
        }

        if ( copyit && !QFile::copy(imagefile, finalpath) )
        {
            QMessageBox::critical(this, "Crafter Workshop", "Could not copy file to demo folder.", QMessageBox::Ok);
            return;
        }
    }

    ui->cmbImage->addItem(info.baseName());
}
