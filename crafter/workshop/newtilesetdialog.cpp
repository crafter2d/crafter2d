#include "newtilesetdialog.h"
#include "ui_newtilesetdialog.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QContextMenuEvent>

#include "world/tilesetwriter.h"
#include "tileselector.h"

#include "tilepropertiesdialog.h"
#include "project.h"

void NewTileSetDialog::edit(TileSet& tileset)
{
    NewTileSetDialog dlg(tileset);
    if ( dlg.exec() == QDialog::Accepted )
    {
        dlg.apply(tileset);

        TileSetWriter writer;
        writer.write(tileset);
    }
}

NewTileSetDialog::NewTileSetDialog(TileSet& tileset, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newtilesetdialog),
    mpSelector(nullptr),
    mTileSet(tileset),
    mInfo()
{
    ui->setupUi(this);
    setupUi();

    mpSelector = new TileSelector(ui->scrollArea);
    mpSelector->setTileSet(&mTileSet);
    connect(mpSelector, SIGNAL(contextMenuPopup(TileContextInfo&)), SLOT(on_contentMenuPopup_triggered(TileContextInfo&)));
    ui->scrollArea->setWidget(mpSelector);

    mpSelector->show();
}

NewTileSetDialog::~NewTileSetDialog()
{
    delete ui;
}

// - Operations

void NewTileSetDialog::setupUi()
{
    ui->editName->setText(QFileInfo(mTileSet.getResourceName()).baseName());
    ui->editName->setReadOnly(true);

    QString imagePath = mTileSet.getPath();
    ui->editImagePath->setText(imagePath);
}

void NewTileSetDialog::apply(TileSet& tileset)
{
    tileset = std::move(mTileSet);
}

struct CompareNames
{
    bool operator()(const QString& s1,const QString& s2)
    {
        if ( s1.length() < s2.length() )
        {
            return true;
        }
        else if ( s1.length() > s2.length() )
        {
            return false;
        }
        return s1.compare(s2) <= 0;
    }
} compare;

void NewTileSetDialog::refresh()
{
    QStringList filters;
    filters << "*.png";

    Project& project = Project::getActiveProject();

    const QString& imagePath = project.getImagePath();
    const QString path = imagePath + '/' + (mTileSet.empty() ? ui->editImagePath->text() : mTileSet.getPath());

    QDir dir(path);
    QStringList images = dir.entryList(filters, QDir::Files);
    std::sort(images.begin(), images.end(), compare);

    auto& atlas = project.getSpriteAtlas();

    for ( int index = 0; index < images.size(); index++ )
    {
        QString name = dir.absoluteFilePath(images[index]);
        name = name.right(name.length() - imagePath.length() - 1).toLower();

        if ( !mTileSet.contains(name) )
        {
            mTileSet.add(name, atlas.lookup(name));
        }
    }

    mTileSet.update();
}

// - Slots

void NewTileSetDialog::on_btnSelectPath_clicked()
{
    const QString& path = Project::getActiveProject().getImagePath();
    QString tilesetpath = QFileDialog::getExistingDirectory(nullptr, "Select image directory", path);
    if ( !tilesetpath.isNull() && tilesetpath.startsWith(path) )
    {
        QString imagePath = tilesetpath.right(tilesetpath.length() - path.length() - 1);
        if ( imagePath != ui->editImagePath->text() )
        {
            ui->editImagePath->setText(imagePath);

            mTileSet.clear();

            refresh();
        }
    }
}

void NewTileSetDialog::on_actionProperties_triggered()
{
    if ( mInfo.tile != TileSet::INVALID_TILE )
    {
        TilePropertiesDialog::edit(mInfo);
    }
}

void NewTileSetDialog::on_contentMenuPopup_triggered(TileContextInfo& info)
{
    if ( info.tile != TileSet::INVALID_TILE )
    {
        mInfo = info;

        QMenu menu(this);
        menu.addAction(ui->actionProperties);
        menu.exec(info.pos);

        mInfo.tile = TileSet::INVALID_TILE;
    }
}
