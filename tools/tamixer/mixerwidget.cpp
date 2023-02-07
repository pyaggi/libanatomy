#include "mixerwidget.h"
#include <QSplitter>
#include <QLayout>
#include <QTreeWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QLabel>
#include <mixertreewidget.h>
MixerWidget::MixerWidget(QWidget *parent)
    : QWidget{parent}
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    auto lay=new QVBoxLayout(this);

    auto w=new QWidget;
    w->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    auto wl=new QHBoxLayout(w);
    lay->addWidget(w);

    m_langCmb=new QComboBox;
    m_langCmb->addItem(tr("Latin"));
    m_langCmb->addItem(tr("Eng GB"));
    m_langCmb->addItem(tr("Eng US"));
    wl->addWidget(m_langCmb);
    connect(m_langCmb,qOverload<int>(&QComboBox::activated),this,qOverload<int>(&MixerWidget::langCmb));

    m_save=new QPushButton(tr("Save"));
    wl->addWidget(m_save);
    m_save->setEnabled(false);
    connect(m_save,&QPushButton::clicked,this,&MixerWidget::save);
    m_reset=new QPushButton(tr("Reset"));
    wl->addWidget(m_reset);
    m_reset->setEnabled(false);
    connect(m_reset,&QPushButton::clicked,this,&MixerWidget::reset);

    auto but=new QPushButton(tr("Match"));
    wl->addWidget(but);
    connect(but,&QPushButton::clicked,this,&MixerWidget::matchSelected);
    but=new QPushButton(tr("Unmatch"));
    wl->addWidget(but);
    connect(but,&QPushButton::clicked,this,&MixerWidget::unmatchSelected);

    auto autocmb=new QComboBox;
    autocmb->addItem(tr("Auto"));
    autocmb->addItem(tr("One level"));
    autocmb->addItem(tr("Leaf/Terms (selected)"));
    autocmb->addItem(tr("Recurive (selected)"));
    autocmb->addItem(tr("Leaf/Terms (all)"));
    autocmb->addItem(tr("Recursive (all)"));
    autocmb->addItem(tr("Find Left"));
    autocmb->addItem(tr("Find Right"));
    autocmb->addItem(tr("Recursive (parent-filter,selected)"));
    autocmb->addItem(tr("Find Left (parent-filter)"));
    autocmb->addItem(tr("Find Right (parent-filter)"));
    autocmb->addItem(tr("One level on matched"));
    autocmb->addItem(tr("Leaf/Term on matched"));
    autocmb->addItem(tr("Recursive on matched"));
    autocmb->addItem(tr("One level on matched (parent-filter)"));
    autocmb->addItem(tr("Recursive on matched (parent-filter)"));
    wl->addWidget(autocmb);
    connect(autocmb,qOverload<int>(&QComboBox::activated),this,&MixerWidget::autoMatch);


    auto sp=new QSplitter;
    sp->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    lay->addWidget(sp);
    m_ta1=new MixerTreeWidget;
    m_ta2=new MixerTreeWidget;
    sp->addWidget(m_ta1);
    sp->addWidget(m_ta2);

    m_ta1->tree()->setHeaderLabels({tr("Id"),tr("Term"),tr("Match")});
    m_ta2->tree()->setHeaderLabels({tr("Id"),tr("Term"),tr("Match"),tr("Synonymous"),tr("Type")});

    connect(m_ta1->tree(),&QTreeWidget::itemActivated,this,&MixerWidget::ta1Activated);
    connect(m_ta2->tree(),&QTreeWidget::itemActivated,this,&MixerWidget::ta2Activated);

    m_report=new QLabel;
    lay->addWidget(m_report);

}
void MixerWidget::ta1Activated(class QTreeWidgetItem *item, int column)
{
    auto m=item->data(2,Qt::UserRole).toInt();
    if (m)
        m_ta2->showSelected(findTa2(m));
}
void MixerWidget::ta2Activated(class QTreeWidgetItem *item, int column)
{
    auto m=item->text(2);
    if (!m.isEmpty())
        m_ta1->showSelected(findTa1(m));
}
void MixerWidget::langCmb(int l)
{
    if (static_cast<int>(m_l)==l)
        return;
    setLanguage(static_cast<Lang>(l));
}
void MixerWidget::loadTa1(class AnaTA1 &a)
{
    m_ta1->clear();
    loadTa1(&a,nullptr);
    clearMatches();
    updateMatches();
    m_ta1->filter();
}
void MixerWidget::loadTa2(class AnaTA2 &a)
{
    m_ta2->clear();
    loadTa2(&a,nullptr);
    clearMatches();
    updateMatches();
    m_ta2->filter();
}
void MixerWidget::loadTa1(AnaTA1Cursor a,class QTreeWidgetItem *i)
{
    while (!a.atEnd())
    {
        QTreeWidgetItem *i1=i;
        if (!a->id().empty())
        {
            if (!i)
            {
                i1=new QTreeWidgetItem(m_ta1->tree());
                m_ta1->tree()->addTopLevelItem(i1);
            }
            else
                i1=new QTreeWidgetItem(i);

            i1->setText(0,QString::fromStdString(a->id()));
            i1->setText(1,QString::fromStdString(a->term()));
        }
        loadTa1(a.firstChildCursor(),i1);
        a=a.nextSiblingCursor();
    }
}
void MixerWidget::setLanguage(Lang l)
{
    if (m_l==l)
        return;
    m_l=l;
    m_langCmb->setCurrentIndex(static_cast<int>(l));
    auto tree=m_ta2->tree();
    auto max=tree->topLevelItemCount();
    for (int c=0;c<max;c++)
        updateLang(tree->topLevelItem(c));
}
void MixerWidget::setModified(bool t)
{
    t &=!m_mixFile.isEmpty();
    m_save->setEnabled(t);
    m_reset->setEnabled(t);
    updateInfo();
}
void MixerWidget::updateLang(class QTreeWidgetItem *i)
{
    switch (m_l)
    {
    case Lat:
        i->setText(1,i->data(1,Qt::UserRole).toString());
        i->setText(3,i->data(3,Qt::UserRole).toStringList().join("; "));
        break;
    case Uk:
        i->setText(1,i->data(1,Qt::UserRole+1).toString());
        i->setText(3,i->data(3,Qt::UserRole+1).toStringList().join("; "));
        break;
    case Us:
        i->setText(1,i->data(1,Qt::UserRole+2).toString());
        i->setText(3,i->data(3,Qt::UserRole+1).toStringList().join("; "));
        break;
    }
    auto max=i->childCount();
    for (int c=0;c<max;c++)
        updateLang(i->child(c));
}
#include "anqtools.h"
void MixerWidget::loadTa2(AnaTA2Cursor a,class QTreeWidgetItem *i)
{
    while (!a.atEnd())
    {
        QTreeWidgetItem *i1=i;
        if (a->id())
        {
            if (!i)
            {
                i1=new QTreeWidgetItem(m_ta2->tree());
                m_ta1->tree()->addTopLevelItem(i1);
            }
            else
                i1=new QTreeWidgetItem(i);

            i1->setText(0,QString::number(a->id()));
            i1->setData(0,Qt::UserRole,a->id());
            i1->setData(0,Qt::UserRole+1,static_cast<int>(a->type()));
            i1->setData(1,Qt::UserRole,QString::fromStdString(a->latin()));
            i1->setData(1,Qt::UserRole+1,QString::fromStdString(a->gb()));
            i1->setData(1,Qt::UserRole+2,QString::fromStdString(a->us()));
            i1->setData(3,Qt::UserRole,Ana::toStringList(a->latinSyn()));
            i1->setData(3,Qt::UserRole+1,Ana::toStringList(a->syn()));
            i1->setText(4,a->type()==Ana::RecordType::Heading?"H":"T");
            updateLang(i1);
        }
        loadTa2(a.firstChildCursor(),i1);
        a=a.nextSiblingCursor();
    }
}
//#include <fileswidget.h>
void MixerWidget::setMixFile(const QString &s)
{
//    auto fw=qobject_cast<FilesWidget *>(sender());
    m_mixFile=s;
    updateMatches();
}
void MixerWidget::clearMatches()
{
    if (!m_match.size())
        return;
    auto tree1=m_ta1->tree();
    auto tree2=m_ta2->tree();

    auto max=tree1->topLevelItemCount();
    for (int c=0;c<max;c++)
        clearMatches(tree1->topLevelItem(c));
    max=tree2->topLevelItemCount();
    for (int c=0;c<max;c++)
        clearMatches(tree2->topLevelItem(c));
    m_match.clear();
    updateInfo();
}
void MixerWidget::clearMatches(QTreeWidgetItem *i)
{
    i->setText(2,{});
    i->setData(2,Qt::UserRole,{});
    auto max=i->childCount();
    for (int c=0;c<max;c++)
        clearMatches(i->child(c));
}
void MixerWidget::updateMatches()
{
    QFile f(m_mixFile);
    if (!f.open(QFile::ReadOnly))
        return ;
    QJsonParseError err;
    auto doc=QJsonDocument::fromJson(f.readAll(),&err);
    if (err.error!=QJsonParseError::NoError)
        return ; //{false,err.errorString()};
    clearMatches();
    auto a=doc.array();
    for (auto i:a)
    {
        auto o=i.toObject();
        if (o.isEmpty())
            continue;
        auto ta1=o["ta1"].toString();
        auto ta2=o["ta2"].toInt();
        match(ta1,ta2,false);
    }
    setModified(false);
}
void MixerWidget::reset()
{
    updateMatches();
    setModified(false);
}
void MixerWidget::save()
{
    QJsonArray a;
    for (auto &m:m_match)
    {
        QJsonObject o;
        o["ta1"]=m.first;
        o["ta2"]=m.second;
        a.push_back(o);
    }
    QJsonDocument doc(a);
    QFile f(m_mixFile);
    if (!f.open(QFile::WriteOnly|QFile::Truncate))
    {
        QMessageBox::critical(this,windowTitle(),f.errorString());
        return ;
    }
    f.write(doc.toJson());
    setModified(false);
    updateInfo();
}
void MixerWidget::unmatchTa1(const QString &id)
{
    auto f=m_match.find(id);
    if (f==m_match.end())
        return ;
    auto i=findTa1(id);
    if (i)
    {
        i->setText(2,{});
        i->setData(2,Qt::UserRole,{});
    }
    i=findTa2(f->second);
    if (i)
        i->setText(2,{});
    m_match.erase(f);
    updateInfo();
}
void MixerWidget::updateInfo()
{
    m_report->setText(tr("Matched: %1").arg(m_match.size()));
}
void MixerWidget::unmatchTa2(int id)
{
    for (auto it=m_match.begin();it!=m_match.end();++it)
    {
        if (it->second==id)
        {
            auto i=findTa1(it->first);
            if (i)
            {
                i->setText(2,{});
                i->setData(2,Qt::UserRole,{});
            }
            i=findTa2(id);
            if (i)
                i->setText(2,{});
            m_match.erase(it);
            break;
        }
    }
    updateInfo();
}
void MixerWidget::unmatchSelected()
{
    auto i1=m_ta1->tree()->currentItem();
    auto i2=m_ta2->tree()->currentItem();
    if (!i1 && !i2)
        return;
    if (QMessageBox::warning(this,windowTitle(),tr("Really unmatch ?"),QMessageBox::StandardButton::Yes,QMessageBox::StandardButton::No)!=QMessageBox::StandardButton::Yes)
        return;
    QString sel1;
    int sel2=0;
    if (i1)
        sel1=i1->text(0);
    if (i2)
        sel2=i2->data(0,Qt::UserRole).toInt();
    if (i1 && i2)
    {
        auto match2=i1->data(2,Qt::UserRole).toInt();
        auto match1=i2->text(2);
        if (match1!=sel1 || match2!=sel2)
        {
            if (QMessageBox::warning(this,windowTitle(),tr("Different pair selected, unmatch both ?"),QMessageBox::StandardButton::Yes,QMessageBox::StandardButton::No)!=QMessageBox::StandardButton::Yes)
                return;
        }
    }
    unmatchTa1(sel1);
    unmatchTa2(sel2);
    setModified();
}
class QTreeWidgetItem *MixerWidget::findTa1(const QString &id)
{
    auto il1=m_ta1->tree()->findItems(id,Qt::MatchFlag::MatchExactly|Qt::MatchRecursive,0);
    if (il1.size())
        return il1.first();
    return nullptr;
}

