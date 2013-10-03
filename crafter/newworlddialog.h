#ifndef NEWWORLDDIALOG_H
#define NEWWORLDDIALOG_H

#include <QDialog>

namespace Ui {
class NewWorldDialog;
}

class TileWorld;

class NewWorldDialog : public QDialog
{
    Q_OBJECT

public:
    static TileWorld* getWorld();

    explicit NewWorldDialog(QWidget *parent = 0);
    ~NewWorldDialog();

private:

  // get/set
    bool needsLayer() const;

    Ui::NewWorldDialog *ui;
};

#endif // NEWWORLDDIALOG_H
