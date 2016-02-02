#include "texturesplitter.h"
#include "ui_texturesplitter.h"

#include <QFileDialog>

TextureSplitter::TextureSplitter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextureSplitter),
    mImage()
{
    ui->setupUi(this);
}

TextureSplitter::~TextureSplitter()
{
    delete ui;
}

void TextureSplitter::on_btnOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home", tr("Images (*.png *.jpg *.bmp)"));
    if ( !fileName.isNull() )
    {
        ui->lineEdit->setText(fileName);

        if ( mImage.load(fileName) )
        {
            // wierd, should have succeeded
            ui->lblInfo->setText("Can not load the image!");
        }

        QString info = QString("Image dimensions are %1 x %2.").arg(mImage.width()).arg(mImage.height());
        ui->lblInfo->setText(info);
    }
}

void TextureSplitter::on_buttonBox_accepted()
{
    int tileWidth = ui->spinWidth->text().toInt();
    int tileHeight = ui->spinHeight->text().toInt();

    std::div_t w = std::div(mImage.width(), tileWidth);
    std::div_t h = std::div(mImage.height(), tileHeight);

    if ( w.rem != 0 || h.rem != 0 )
    {
        // can not divide by it
        return;
    }

    QFileInfo info(ui->lineEdit->text());
    QString filename = info.absolutePath() + QDir::separator() + ui->edtExport->text();

    const QString idlocator("{n}");
    int tileId = 0;

    for ( int y = 0; y < mImage.height(); y += tileHeight )
    {
        for ( int x = 0; x < mImage.width(); x += tileWidth )
        {
            QImage tile = mImage.copy(x, y, tileWidth, tileHeight);
            QString path = filename;
            path.replace(idlocator, QString::number(tileId));
            tile.save(path, "PNG");
            tileId++;
        }
    }
}