class QTreeWidgetItem *MixerWidget::findTa2(int id)
{
    auto il2=m_ta2->tree()->findItems(QString::number(id),Qt::MatchFlag::MatchExactly|Qt::MatchRecursive,0);
    if (il2.size())
        return il2.first();
    return nullptr;
}

void MixerWidget::match(const QString &ta1,int ta2,bool hl)
{
    matchItems(findTa1(ta1),findTa2(ta2),false,hl);
}
void MixerWidget::highlightTa1(const QString &ta1)
{
    auto i=findTa1(ta1);
    if (i)
        m_ta1->highlight(i);
}
bool MixerWidget::matchItems(class QTreeWidgetItem *i1,class QTreeWidgetItem *i2,bool warn,bool hl)
{
    if (!i1 || !i2)
        return false;
    auto cur1=i2->text(2);
    auto cur2=i1->data(2,Qt::UserRole).toInt();
    if (warn && (cur1.size() || cur2))
    {
        if (QMessageBox::warning(this,windowTitle(),tr("Unmatch current pairs ?"),QMessageBox::StandardButton::Yes,QMessageBox::StandardButton::No)!=QMessageBox::StandardButton::Yes)
            return false;
    }
    unmatchTa1(cur1);
    unmatchTa2(cur2);
    auto ta1id=i1->text(0);
    auto ta2id=i2->data(0,Qt::UserRole).toInt();
    m_match[ta1id]=ta2id;
    i1->setText(2,QString::number(ta2id));
    i1->setData(2,Qt::UserRole,ta2id);
    i2->setText(2,ta1id);
    if (hl)
    {
        m_ta1->highlight(i1);
        m_ta2->highlight(i2);
        m_ta1->expand(i1);
        m_ta1->expand(i2);
    }

    return true;
}
void MixerWidget::matchSelected()
{
    if (matchItems(m_ta1->tree()->currentItem(),m_ta2->tree()->currentItem(),true))
        setModified();
}
void MixerWidget::clearHighlight()
{
    m_ta1->clearHighlight();
    m_ta2->clearHighlight();
}
void MixerWidget::autoMatch(int i)
{
    clearHighlight();
    switch (i)
    {
    case 0:
        return;
    case 1:
        autoOneLevel();
        break;
    case 2:
        autoLeafLevel();
        break;
    case 3:
        autoRecLevel();
        break;
    case 4:
        autoLeaf();
        break;
    case 5:
        autoAll();
        break;
    case 6:
        autoFindLeft();
        break;
    case 7:
        autoFindRight();
        break;
    case 8:
        autoParentFiltered();
        break;
    case 9:
        autoPfFindLeft();
        break;
    case 10:
        autoPfFindRight();
        break;
    case 11:
        autoOneLevelOnMatched();
        break;
    case 12:
        autoLeafOnMatched();
        break;
    case 13:
        autoRecOnMatched();
        break;
    case 14:
        autoPfOnMatched();
        break;
    case 15:
        autoPfRecOnMatched();
        break;
    }
    qobject_cast<QComboBox*>(sender())->setCurrentIndex(0);
}

