#ifndef ACCORDION_H
#define ACCORDION_H

#include <QWidget>

class AccordionItem;

class Accordion : public QWidget
{
    Q_OBJECT

public:
    Accordion(QWidget* parent = nullptr);

    void addItem(AccordionItem* pitem);
    void addWidget(const QString& title, QWidget* pwidget);
private:

    std::vector<AccordionItem*> mItems;
};

#endif // ACCORDION_H
