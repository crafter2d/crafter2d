#ifndef NEWTILESETDIALOG_H
#define NEWTILESETDIALOG_H

#include <QDialog>

namespace Ui {
class newtilesetdialog;
}

class QTileSet;

class NewTileSetDialog : public QDialog
{
    Q_OBJECT

public:
    static void NewTileSetDialog::edit(QTileSet& tileset);

    explicit NewTileSetDialog(QWidget *parent = 0);
    ~NewTileSetDialog();

private slots:
    void on_btnImport_clicked();

private:

    void setupUp();
    void set(const QTileSet& tileset);
    void apply(QTileSet& tileset);

    Ui::newtilesetdialog *ui;
};

#endif // NEWTILESETDIALOG_H
