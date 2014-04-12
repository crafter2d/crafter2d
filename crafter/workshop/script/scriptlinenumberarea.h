#ifndef SCRIPTLINENUMBERAREA_H
#define SCRIPTLINENUMBERAREA_H

#include <QWidget>

class ScriptView;

class ScriptLineNumberArea : public QWidget
{
public:
    ScriptLineNumberArea(ScriptView& view);

  // overrides
    virtual QSize sizeHint() const override;

protected:
  // overrides
    virtual void paintEvent(QPaintEvent *event) override;

private:
  // data
    ScriptView& mView;
};

#endif // SCRIPTLINENUMBERAREA_H
