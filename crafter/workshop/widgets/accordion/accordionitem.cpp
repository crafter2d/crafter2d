#include "accordionitem.h"

#include <QVBoxLayout>

#include "accordionheader.h"

AccordionItem::AccordionItem(QString title, QWidget* body, QWidget *parent):
    QFrame(parent),
    mpHeader(nullptr),
    mpBody(nullptr),
    mCollapsed(true)
{
    //mpHeader = new QLabel(title, this);
    auto header = new AccordionHeader(title);
    connect(header, SIGNAL(expanded(bool)), SLOT(on_headerExpanded(bool)));

    auto playout = new QVBoxLayout();
    playout->setContentsMargins(0, 0, 0, 0);
    playout->addWidget(header);
    setLayout(playout);

    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);

    setBody(body);
}

void AccordionItem::setBody(QWidget* pbody)
{
    if (mpBody) {
        layout()->removeWidget(mpBody);
        delete mpBody;
    }

    mpBody = pbody;
    if (mpBody) {
        layout()->addWidget(mpBody);
        if (mCollapsed) {
            mpBody->setVisible(true);
        }
    }
}

void AccordionItem::on_headerExpanded(bool expanded)
{
    mpBody->setVisible(expanded);
}
