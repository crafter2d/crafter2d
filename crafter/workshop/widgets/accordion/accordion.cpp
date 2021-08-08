#include "accordion.h"

#include <QVBoxLayout>

#include "accordionheader.h"
#include "accordionitem.h"

Accordion::Accordion(QWidget *parent):
    QWidget(parent)
{
    auto playout = new QVBoxLayout();
    playout->setContentsMargins(0, 0, 0, 0);
    setLayout(playout);
}

void Accordion::addItem(AccordionItem *pitem)
{
    mItems.push_back(pitem);

    layout()->addWidget(pitem);
}

void Accordion::addWidget(const QString &title, QWidget *pwidget)
{
    auto pitem = new AccordionItem(title, pwidget, this);
    addItem(pitem);
}
