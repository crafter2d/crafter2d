#ifndef TEXTUREPACKER_H
#define TEXTUREPACKER_H

#include <QVector>
#include <QWidget>

#include "sprite.h"

namespace Ui {
class TexturePacker;
}

class SpriteSheet;

class TexturePacker : public QWidget
{
    Q_OBJECT

public:
    explicit TexturePacker(QWidget *parent = 0);
    ~TexturePacker();

    void pack();

protected:
  // override
    virtual void paintEvent(QPaintEvent* /* event */) override;

private:
    using SpriteArray = QVector<Sprite>;

    void recurseDirectory(SpriteArray& sprites, const QString& path);
    SpriteSheet createSpriteSheet(SpriteArray& sprites);

  // data
    Ui::TexturePacker *ui;
};

#endif // TEXTUREPACKER_H
