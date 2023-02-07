#include "anqta2tree.h"
#include "anqtools.h"
#include <QLayout>
#include <QTreeWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
AnqTa2Tree::AnqTa2Tree(QWidget *parent)
    :AnqTreeWidget(parent)
{
    auto lang=new QComboBox();
    lang->addItem(tr("Latin"));
    lang->addItem(tr("Eng US"));
    lang->addItem(tr("Eng GB"));
    connect(lang,qOverload<int>(&QComboBox::activated),this,&AnqTa2Tree::selLang);
    addControlWidget(lang);

    auto but=new QPushButton(tr("Expand"));
    connect(but,&QPushButton::clicked,this,&AnqTa2Tree::expandVisible);
    addControlWidget(but);
    but=new QPushButton(tr("Collapse"));
    connect(but,&QPushButton::clicked,this,&AnqTa2Tree::collapseVisible);
    addControlWidget(but);

    auto ed=new QLineEdit;
    ed->setPlaceholderText(tr("Search"));
    connect(ed,&QLineEdit::textEdited,this,&AnqTa2Tree::search);
    addControlWidget(ed);

    tree()->setHeaderLabels({tr("ID"),
                             tr("Term"),
                             tr("Synonyms"),
                             tr("Sex"),
                             tr("Type"),
                            });
}
void AnqTa2Tree::selLang(int v)
{

    auto d0=v?Qt::UserRole+1:Qt::UserRole;
    auto d=v?Qt::UserRole+1:Qt::UserRole;
    if (v==2)
        d0=Qt::UserRole+2;

    std::vector<QTreeWidgetItem *> all;
    collect(all);
    for (auto &i:all)
    {
        i->setText(1,i->data(1,d0).toString());
        i->setText(2,i->data(2,d).toString());
    }
}
void AnqTa2Tree::loadTerms(AnaTA2ConstCursor c, QTreeWidgetItem *p)
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
        i->setText(0,QString::number(c->id()));

        i->setText(1,QString::fromStdString(c->latin()));
        i->setData(1,Qt::UserRole,QString::fromStdString(c->latin()));
        i->setData(1,Qt::UserRole+1,QString::fromStdString(c->us()));
        i->setData(1,Qt::UserRole+2,QString::fromStdString(c->gb()));

        i->setText(2,Ana::toStringList(c->latinSyn()).join(";"));
        i->setData(2,Qt::UserRole,Ana::toStringList(c->latinSyn()).join(";"));
        i->setData(2,Qt::UserRole+1,Ana::toStringList(c->syn()).join(";"));

        i->setText(3,QString::fromStdString(std::string(Ana::sexCode(c->sex()))));
        i->setText(4,QString::fromStdString(std::string(Ana::recordTypeCode(c->type()))));
        loadTerms(c.firstChildCursor(),i);
        c=c.nextSiblingCursor();
    }
}
void AnqTa2Tree::loadTerms(const AnaTA2 &t)
{
    loadTerms(AnaTA2ConstCursor(&t).firstChildCursor(),nullptr);

}
void AnqTa2Tree::search(const QString &sq)
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
