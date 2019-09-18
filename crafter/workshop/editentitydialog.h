#ifndef EDITENTITYDIALOG_H
#define EDITENTITYDIALOG_H

#include <QDialog>

class TileEntity;
class TileWorld;

namespace Ui {
class EditEntityDialog;
}

class EditEntityDialog : public QDialog
{
    Q_OBJECT

public:
    static void edit(TileWorld& world, TileEntity& entity);

private:
    EditEntityDialog(TileWorld& world, TileEntity& entity, QWidget *parent = nullptr);
    ~EditEntityDialog();

    Ui::EditEntityDialog *ui;

  // members
    TileEntity& mEntity;
};

#endif // EDITENTITYDIALOG_H