void MixerWidget::autoRecLevel()
{
    std::vector<QTreeWidgetItem *> list1,list2;
    MixerTreeWidget::collect(m_ta1->tree()->currentItem(),list1,false);
    MixerTreeWidget::collect(m_ta2->tree()->currentItem(),list2,false);
    autoMatchItems(list1,list2);
}
void MixerWidget::autoAll()
{
    std::vector<QTreeWidgetItem *> list1,list2;
    m_ta1->collect(list1);
    m_ta2->collect(list2);
    autoMatchItems(list1,list2);
}
void MixerWidget::autoOneLevel()
{
    std::vector<QTreeWidgetItem *> list1,list2;
    MixerTreeWidget::collectChildren(m_ta1->tree()->currentItem(),list1,false);
    MixerTreeWidget::collectChildren(m_ta2->tree()->currentItem(),list2,false);
    autoMatchItems(list1,list2);
}
void MixerWidget::autoLeafOnMatched()
{
    std::vector<QTreeWidgetItem *> list0,list1,list2;
    m_ta1->collect(list0);
    for (auto &i1:list0)
    {
        auto match2=i1->data(2,Qt::UserRole).toInt();
        if (!match2)
            continue;
        auto i2=findTa2(match2);
        MixerTreeWidget::collectLeaf(i1,list1);
        MixerTreeWidget::collectLeaf(static_cast<int>(Ana::RecordType::Term),i2,list2);
        autoMatchItems(list1,list2);
        list1.clear();
        list2.clear();
    }
}

