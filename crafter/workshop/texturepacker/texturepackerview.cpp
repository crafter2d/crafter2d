#include "texturepackerview.h"

#include "texturepacker.h"

TexturePackerView::TexturePackerView(QWidget *parent) :
    QScrollArea(parent),
    mTexturePacker(new TexturePacker())
{
    setBackgroundRole(QPalette::Background);
    setWidget(mTexturePacker.get());
}

TexturePackerView::~TexturePackerView()
{
}
