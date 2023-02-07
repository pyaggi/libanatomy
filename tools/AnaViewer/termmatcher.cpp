#include "termmatcher.h"
#include "anqjson.h"
#include "anaterm.h"
#include <QMessageBox>
#include <QLayout>
#include <QSplitter>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QProgressBar>
#include <QCoreApplication>
#include <QLabel>
#include <QFileDialog>
#include <filesystem>
TermMatcher::TermMatcher(QWidget *parent)
    : QWidget{parent}
{
    auto lay=new QVBoxLayout(this);
    m_pw=new QWidget;
    auto pwlay=new QHBoxLayout(m_pw);
    lay->addWidget(m_pw);
    m_barinfo=new QLabel;
    pwlay->addWidget(m_barinfo);
    m_bar=new QProgressBar;
    pwlay->addWidget(m_bar);
    m_bw=new QWidget;
    lay->addWidget(m_bw);
    auto bl=new QHBoxLayout(m_bw);
    auto but=new QPushButton(tr("Identical Unique Match"));
    bl->addWidget(but);
    connect(but,&QPushButton::clicked,this,&TermMatcher::autoMatchIdentical);
    but=new QPushButton(tr("IUM on Matched"));
    bl->addWidget(but);
    connect(but,&QPushButton::clicked,this,&TermMatcher::autoMatchIdenticalOnMatched);
    but=new QPushButton(tr("Abort"));
    pwlay->addWidget(but);
    connect(but,&QPushButton::clicked,this,&TermMatcher::abort);
    auto sp=new QSplitter;
    sp->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    lay->addWidget(sp);
    m_json=new ViewerTree(ViewerTree::Mode::Generic);
    m_terms=new ViewerTree(ViewerTree::Mode::Terms);
    sp->addWidget(m_json);
    sp->addWidget(m_terms);

    m_pw->setHidden(true);

    connect(m_json->tree(),&QTreeWidget::itemActivated,this,&TermMatcher::jsonAct);
    connect(m_terms->tree(),&QTreeWidget::itemActivated,this,&TermMatcher::termsAct);

    auto l=new QLabel;
    m_json->addControlWidget(l);

    but=new QPushButton(tr("Load Matches"));
    m_json->addControlWidget(but);
    connect(but,&QPushButton::clicked,this,qOverload<>(&TermMatcher::loadMatches));
    but=new QPushButton(tr("Save Matches"));
    m_json->addControlWidget(but);
    connect(but,&QPushButton::clicked,this,&TermMatcher::exportJson);

}
void TermMatcher::loadMatches()
{
    QString dst=m_mfile;
    if (QMessageBox::question(this,windowTitle(),tr("Load from %1").arg(dst),QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes)
    {
        dst=QFileDialog::getOpenFileName(this,windowTitle(),QString::fromStdString(std::filesystem::path(dst.toStdString()).parent_path().u8string()));
        if (dst.isEmpty())
            return;
    }
    loadMatches(dst);
}
void TermMatcher::loadMatches(const QString &f)
{
    m_matched=0;
    AnqJson json;
    auto  a=json.loadArray(f);
    if (!a.size())
    {
        QMessageBox::critical(this,windowTitle(),json.errorString());
        return ;
    }
    m_mfile=f;
    start(tr("Loading matches"));
    auto t=m_json->tree();
    auto max=t->topLevelItemCount();
    m_bar->setRange(0,max);
    for (int c=0;c<max && !m_abort;c++)
    {
        auto i=t->topLevelItem(c);
        loadMatches(i,a);
        m_bar->setValue(c);
        qApp->processEvents();
    }
    updateStats();
    end();
}
void TermMatcher::loadMatches(QTreeWidgetItem *i,const QJsonArray &a)
{
    auto n=i->data(0,Qt::UserRole+1).toString();
    i->setText(m_tcol,{});
    for (const auto &ia:a)
    {
       if (m_abort)
           break;
       auto o=ia.toObject();
       if (o[m_json->nameKey()]==n)
       {
           auto tid=o["tid"].toInt();
           if (tid)
           {
               i->setText(m_tcol,QString::number(tid));
               auto t=findTerm(tid);
               if (!t)
                   m_json->highlight(i,m_tcol);
               m_matched++;
           }
           auto max=i->childCount();
           for (int c=0;c<max;c++)
               loadMatches(i->child(c),o["z"].toArray());
           break;
       }
    }

}
void TermMatcher::exportJson()
{
    QString dst=m_mfile;
    if (QMessageBox::question(this,windowTitle(),tr("Save to %1").arg(dst),QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes)
    {
        dst=QFileDialog::getSaveFileName(this,windowTitle(),QString::fromStdString(std::filesystem::path(dst.toStdString()).parent_path().u8string()));
        if (dst.isEmpty())
            return;
    }
    QFile file(dst);
    if (!file.open(QFile::WriteOnly|QFile::Truncate))
    {
        QMessageBox::critical(this,windowTitle(),file.errorString());
        return ;
    }
    m_mfile=dst;

    QJsonArray a;
    auto t=m_json->tree();
    auto max=t->topLevelItemCount();
    for (int c=0;c<max;c++)
    {
        auto i=t->topLevelItem(c);
        exportItemJson(i,a);
    }
    file.write(QJsonDocument(a).toJson());

}
void TermMatcher::exportItemJson(QTreeWidgetItem *i,QJsonArray &a)
{
    auto o=i->data(0,Qt::UserRole).toJsonObject();
    auto mid=i->text(m_tcol);
    if (!mid.isEmpty())
        o["tid"]=mid.toInt();
    auto cc=i->childCount();
    if (cc)
    {
        QJsonArray z;
        for (int c=0;c<cc;c++)
            exportItemJson(i->child(c),z);
        o["z"]=std::move(z);
    }
    a.push_back(std::move(o));
}
QTreeWidgetItem *TermMatcher::findTerm(int id)
{
    auto f=m_terms->tree()->findItems(QString::number(id),Qt::MatchFlag::MatchRecursive,0);
    if  (f.size())
        return f.front();
    return nullptr;
}
void TermMatcher::jsonAct(QTreeWidgetItem *i,int col)
{
    m_terms->tree()->clearSelection();
    auto id=matched(i);
    if (!id.isEmpty())
    {
        auto r=findTerm(id.toInt());
        if (r)
        {
            m_terms->expand(r);
            r->setSelected(true);
            m_terms->tree()->scrollToItem(r);
        }
    }
}
void TermMatcher::termsAct(QTreeWidgetItem *i,int col)
{
    m_json->tree()->clearSelection();
    auto id=i->text(0);
    auto f=m_json->tree()->findItems(id,Qt::MatchFlag::MatchRecursive,m_tcol);
    for (auto &r:f)
    {
        m_json->expand(r);
        r->setSelected(true);
        m_json->tree()->scrollToItem(r);
    }
}
void TermMatcher::start(const QString &info)
{
    m_barinfo->setText(info);
    m_pw->setHidden(false);
    m_bw->setDisabled(true);
    m_abort=false;
}
void TermMatcher::end()
{
    m_bw->setDisabled(false);
    m_pw->setHidden(true);
    m_abort=false;
}

int TermMatcher::matchIdentical(const std::vector<QTreeWidgetItem *>a,const std::vector<QTreeWidgetItem *>b,bool report)
{
    int count=0,matches=0,umatches=0,max=a.size()*b.size(),unmatched=0;
    if (report)
    {
        start(tr("Matching IUM"));
        m_bar->setRange(0,max);
        m_bar->setValue(count);
    }
    std::map<QTreeWidgetItem*,std::vector<QTreeWidgetItem*>> pos;

    for (auto &ia:a)
    {
        if (m_abort)
            break;
        if (isMatched(ia))
            continue;
        unmatched++;
        for (auto &ib:b)
        {
            if (ia->data(0,Qt::UserRole+2)==ib->data(0,Qt::UserRole+2))
            {
                pos[ia].push_back(ib);
                matches++;
            }
            count++;
        }
        if (report)
        {
            m_bar->setValue(count);
            qApp->processEvents();
        }
    }
    for (auto &p:pos)
    {
        if (p.second.size()==1)
        {
            match(p.first,p.second.front());
            umatches++;
        }
//        else
//            qDebug()<<p.second.front()->text(1);
    }
    if (report)
    {
        end();
        QMessageBox::information(this,windowTitle(),tr("Tested %3 unmatched\n%1 Matches, %2 Unique Matches").arg(matches).arg(umatches).arg(unmatched));
    }
    return umatches;
}
void TermMatcher::prepareForMatch()
{
    std::vector<QTreeWidgetItem*> items;
    m_terms->collect(items);
    for (auto &i:items)
    {
        auto s=i->text(1).toLower();
        i->setData(0,Qt::UserRole+2,s);
    }

}
void TermMatcher::autoMatchIdentical()
{
    prepareForMatch();
    m_json->clearHighlight();
    std::vector<QTreeWidgetItem*> a,b;
    m_json->collect(a);
    m_terms->collect(b);
    matchIdentical(a,b);
}
void TermMatcher::autoMatchIdenticalOnMatched()
{
    prepareForMatch();
    m_json->clearHighlight();
    start(tr("Matching IUM on Matched"));
    std::vector<QTreeWidgetItem*> a,b,c;
    m_json->collect(a);
    QString m;
    int matchedCount=0;
    int add;
    int loop=1;
    do
    {
        m_barinfo->setText(tr("Matching IUM on Matched / pass %1").arg(loop));
        add=0;
        m_bar->setRange(0,a.size());
        m_bar->setValue(0);
        for (auto it=a.begin();it!=a.end();++it)
        {
            if (m_abort)
                break;
            m=matched(*it);
            if (!m.isEmpty())
            {
                b.clear();
                m_json->collectChildren(*it,b,false);
                auto f=m_terms->tree()->findItems(m,Qt::MatchFlag::MatchRecursive,0);
                if (f.size())
                {
                    c.clear();
                    m_terms->collectChildren(f.first(),c,false);
                    add+=matchIdentical(b,c,false);
                }
            }
            m_bar->setValue(m_bar->value()+1);
            qApp->processEvents();
        }
        matchedCount+=add;
    }while (add && !m_abort);
    QMessageBox::information(this,windowTitle(),tr("%1 Unique Matches").arg(matchedCount));
    end();
}
QString TermMatcher::matched(QTreeWidgetItem *j)
{
    return j->text(m_tcol);
}
bool TermMatcher::isMatched(QTreeWidgetItem *j)
{
    return !j->text(m_tcol).isEmpty();
}
void TermMatcher::match(QTreeWidgetItem *j,QTreeWidgetItem *t)
{
    if (j->text(m_tcol).isEmpty())
        m_matched++;
    j->setText(m_tcol,t->text(0));
    m_json->highlight(j);
    m_json->expand(j);
    m_json->tree()->scrollToItem(j);
    updateStats();
}
void TermMatcher::updateStats()
{
    auto l=static_cast<QLabel*>(m_json->controlWidget(4));
    l->setText(tr("Matched: %1/%2").arg(m_matched).arg(m_total));
}
bool TermMatcher::load(const QString &jsonFile,ViewerTree::Mode jsonMode,const QString &termsFile)
{
    AnqJson json;
    auto arr=json.loadArray(jsonFile);
    if (arr.empty())
    {
        QMessageBox::critical(this,windowTitle(),tr("Invalid JSON File: %1").arg(json.errorString()));
        return false;
    }
    m_file=jsonFile;

    std::filesystem::path src(m_file.toStdString());
    auto dstFile=src.filename().stem().u8string();
    dstFile+="_matched";
    dstFile+=src.filename().extension().u8string();
    m_mfile=QString::fromStdString((src.parent_path()/dstFile).u8string());


    m_json->setMode(jsonMode);
    m_json->load(arr);
    m_tcol=m_json->tree()->columnCount();
    m_json->tree()->setColumnCount(m_tcol+1);
    m_json->tree()->headerItem()->setText(m_tcol,tr("Term"));

    std::vector<QTreeWidgetItem*> items;
    m_json->collect(items);
    m_total=items.size();
    m_matched=0;
    for (auto &i:items)
    {
        auto s=i->text(0).toLower();
        if (s.startsWith('(') && s.endsWith(')'))
            s=s.mid(1,s.size()-2);
        i->setData(0,Qt::UserRole+2,s);
    }

    arr=json.loadArray(termsFile);
    if (arr.empty())
    {
        QMessageBox::critical(this,windowTitle(),tr("Invalid Terms file: %1").arg(json.errorString()));
        return false;
    }
    m_terms->load(arr);

    updateStats();

//    AnaTermTree terms;
//    std::vector<class AnaTermStringsMap> trs ;
//    if (!json.loadTerms(termsFile,terms,trs))
//    {
//        QMessageBox::critical(this,windowTitle(),tr("Invalid resource: %1").arg(json.errorString()));
//        return false;
//    }
    return true;
}
