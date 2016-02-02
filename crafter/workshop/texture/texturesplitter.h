#ifndef TEXTURESPLITTER_H
#define TEXTURESPLITTER_H

#include <QDialog>
#include <QImage>

namespace Ui {
class TextureSplitter;
}

class TextureSplitter : public QDialog
{
    Q_OBJECT

public:
    explicit TextureSplitter(QWidget *parent = 0);
    ~TextureSplitter();

private slots:
    void on_btnOpen_clicked();

    void on_buttonBox_accepted();

private:
    Ui::TextureSplitter *ui;
    QImage mImage;
};

#endif // TEXTURESPLITTER_H
