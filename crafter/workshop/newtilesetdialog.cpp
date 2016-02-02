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
}

NewTileSetDialog::~NewTileSetDialog()
{
    delete ui;
}

void NewTileSetDialog::on_btnSelectPath_clicked()
{
    QString path = Project::getActiveProject().getBasePath() + QDir::separator() + "Images";
    QString tilesetpath = QFileDialog::getExistingDirectory(nullptr, "Select image directory", path);
    if ( !tilesetpath.isNull() )
    {
        if ( tilesetpath.startsWith(path) )
        {
            ui->editImagePath->setText(tilesetpath.right(tilesetpath.length() - path.length()));
        }
    }
}

void NewTileSetDialog::set(const TileSet& tileset)
{
    ui->editName->setText(QFileInfo(tileset.getResourceName()).baseName());
    ui->editName->setReadOnly(true);

    ui->editImagePath->setText(tileset.getImagePath());
}

void NewTileSetDialog::apply(TileSet& tileset)
{
   tileset.setImagePath(ui->editImagePath->text());
}
