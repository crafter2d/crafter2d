#ifndef NEWLAYERDIALOG_H
#define NEWLAYERDIALOG_H

#include <QDialog>

namespace Ui {
class NewLayerDialog;
}

struct LayerDefinition;

class NewLayerDialog : public QDialog
{
    Q_OBJECT

public:
    static LayerDefinition* getLayer();
    static void edit(LayerDefinition& definition);

    explicit NewLayerDialog(QWidget *parent = 0);
    ~NewLayerDialog();

  // get/set
    QString getName() const;

    int getWidth() const;
    int getHeight() const;

    QString getEffect() const;
    QString getTileset() const;

private:
    Ui::NewLayerDialog *ui;
};

#endif // NEWLAYERDIALOG_H