void MixerWidget::autoOneLevelOnMatched()
{
    std::vector<QTreeWidgetItem *> list0,list1,list2;
    m_ta1->collect(list0);
    for (auto &i1:list0)
    {
        auto match2=i1->data(2,Qt::UserRole).toInt();
        if (!match2)
            continue;
        auto i2=findTa2(match2);
        MixerTreeWidget::collectChildren(i1,list1,false);
        MixerTreeWidget::collectChildren(i2,list2,false);
        autoMatchItems(list1,list2);
        list1.clear();
        list2.clear();
    }
}
void MixerWidget::autoRecOnMatched()
{
    std::vector<QTreeWidgetItem *> list0,list1,list2;
    m_ta1->collect(list0);
    for (auto &i1:list0)
    {
        auto match2=i1->data(2,Qt::UserRole).toInt();
        if (!match2)
            continue;
        auto i2=findTa2(match2);
        MixerTreeWidget::collect(i1,list1,false);
        MixerTreeWidget::collect(i2,list2,false);
        autoMatchItems(list1,list2);
        list1.clear();
        list2.clear();
    }
}
void MixerWidget::autoLeaf()
{
   std::vector<QTreeWidgetItem *> list1,list2;
   m_ta1->collectLeaf(list1);
   m_ta2->collectLeaf(static_cast<int>(Ana::RecordType::Term),list2);
   autoMatchItems(list1,list2);
}
void MixerWidget::autoLeafLevel()
{
   std::vector<QTreeWidgetItem *> list1,list2;
   MixerTreeWidget::collectLeaf(m_ta1->tree()->currentItem(),list1);
   MixerTreeWidget::collectLeaf(static_cast<int>(Ana::RecordType::Term),m_ta2->tree()->currentItem(),list2);
   autoMatchItems(list1,list2);
}
void MixerWidget::autoPfOnMatched()
{
    std::vector<QTreeWidgetItem *> list0,list1,list2;
    m_ta1->collect(list0);
    for (auto &i1:list0)
    {
        auto match2=i1->data(2,Qt::UserRole).toInt();
        if (!match2)
            continue;
        auto i2=findTa2(match2);
        MixerTreeWidget::collectChildren(i1,list1,false);
        MixerTreeWidget::collectChildren(i2,list2,false);
        pfFirstMatch(list1,list2);
        list1.clear();
        list2.clear();
    }
}
void MixerWidget::autoPfRecOnMatched()
{
    std::vector<QTreeWidgetItem *> list0,list1,list2;
    m_ta1->collect(list0);
    for (auto &i1:list0)
    {
        auto match2=i1->data(2,Qt::UserRole).toInt();
        if (!match2)
            continue;
        auto i2=findTa2(match2);
        MixerTreeWidget::collect(i1,list1,false);
        MixerTreeWidget::collect(i2,list2,false);
        pfFirstMatch(list1,list2);
        list1.clear();
        list2.clear();
    }
}
void MixerWidget::autoParentFiltered()
{
    std::vector<QTreeWidgetItem *> list1,list2;
    MixerTreeWidget::collect(m_ta1->tree()->currentItem(),list1,false);
    MixerTreeWidget::collect(m_ta2->tree()->currentItem(),list2,false);
    pfFirstMatch(list1,list2);
}
void MixerWidget::pfFirstMatch(std::vector<class QTreeWidgetItem *> &list1,std::vector<QTreeWidgetItem *> &list2)
{
    int count=0;
    for (auto &i1:list1)
    {
        if (MixerTreeWidget::isMatched(i1))
            continue;
        for (auto &i2:list2)
        {
            if (MixerTreeWidget::isMatched(i2))
                continue;
            if (matches(i1,i2))
            {
                count++;
                matchItems(i1,i2,false);
            }
        }
    }
    reportMatched(count);
}
QString MixerWidget::removeShorts(const QString  &s)
{
    if (!s.contains('.'))
        return s;
    auto l=s.split(' ');
    QString ret;
    for (auto &i:l)
    {
        auto ns=Ana::latinUnshort(i.toStdString());
        if (ret.size())
            ret+=' ';
        ret+=QString::fromStdString(ns);
    }
    return ret;
}
void MixerWidget::autoFindLeft()
{
    auto i1=m_ta1->tree()->currentItem();
    if (!i1)
        return;
    std::vector<QTreeWidgetItem *> list1,list2;
    m_ta2->collect(list2);
    list1.push_back(i1);
    autoMatchItems(list1,list2);
}
void MixerWidget::autoFindRight()
{
    auto i2=m_ta2->tree()->currentItem();
    if (!i2)
        return;
    std::vector<QTreeWidgetItem *> list1,list2;
    m_ta1->collect(list1);
    list2.push_back(i2);
    autoMatchItems(list1,list2);
}
void MixerWidget::autoPfFindLeft()
{
    auto i1=m_ta1->tree()->currentItem();
    if (!i1 || MixerTreeWidget::isMatched(i1))
        return;
    std::vector<QTreeWidgetItem *> list2;
    m_ta2->collect(list2);
    for (auto &i2:list2)
    {
        if (MixerTreeWidget::isMatched(i2))
            continue;
        if (matches(i1,i2))
        {
            matchItems(i1,i2,false);
            setModified();
        }
    }
}
void MixerWidget::autoPfFindRight()
{
    auto i2=m_ta2->tree()->currentItem();
    if (!i2 || MixerTreeWidget::isMatched(i2))
        return;
    std::vector<QTreeWidgetItem *> list1;
    m_ta1->collect(list1);
    for (auto &i1:list1)
    {
        if (MixerTreeWidget::isMatched(i1))
            continue;
        if (matches(i1,i2))
        {
            matchItems(i1,i2,false);
            setModified();
        }
    }
}
void MixerWidget::autoMatchItems(std::vector<QTreeWidgetItem *> &list1,std::vector<QTreeWidgetItem *> &list2)
{
    std::map<QString,std::pair<int,QTreeWidgetItem*>> m1,m2;
    for (auto &i:list1)
    {
        if (MixerTreeWidget::isMatched(i))
            continue;
        auto t=removeShorts(i->text(1).toLower());
        auto &m=m1[t];
        m.first++;
        m.second=i;
    }
    for (auto &i:list2)
    {
        if (MixerTreeWidget::isMatched(i))
            continue;
        auto t=i->text(1).toLower();
        auto &m=m2[t];
        m.first++;
        m.second=i;
        auto l=i->data(3,m_l==Lat?Qt::UserRole:Qt::UserRole+1).toStringList();
        for (auto &li:l)
        {
            auto &m=m2[li.toLower()];
            m.first++;
            m.second=i;
        }
    }
    int count=0;
    for (auto &i1:m1)
    {
        if (i1.second.first==1)
        {
            auto i2=m2.find(i1.first);
            if (i2!=m2.end())
            {
                if (i2->second.first==1)
                {
                    count++;
                    matchItems(i1.second.second,i2->second.second,false);
                }
            }
        }
    }
    reportMatched(count);
}

