#ifndef ACCORDIONITEM_H
#define ACCORDIONITEM_H

#include <QFrame>
#include <QLabel>
#include <QString>


class AccordionItem : public QFrame
{
    Q_OBJECT

public:
    AccordionItem(QString title, QWidget* body, QWidget* parent);

    QWidget* body() { return mpBody; }
    void setBody(QWidget* pbody);

    bool isCollapsed() const { return mCollapsed; }
    void setCollapsed(bool collapsed) { mCollapsed = collapsed; }

private slots:
    void on_headerExpanded(bool expanded);

private:

    QLabel* mpHeader;
    QWidget* mpBody;
    bool mCollapsed;
};

#endif // ACCORDIONITEM_H
