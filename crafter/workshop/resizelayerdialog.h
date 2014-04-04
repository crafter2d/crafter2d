#ifndef RESIZELAYERDIALOG_H
#define RESIZELAYERDIALOG_H

#include <QDialog>
#include <QSize>

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

  // overloads
    virtual void accept() override;

private slots:


private:

  // data
    Ui::ResizeLayerDialog *ui;
    QSize mOrigSize;
};

#endif // RESIZELAYERDIALOG_H
