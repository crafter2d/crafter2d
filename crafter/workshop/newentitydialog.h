#ifndef NEWENTITYDIALOG_H
#define NEWENTITYDIALOG_H

#include <QDialog>

namespace Ui {
class NewEntityDialog;
}

class Entity;

class NewEntityDialog : public QDialog
{
    Q_OBJECT

public:
    static void edit(Entity& entity);

    explicit NewEntityDialog(QWidget *parent = 0);
    ~NewEntityDialog();

private:

    void setEntity(Entity& entity);

    Ui::NewEntityDialog *ui;
};

#endif // NEWENTITYDIALOG_H
