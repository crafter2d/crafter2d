#ifndef NEWLAYERDIALOG_H
#define NEWLAYERDIALOG_H

#include <QDialog>

namespace Ui {
class NewLayerDialog;
}

class TileMap;
struct TileMapDesc;

class NewLayerDialog : public QDialog
{
    Q_OBJECT

public:
    static TileMap* getMap();
    static void edit(TileMap& map);

    explicit NewLayerDialog(QWidget *parent = nullptr);
    ~NewLayerDialog();

  // get/set
    QString getName() const;

    int getWidth() const;
    int getHeight() const;

    QString getEffect() const;
    QString getTileset() const;

    void setDesc(const TileMapDesc& desc);

private:
    Ui::NewLayerDialog *ui;
};

#endif // NEWLAYERDIALOG_H
