#include "accordionheader.h"

#include <QIcon>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMouseEvent>

AccordionHeader::AccordionHeader(const QString& title):
    QWidget(nullptr),
    mExpanded(true)
{
    plabel = new QLabel(title);
    auto pspacer = new QSpacerItem(20,20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    expandLabel = new QLabel();
    QIcon icon(":/images/arrow-up-2.png");
    expandLabel->setPixmap(icon.pixmap(16, 16));
    expandLabel->installEventFilter(this);

    auto playout = new QHBoxLayout();
    playout->addWidget(plabel);
    playout->addItem(pspacer);
    playout->addWidget(expandLabel);
    setLayout(playout);
}

bool AccordionHeader::eventFilter(QObject* watched, QEvent* event) {
    if ( watched != expandLabel )
        return false;
    if ( event->type() != QEvent::MouseButtonPress )
        return false;
    clicked();
    return false;
}

void AccordionHeader::clicked() {
    mExpanded = !mExpanded;
    QIcon icon(mExpanded ? ":/images/arrow-up-2.png" : ":/images/arrow-down-2.png");
    expandLabel->setPixmap(icon.pixmap(QSize(16, 16)));
    emit expanded(mExpanded);
}
