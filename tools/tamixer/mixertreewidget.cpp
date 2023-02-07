#include "mixertreewidget.h"
#include <QLayout>
#include "mixerfilterwidget.h"
#include <QTreeWidget>
MixerTreeWidget::MixerTreeWidget(QWidget *parent)
    :AnqTreeWidget{parent}
{
    m_filter=new MixerFilterWidget;
    addControlWidget(m_filter);

    connect(m_filter,&MixerFilterWidget::filterUpdated,this,&MixerTreeWidget::filter);
    connect(m_filter,&MixerFilterWidget::expand,this,&MixerTreeWidget::expandVisible);
    connect(m_filter,&MixerFilterWidget::collapse,this,&MixerTreeWidget::collapseVisible);
}
void MixerTreeWidget::filter()
{
    bool ucheck=m_filter->unmatchedFilter();
    auto idcheck=m_filter->idFilter();
    auto tcheck=m_filter->contentFilter();

    std::vector<QTreeWidgetItem *> all;
    collect(all);

    if (!ucheck && idcheck.isEmpty() && tcheck.isEmpty())
    {
        for (auto &i:all)
            i->setHidden(false);
        return;
    }

    std::map<QTreeWidgetItem *,bool> showmap;

    for (auto &i:all)
    {
        if (i->childCount())
            continue;
        auto o=i;
        bool show=false;
        do
        {
            if (!show)
            {
                if ((ucheck && !isMatched(o)))
                    show=true;
                else if (!idcheck.isEmpty() && o->text(0).contains(idcheck,Qt::CaseInsensitive))
                    show=true;
                else if (!tcheck.isEmpty())
                {
                    if (o->text(1).contains(tcheck,Qt::CaseInsensitive))
                        show=true;
                    else if (tree()->columnCount()>4 && o->text(4).contains(tcheck,Qt::CaseInsensitive))
                        show=true;
                }
            }
            if (show)
                showmap[o]=show;
            o=o->parent();
        }while (o);
    }
    for (auto &i:all)
        i->setHidden(!showmap[i]);

}
bool MixerTreeWidget::isMatched(class QTreeWidgetItem *from)
{
    return !from->text(2).isEmpty();
}
