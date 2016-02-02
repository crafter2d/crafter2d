#ifndef NEWTILESETDIALOG_H
#define NEWTILESETDIALOG_H

#include <QDialog>

namespace Ui {
class newtilesetdialog;
}

class TileSet;

class NewTileSetDialog : public QDialog
{
    Q_OBJECT

public:
    static void NewTileSetDialog::edit(TileSet& tileset);

    explicit NewTileSetDialog(QWidget *parent = 0);
    ~NewTileSetDialog();

private slots:
    void on_btnSelectPath_clicked();

private:

    void set(const TileSet& tileset);
    void apply(TileSet& tileset);

    Ui::newtilesetdialog *ui;
};

#endif // NEWTILESETDIALOG_H
