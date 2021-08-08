#ifndef ACCORDIONHEADER_H
#define ACCORDIONHEADER_H

#include <QLabel>
#include <QWidget>

class AccordionHeader : public QWidget
{
    Q_OBJECT
public:
    AccordionHeader(const QString &title);

signals:
    void expanded(bool isexpanded);

private:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void clicked();

    QLabel* plabel;
    QLabel* expandLabel;
    bool mExpanded;
};

#endif // ACCORDIONHEADER_H
