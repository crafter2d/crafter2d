#include "resizelayerdialog.h"
#include "ui_resizelayerdialog.h"

#include <QMessageBox>

// static
bool ResizeLayerDialog::resize(QWidget* parent, QSize &size)
{
    ResizeLayerDialog dialog(parent);
    dialog.setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::DefaultType));
    dialog.setLayerSize(size);

    if ( dialog.exec() == DialogCode::Accepted )
    {
        size = dialog.getLayerSize();
        return true;
    }

    return false;
}

ResizeLayerDialog::ResizeLayerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResizeLayerDialog),
    mOrigSize()
{
    ui->setupUi(this);
}

ResizeLayerDialog::~ResizeLayerDialog()
{
    delete ui;
}

// - Get/set

QSize ResizeLayerDialog::getLayerSize() const
{
    return QSize(ui->spinWidth->value(), ui->spinHeight->value());
}

void ResizeLayerDialog::setLayerSize(const QSize& size)
{
    mOrigSize = size;

    ui->spinWidth->setValue(size.width());
    ui->spinHeight->setValue(size.height());
}

// - Signal handling

void ResizeLayerDialog::accept()
{
    int width = ui->spinWidth->value();
    if ( width < mOrigSize.width() )
    {
        int result = QMessageBox::warning(this, "Crafter Workshop", "The new size is smaller that the original. Data will be lost after this action.", QMessageBox::Yes | QMessageBox::No);
        if ( result == QMessageBox::Yes )
        {
            done(QDialog::Accepted);
        }
    }
}
