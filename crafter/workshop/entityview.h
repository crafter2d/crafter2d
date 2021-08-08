#ifndef ENTITYVIEW_H
#define ENTITYVIEW_H

#include <QWidget>
#include "widgets/accordion/accordion.h"

class Entity;

namespace Ui {
class EntityView;
}

class EntityView : public QWidget
{
    Q_OBJECT

public:
    explicit EntityView(QWidget *parent = nullptr);
    ~EntityView();

    void setEntity(Entity& entity);

private slots:
    void on_addComponent();

private:
    Ui::EntityView *ui;
    Accordion* mpAccordion;
    Entity* mpEntity;
};

#endif // ENTITYVIEW_H
