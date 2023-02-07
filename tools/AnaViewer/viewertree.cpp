#include "viewertree.h"
#include <QTreeWidget>
#include <QComboBox>
#include "anqjson.h"
ViewerTree::ViewerTree(Mode m, QWidget *parent)
    : AnqTreeWidget{parent}
{
    auto lang=new QComboBox();
    lang->addItem(tr("Latin"));
    lang->addItem(tr("English"));
    connect(lang,qOverload<int>(&QComboBox::activated),this,&ViewerTree::setLang);
    addControlWidget(lang);

    addStandardControls();
    setMode(m);
}
void ViewerTree::setMode(Mode m)
{
    auto lang=static_cast<QComboBox*>(controlWidget(0));
    m_mode=m;
    tree()->setColumnCount(0);
    switch (m)
    {
    case Mode::TA1:
        tree()->setHeaderLabels({tr("Id"),tr("Term"),tr("Synonyms")});
        lang->setHidden(false);
        break;
    case Mode::TA2:
        tree()->setHeaderLabels({tr("Id"),tr("Term"),tr("Synonyms"),tr("Sex"),tr("Type")});
        lang->setHidden(false);
        break;
    case Mode::Terms:
        tree()->setHeaderLabels({tr("Id"),tr("Term"),tr("Synonyms"),tr("TA1"),tr("TA2"),tr("Sex"),tr("Type")});
        lang->setHidden(false);
        break;
    case Mode::Generic:
        tree()->setHeaderLabels({tr("Name")});
        lang->setHidden(true);
        break;
    case Mode::ZAnatomy:
        tree()->setHeaderLabels({tr("Name"),tr("Suffix")});
        lang->setHidden(true);
        break;
    }
}
void ViewerTree::load(const QJsonArray &a,class QTreeWidgetItem *i0)
{
    for (const auto &i:a)
    {
        if (!i.isObject())
            continue;
        auto o=i.toObject();
        QTreeWidgetItem *it;
        if (i0)
            it=new QTreeWidgetItem(i0);
        else
            it=new QTreeWidgetItem(tree());
        if (m_mode==Mode::TA1 ||m_mode==Mode::TA2 || m_mode==Mode::Terms)
        {
            it->setText(0,QString::number(o["id"].toInt()));
            it->setData(0,Qt::UserRole,QVariant::fromValue(o));
            it->setText(1,o["la"].toString());
            it->setData(1,Qt::UserRole,it->text(1));
            it->setData(1,Qt::UserRole+1,o["en"].toString());

            int add=0;
            if (m_mode==Mode::Terms)
            {
                it->setText(3,o["ta1"].toString());
                auto ta2=o["ta2"].toInt();
                if (ta2)
                    it->setText(4,QString::number(ta2));
                add=2;
            }
            if (m_mode==Mode::Terms || m_mode==Mode::TA2)
            {
                it->setText(3+add,o["sex"].toString());
                it->setText(4+add,o["type"].toString());
            }
            const auto &tra=o["trs"].toArray();
            for (const auto &tr:tra)
            {
                auto lang=tr["l"].toString();
                auto term=tr["t"].toString();
                auto syn=tr["s"].toArray();
                if (lang=="la")
                {
                    it->setText(2,AnqJson::toQStringList(syn).join("; "));
                    it->setData(2,Qt::UserRole,it->text(2));

                }
                else if (lang=="en" || lang=="en_US")
                    it->setData(2,Qt::UserRole+1,AnqJson::toQStringList(syn).join("; "));

                if (!lang.isEmpty() && !term.isEmpty())
                {
                    auto idx=lIndex(lang);
                    it->setData(1,Qt::UserRole+idx,term);
                    it->setData(2,Qt::UserRole+idx,AnqJson::toQStringList(syn).join("; "));
                }
            }
        }
        else
        {
            if (m_nameKey.isEmpty())
            {
                static const QStringList keys={"n","name","t","term"};
                for (auto &k:keys)
                    if (o.contains(k))
                    {
                        m_nameKey=k;
                        break;
                    }
                if (m_nameKey.isEmpty())
                {
                    auto okeys=o.keys();
                    for (const auto &oi:okeys)
                        if (oi!="z")
                        {
                            m_nameKey=oi;
                            break;
                        }
                }
                if (m_nameKey.isEmpty())
                    continue;
            }
            QString text=AnqJson::toString(o[m_nameKey]);
            QString otext=text;
            if (m_mode==Mode::ZAnatomy)
            {
                if (text.contains('.'))
                {
                    auto usuf=text.section('.',-1);
                    auto suf=explain(usuf);
                    if (!suf.isEmpty())
                    {
                        text=text.section('.',0,-2);
                        it->setText(1,suf);
                        it->setData(1,Qt::UserRole,usuf);
                    }
                }
            }
            it->setText(0,text);
            it->setData(0,Qt::UserRole+1,otext);
            if (o.contains("id"))
            {
                if (tree()->columnCount()<3)
                {
                    tree()->setColumnCount(3);
                    tree()->headerItem()->setText(2,tr("Id"));
                }
                it->setText(2,AnqJson::toString(o["id"]));
            }
        }
        load(o["z"].toArray(),it);
        o.remove("z");
        it->setData(0,Qt::UserRole,QVariant::fromValue(o));
    }
}
QString ViewerTree::explain(const QString &suffix)
{
    if (suffix=="st")
        return tr("Simple Text");
    if (suffix=="l")
        return tr("Left");
    if (suffix=="r")
        return tr("Right");
    if (suffix=="t")
        return tr("Text/Right Text");
    if (suffix=="s")
        return tr("Left Text");
    if (suffix=="g")
        return tr("Group");
    if (suffix=="j")
        return tr("Joining Line");
    if (suffix=="i")
        return tr("Joining Line (Left)");
    QRegExp test("([oe])([0-9]*)([lr]?)");
    if (test.exactMatch(suffix))
    {
        auto point=test.cap(2).toInt();
        QString ret;
        if (test.cap(1)=="o")
            ret=tr("Muscular Origin");
        else
            ret=tr("Muscular End");
        if (point)
            ret+=QString(" (%1)").arg(point);
        if (test.cap(3)=="l")
            ret+=" "+tr("Left");
        else if (test.cap(3)=="r")
            ret+=" "+tr("Right");
        return ret;
    }
    return {};
}
int ViewerTree::lIndex(const QString &s)
{
    auto combo=static_cast<QComboBox*>(controlWidget(0));
    auto f=combo->findText(s);
    if (f<0)
    {
        f=combo->count();
        combo->addItem(s);
    }
    return f;
}
void ViewerTree::setLang(int lang)
{
    std::vector<QTreeWidgetItem*> all;
    collect(all);
    for (auto &i:all)
    {
        i->setText(1,i->data(1,Qt::UserRole+lang).toString());
        i->setText(2,i->data(2,Qt::UserRole+lang).toString());
    }
}
