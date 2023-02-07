#include "anqtreewidget.h"
#include <QLayout>
#include <QTreeWidget>
#include <QLineEdit>
#include <QPushButton>

AnqTreeWidget::AnqTreeWidget(QWidget *parent)
    : QWidget{parent}
{
    auto lay=new QVBoxLayout(this);
    auto w=new QWidget;
    auto wl=new QHBoxLayout(w);
    wl->setMargin(0);
    lay->addWidget(w);
    m_tree=new QTreeWidget;
    lay->addWidget(m_tree);

    m_tree->setSelectionMode(QTreeWidget::SelectionMode::SingleSelection);

}
void AnqTreeWidget::addStandardControls()
{
    auto but=new QPushButton(tr("Expand"));
    connect(but,&QPushButton::clicked,this,&AnqTreeWidget::expandVisible);
    addControlWidget(but);
    but=new QPushButton(tr("Collapse"));
    connect(but,&QPushButton::clicked,this,&AnqTreeWidget::collapseVisible);
    addControlWidget(but);

    auto ed=new QLineEdit;
    ed->setPlaceholderText(tr("Search"));
    connect(ed,&QLineEdit::textEdited,this,&AnqTreeWidget::search);
    addControlWidget(ed);
}
void AnqTreeWidget::addControlWidget(QWidget *w)
{
    auto lay=static_cast<QHBoxLayout*>(layout())->itemAt(0)->widget()->layout();
    lay->addWidget(w);
}
QWidget *AnqTreeWidget::controlWidget(int c)
{
    auto lay=static_cast<QHBoxLayout*>(layout())->itemAt(0)->widget()->layout();
    return lay->itemAt(c)->widget();
}
void AnqTreeWidget::collect(class QTreeWidgetItem *from,std::vector<class QTreeWidgetItem *>&v,bool includeFrom)
{
    if (!from)
        return;
    auto max=from->childCount();
    for (int c=0;c<max;c++)
        collect(from->child(c),v);
    if (includeFrom)
        v.push_back(from);
}
void AnqTreeWidget::collectChildren(class QTreeWidgetItem *from,std::vector<class QTreeWidgetItem *>&v,bool includeFrom)
{
    if (!from)
        return;
    if (includeFrom)
        v.push_back(from);
    auto max=from->childCount();
    for (int c=0;c<max;c++)
        v.push_back(from->child(c));
}
void AnqTreeWidget::collect(std::vector<class QTreeWidgetItem *>&v)
{
    auto max=m_tree->topLevelItemCount();
    for (int c=0;c<max;c++)
        collect(m_tree->topLevelItem(c),v);
}
void AnqTreeWidget::collectLeaf(class QTreeWidgetItem *from,std::vector<class QTreeWidgetItem *>&v,bool includeFrom)
{
    if (!from)
        return;
    auto max=from->childCount();
    if (!max && includeFrom)
        v.push_back(from);
    for (int c=0;c<max;c++)
        collectLeaf(from->child(c),v);
}
void AnqTreeWidget::collectLeaf(std::vector<class QTreeWidgetItem *>&v)
{
    auto max=m_tree->topLevelItemCount();
    for (int c=0;c<max;c++)
        collectLeaf(m_tree->topLevelItem(c),v);
}
void AnqTreeWidget::collectLeaf(int check,class QTreeWidgetItem *from,std::vector<class QTreeWidgetItem *>&v,bool includeFrom)
{
    if (!from)
        return;
    auto max=from->childCount();
    if (includeFrom)
    {
        if (!max || from->data(0,Qt::UserRole+1).toInt()==check)
            v.push_back(from);
    }
    for (int c=0;c<max;c++)
        collectLeaf(check,from->child(c),v);
}
void AnqTreeWidget::collectLeaf(int check,std::vector<class QTreeWidgetItem *>&v)
{
    auto max=m_tree->topLevelItemCount();
    for (int c=0;c<max;c++)
        collectLeaf(check,m_tree->topLevelItem(c),v);
}
void AnqTreeWidget::expand(class QTreeWidgetItem*i)
{
    do
    {
        i->setExpanded(true);
        i=i->parent();
    }while (i);
}
void AnqTreeWidget::expandVisible()
{
    std::vector<QTreeWidgetItem*> v;
    collect(v);
    for (auto &i:v)
    {
        if (!i->isHidden())
            i->setExpanded(true);
    }
}
void AnqTreeWidget::collapseVisible()
{
    std::vector<QTreeWidgetItem*> v;
    collect(v);
    for (auto &i:v)
    {
        if (!i->isHidden())
            i->setExpanded(false);
    }
}

void AnqTreeWidget::showSelected(class QTreeWidgetItem *i)
{
    if (!i)
        return;
    expand(i);
    m_tree->scrollToItem(i);
    m_tree->clearSelection();
    i->setSelected(true);
}
void AnqTreeWidget::clearHighlight(int col)
{
    auto max=m_tree->topLevelItemCount();
    for (int c=0;c<max;c++)
        clearHighlight(m_tree->topLevelItem(c),col);
}
void AnqTreeWidget::clearHighlight(class QTreeWidgetItem*i,int col)
{
    if (!i)
        return;
    i->setBackground(col,{});
    auto max=i->childCount();
    for (int c=0;c<max;c++)
        clearHighlight(i->child(c),col);
}
void AnqTreeWidget::highlight(class QTreeWidgetItem*i,int col)
{
    static const QBrush br(QColor(0xa0,0xa0,0xa0));
    i->setBackground(col,br);
}
void AnqTreeWidget::clear()
{
    m_tree->clear();
}
void AnqTreeWidget::search(const QString &sq)
{
    std::vector<QTreeWidgetItem *> all;
    collect(all);

    if (sq.isEmpty())
    {
        for (auto &i:all)
            i->setHidden(false);
        return;
    }

    std::map<QTreeWidgetItem *,bool> showmap;
    auto cols=m_tree->columnCount();
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
                for (int c=0;c<cols;c++)
                    if (o->text(c).contains(sq,Qt::CaseInsensitive))
                    {
                        show=true;
                        break;
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
void AnqTreeWidget::generateStats(int colin,int colout)
{
    if (colout>=m_tree->columnCount())
    {
        m_tree->setColumnCount(colout+1);
        m_tree->headerItem()->setText(colout,tr("Stats"));
    }
    int max=m_tree->topLevelItemCount();
    for (int c=0;c<max;c++)
        genStats(m_tree->topLevelItem(c),colin,colout);
}
#include <qdebug.h>
AnqTreeWidget::Stats AnqTreeWidget::genStats(class QTreeWidgetItem*i,int colin,int colout)
{
    Stats s;
    auto t=i->text(colin);
    s.t=1;
    s.w=t.split(' ').count();
    s.l=t.size();
    auto max=i->childCount();
    for (int c=0;c<max;c++)
    {
        auto s1=genStats(i->child(c),colin,colout);
        s.t+=s1.t;
        s.w+=s1.w;
        s.l+=s1.l;
    }
    i->setText(colout,tr("Terms: %1 Words: %2 Characters: %3").arg(s.t).arg(s.w).arg(s.l));
    return s;
}
