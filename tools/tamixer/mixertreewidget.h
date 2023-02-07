#ifndef MIXERTREEWIDGET_H
#define MIXERTREEWIDGET_H

#include <anqtreewidget.h>

class MixerTreeWidget:public AnqTreeWidget
{
    Q_OBJECT
public:
    MixerTreeWidget(QWidget *parent = nullptr);
    static bool isMatched(class QTreeWidgetItem *from);


public slots:
    void filter();
private:
    class MixerFilterWidget *m_filter;
};

#endif // MIXERTREEWIDGET_H
