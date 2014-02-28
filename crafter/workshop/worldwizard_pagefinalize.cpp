#include "worldwizard_pagefinalize.h"

#include <QLabel>
#include <QVBoxLayout>

WorldWizard_PageFinalize::WorldWizard_PageFinalize(QWidget* parent):
    QWizardPage(parent)
{
    setTitle("Finally");

    QLabel *label = new QLabel("This wizard will help you register your copy ");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

int WorldWizard_PageFinalize::nextId() const
{
    return -1;
}
