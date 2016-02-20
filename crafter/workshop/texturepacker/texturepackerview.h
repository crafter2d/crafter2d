#ifndef TEXTUREPACKERVIEW_H
#define TEXTUREPACKERVIEW_H

#include <memory>
#include <QScrollArea>
#include "texturepacker.h"

class TexturePackerView : public QScrollArea
{
    Q_OBJECT

public:
    explicit TexturePackerView(QWidget *parent = 0);
    ~TexturePackerView();

private:

  // data
    std::unique_ptr<TexturePacker> mTexturePacker;
};

#endif // TEXTUREPACKERVIEW_H
