#include "resizelayerdialog.h"
#include "ui_resizelayerdialog.h"

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
    ui(new Ui::ResizeLayerDialog)
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
    ui->spinWidth->setValue(size.width());
    ui->spinHeight->setValue(size.height());
}