void MixerWidget::reportMatched(int c)
{
    if (c)
    {
        QMessageBox::information(this,windowTitle(),QString("Matched %1 records").arg(c));
        setModified(true);
    }
}
QString MixerWidget::parentFiltered(const QString &s,class QTreeWidgetItem *parent)
{
    auto ret=s;
    ret.remove("of",Qt::CaseInsensitive);
    ret.remove("the",Qt::CaseInsensitive);
    do
    {
        auto plist=parent->text(1).split(' ');
        for (auto &p:plist)
        {
            ret.remove(p.trimmed(),Qt::CaseInsensitive);
            if (p.endsWith('s'))
                ret.remove(p.left(p.size()-1),Qt::CaseInsensitive);
        }
        parent=parent->parent();
    }while (parent);
    return ret.simplified();
}

bool MixerWidget::  matches(class QTreeWidgetItem *ta1,class QTreeWidgetItem *ta2,bool tryParentFiltered)
{
    auto text1=removeShorts(ta1->text(1));
    auto text2=ta2->text(1);
    if  (text1.compare(text2,Qt::CaseInsensitive)==0)
        return true;
    auto l=ta2->data(3,m_l==Lat?Qt::UserRole:Qt::UserRole+1).toStringList();
    for (auto &i:l)
    {
        if  (text1.compare(i,Qt::CaseInsensitive)==0)
            return true;
    }
    if (!tryParentFiltered || !ta2->parent())
        return false;

    auto pf=parentFiltered(text2,ta2->parent());
        if (!pf.isEmpty() && (text1.compare(pf,Qt::CaseInsensitive)==0))
        return true;

    for (auto &i:l)
    {
        pf=parentFiltered(i,ta2->parent());
        if  (!pf.isEmpty() && text1.compare(pf,Qt::CaseInsensitive)==0)
            return true;
    }

    return false;
}
