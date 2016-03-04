#ifndef TILEPROPERTIESDIALOG_H
#define TILEPROPERTIESDIALOG_H

#include <QDialog>

#include "tileselector.h"

namespace Ui {
class TilePropertiesDialog;
}

class TilePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    static void edit(TileContextInfo &info);

    TilePropertiesDialog(TileContextInfo& info, QWidget *parent = 0);
    ~TilePropertiesDialog();

private:
    void apply();

    Ui::TilePropertiesDialog *ui;
    TileContextInfo mInfo;
};

#endif // TILEPROPERTIESDIALOG_H
