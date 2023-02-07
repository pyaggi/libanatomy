#include "anqtermstree.h"
#include <QLayout>
#include <QTreeWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
AnqTermsTree::AnqTermsTree(QWidget *parent)
    :AnqTreeWidget(parent)
{
    auto lang=new QComboBox();
    lang->addItem(tr("Latin"));
    lang->addItem(tr("English"));
    connect(lang,qOverload<int>(&QComboBox::activated),this,&AnqTermsTree::selLang);
    addControlWidget(lang);
    addStandardControls();

    tree()->setHeaderLabels({tr("ID"),
                             tr("Term"),
                             tr("TA-1"),
                             tr("TA-2"),
                             tr("Sex"),
                             tr("Type"),
                            });
}
void AnqTermsTree::selLang(int v)
{
    auto d=v?Qt::UserRole+1:Qt::UserRole;
    std::vector<QTreeWidgetItem *> all;
    collect(all);
    for (auto &i:all)
        i->setText(1,i->data(1,d).toString());
}
void AnqTermsTree::loadTerms(AnaTermConstCursor c,QTreeWidgetItem *p)
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
        i->setText(0,QString::fromStdString(c->id().toString()));
        i->setData(0,Qt::UserRole,c->id().toInt());
        i->setText(1,QString::fromStdString(c->latin()));
        i->setData(1,Qt::UserRole,QString::fromStdString(c->latin()));
        i->setData(1,Qt::UserRole+1,QString::fromStdString(c->english()));
        i->setText(2,QString::fromStdString(c->ta1Code()));
        auto ta2n=c->id().ta2Number();
        if (ta2n)
            i->setText(3,QString::number(ta2n));
        i->setText(4,QString::fromStdString(std::string(Ana::sexCode(c->sex()))));
        i->setText(5,QString::fromStdString(std::string(Ana::termTypeCode(c->type()))));
        loadTerms(c.firstChildCursor(),i);
        c=c.nextSiblingCursor();
    }
}
void AnqTermsTree::loadTerms(const class AnaTermTree &t)
{
    loadTerms(AnaTermConstCursor(&t).firstChildCursor(),nullptr);

}
