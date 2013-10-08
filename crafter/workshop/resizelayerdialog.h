#ifndef RESIZELAYERDIALOG_H
#define RESIZELAYERDIALOG_H

#include <QDialog>

namespace Ui {
class ResizeLayerDialog;
}

class ResizeLayerDialog : public QDialog
{
    Q_OBJECT

public:
  // statics
    static bool resize(QWidget* parent, QSize &size);

    explicit ResizeLayerDialog(QWidget *parent = 0);
    ~ResizeLayerDialog();

  // get/set
    QSize getLayerSize() const;
    void  setLayerSize(const QSize& size);

private:
    Ui::ResizeLayerDialog *ui;
};

#endif // RESIZELAYERDIALOG_H
