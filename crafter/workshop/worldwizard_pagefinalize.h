#ifndef FINALIZEWORLDPAGE_H
#define FINALIZEWORLDPAGE_H

#include <QWizardPage>

class WorldWizard_PageFinalize : public QWizardPage
{
public:
    WorldWizard_PageFinalize(QWidget *parent = 0);

    virtual int nextId() const override;
};

#endif // FINALIZEWORLDPAGE_H
