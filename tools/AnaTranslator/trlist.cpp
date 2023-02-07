#include "trlist.h"
#include <QLayout>
#include <QListWidget>

TrList::TrList(QWidget *parent)
    : QWidget{parent}
{
    auto lay=new QGridLayout(this);
    m_list=new QListWidget;
    lay->addWidget(m_list,1,1);

    connect(m_list,&QListWidget::itemActivated,this,&TrList::act);

}
void TrList::act(class QListWidgetItem*i)
{
    auto idx=i->data(Qt::UserRole).toInt();;
    if (idx==m_current)
        return;
    m_current=idx;
    activated(m_current);
}
QIcon TrList::icon(TrStatus st)
{
    switch (st)
    {
    case TrStatus::NewTr:
        return QIcon(":/icons/tr_new.svg");
    case TrStatus::AutoTr:
        return QIcon(":/icons/tr_auto.svg");
    case TrStatus::WorkInProgress:
        return QIcon(":/icons/tr_work.svg");
    case TrStatus::Finished:
        return QIcon(":/icons/tr_fin.svg");
    case TrStatus::Obsolete:
        return QIcon(":/icons/tr_obs.svg");
    }
    return {};
}
void TrList::addWord(TrStatus st,const QString &w)
{
    auto idx=m_list->count();
    auto i=new QListWidgetItem(m_list);
    i->setText(w);
    i->setData(Qt::UserRole,idx);
    setStatus(idx,st);
}
void TrList::setStatus(int idx,TrStatus st)
{
    auto i=m_list->item(idx);
    i->setIcon(icon(st));
    i->setData(Qt::UserRole+1,static_cast<int>(st));
    switch (st)
    {
    case TrStatus::NewTr:
        i->setToolTip(tr("New translation"));
        break;
    case TrStatus::AutoTr:
        i->setToolTip(tr("Auto translated"));
        break;
    case TrStatus::WorkInProgress:
        i->setToolTip(tr("Work in progress"));
        break;
    case TrStatus::Finished:
        i->setToolTip(tr("Translation finished"));
        break;
    case TrStatus::Obsolete:
        i->setToolTip(tr("Obsolete translation"));
        break;
    }
}
TrStatus TrList::status(int idx) const
{
    auto i=m_list->item(idx);
    return static_cast<TrStatus>(i->data(Qt::UserRole+1).toInt());
}
void TrList::clear()
{
    m_current=-1;
    m_list->clear();
}
void TrList::next(bool nextU)
{
    if (m_current<0)
        return;
    int tryed=0;
    do
    {
        ++tryed;
        m_current++;
        if (m_current>=m_list->count())
        {
            if (m_list->count())
                m_current=0;
            else
                m_current=-1;
        }
        if (m_current>=0)
        {
            if (!nextU || status(m_current)!=TrStatus::Finished || tryed==m_list->count())
            {
                m_list->setCurrentItem(m_list->item(m_current));
                activated(m_current);
                return;
            }
        }
    }while (m_current>0);
}
