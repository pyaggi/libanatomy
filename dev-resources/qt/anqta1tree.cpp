#include "anqta1tree.h"
#include <QLayout>
#include <QTreeWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
AnqTa1Tree::AnqTa1Tree(QWidget *parent)
    :AnqTreeWidget(parent)
{
    auto lang=new QComboBox();
    lang->addItem(tr("Latin"));
    lang->addItem(tr("English"));
    connect(lang,qOverload<int>(&QComboBox::activated),this,&AnqTa1Tree::selLang);
    addControlWidget(lang);
    lang->setHidden(true);

    auto but=new QPushButton(tr("Expand"));
    connect(but,&QPushButton::clicked,this,&AnqTa1Tree::expandVisible);
    addControlWidget(but);
    but=new QPushButton(tr("Collapse"));
    connect(but,&QPushButton::clicked,this,&AnqTa1Tree::collapseVisible);
    addControlWidget(but);

    auto ed=new QLineEdit;
    ed->setPlaceholderText(tr("Search"));
    connect(ed,&QLineEdit::textEdited,this,&AnqTa1Tree::search);
    addControlWidget(ed);

    tree()->setHeaderLabels({tr("ID"),
                             tr("Term"),
                            });
}
void AnqTa1Tree::selLang(int v)
{
    auto d=v?Qt::UserRole+1:Qt::UserRole;
    std::vector<QTreeWidgetItem *> all;
    collect(all);
    for (auto &i:all)
        i->setText(1,i->data(1,d).toString());
}
void AnqTa1Tree::loadTerms(AnaTA1ConstCursor c,QTreeWidgetItem *p)
{
    while (!c.atEnd())
    {
        QTreeWidgetItem *i;
        if (!p)
            i=new QTreeWidgetItem(tree());
        else
        {
            i=new QTreeWidgetItem(p);
            p->addChild(i);
        }
        i->setText(0,QString::fromStdString(c->id()));
        i->setText(1,QString::fromStdString(c->term()));
        i->setData(1,Qt::UserRole,QString::fromStdString(c->term()));
//        i->setData(1,Qt::UserRole+1,QString::fromStdString(c->english()));
        loadTerms(c.firstChildCursor(),i);
        c=c.nextSiblingCursor();
    }
}
void AnqTa1Tree::loadTerms(const AnaTA1 &t)
{
    loadTerms(AnaTA1ConstCursor(&t).firstChildCursor(),nullptr);

}
void AnqTa1Tree::search(const QString &sq)
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
                for (int c=0;c<4;c++)
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
