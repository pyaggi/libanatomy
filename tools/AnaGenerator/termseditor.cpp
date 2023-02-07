#include "termseditor.h"
#include "anata1.h"
#include "anata2.h"
#include "anaterm.h"
#include "anqjson.h"
#include <QMessageBox>
#include <QLayout>
#include "anqtermstree.h"
#include "anqcsv.h"
#include "anatermstrings.h"
#include "anqtools.h"
#include <QDir>
TermsEditor::TermsEditor(QWidget *parent)
    : QWidget{parent}
{
    auto lay=new QVBoxLayout(this);
    m_tree=new AnqTermsTree;
    lay->addWidget(m_tree);
}
static int findTr(const QString &latin,const QString &english,const QStringList &latList,const QStringList &enList)
{
    auto idx=latList.indexOf(latin.toLower());
    if (idx>=0)
        return idx;
    return enList.indexOf(english.toLower());
}
QStringList TermsEditor::basicCsvHeaders()
{
    return {"AnaTermId","ParentId","LatinTerm","EnglishTerm","TA1","TA2","Sex","Type"};
}
QStringList TermsEditor::termToCsv(AnaTermTree::Node *n)
{
    QStringList row;
    row.push_back(QString::fromStdString(n->id().toString()));
    if (n->parent())
        row.push_back(QString::fromStdString(n->parent()->id().toString()));
    else
        row.push_back("");
    row.push_back(QString::fromStdString(n->data().latin()));
    row.push_back(QString::fromStdString(n->data().english()));
    row.push_back(QString::fromStdString(n->data().ta1Code()));
    auto ta2=n->id().ta2Number();
    if (ta2)
        row.push_back(QString::number(ta2));
    row.push_back(QString::fromStdString(std::string(Ana::sexCode(n->data().sex()))));
    row.push_back(QString::fromStdString(std::string(Ana::termTypeCode(n->data().type()))));
    return row;
}

bool TermsEditor::csvFullSave(const QString &loc, const AnaTermTranslations &langs)
{
    AnqCSV csv;

    QStringList headers=basicCsvHeaders();
    for (auto &l:langs )
    {
        auto ql=QString::fromStdString(l.language());
        headers.push_back(ql);
        headers.push_back(ql+"_Syns");
    }
    csv.setHeader(headers);

    for (auto &t:(*m_terms.idMap()))
    {
        auto r=termToCsv(t.second);
        for (auto &l:langs)
        {
            auto f=l.find(t.second->id());
            if (f!=l.end())
            {
                r.push_back(QString::fromStdString(f->second.term()));
                r.push_back(Ana::toStringList(f->second.synonyms()).join("; "));
            }
            else
            {
                r.push_back({});
                r.push_back({});
            }
        }
        csv.addRow(r);
    }
    auto str=csv.save(loc);
    if (!str.isEmpty())
    {
        QMessageBox::critical(this,windowTitle(),tr("Can't save CSV: {}").arg(str));
        return false;
    }
    return true;

    return true;
}
bool TermsEditor::csvSave(const QString &loc)
{
    AnqCSV csv;
    csv.setHeader(basicCsvHeaders());
    for (auto &t:(*m_terms.idMap()))
        csv.addRow(termToCsv(t.second));
    auto str=csv.save(loc);
    if (!str.isEmpty())
    {
        QMessageBox::critical(this,windowTitle(),tr("Can't save CSV: {}").arg(str));
        return false;
    }
    return true;
}
bool TermsEditor::jsonSave(const QString &loc)
{
    AnqJson json;
    if (!json.save(loc,m_terms))
    {
        QMessageBox::critical(this,windowTitle(),tr("Can't savee Json: {}").arg(json.errorString()));
        return false;
    }
    return true;
}
bool TermsEditor::jsonFullSave(const QString &loc, const AnaTermTranslations &m)
{
    AnqJson json;
    if (!json.save(loc,m_terms,m))
    {
        QMessageBox::critical(this,windowTitle(),tr("Can't savee Json: {}").arg(json.errorString()));
        return false;
    }
    return true;
}
bool TermsEditor::mergeZAnatomyTranslations(const class AnqCSV &trans, AnaTermTranslations &trs0, const AnaTA1 &ta1Lat, const AnaTA1 &ta1En, const AnaTA1 &ta1Es,const std::map<std::string,int> &tarmap)
{

    std::vector<AnaTermStringsMap*> trs;
    trs.push_back(&trs0.language({"fr"}));
    trs.push_back(&trs0.language({"es"}));
    trs.push_back(&trs0.language({"pt"}));
    trs.push_back(&trs0.language({"it"}));
    trs.push_back(&trs0.language({"nl"}));
    trs.push_back(&trs0.language({"de"}));
    trs.push_back(&trs0.language({"pl"}));
    trs.push_back(&trs0.language({"zh"}));
    trs.push_back(&trs0.language({"en"}));
    trs.push_back(&trs0.language({"en_GK"}));
    trs.push_back(&trs0.language({"la"}));

    QStringList langs[12];

    int cols[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int col=0;
    for (auto &h:trans.header())
    {
        if (h.compare("English",Qt::CaseInsensitive)==0)
            cols[0]=col;
        else if (h.compare("Latin",Qt::CaseInsensitive)==0)
            cols[1]=col;
        else if (h.compare("Français",Qt::CaseInsensitive)==0 ||h.compare("French",Qt::CaseInsensitive)==0)
            cols[2]=col;
        else if (h.compare("Español",Qt::CaseInsensitive)==0 || h.compare("Spanish",Qt::CaseInsensitive)==0)
            cols[3]=col;
        else if (h.compare("Portugues",Qt::CaseInsensitive)==0 || h.compare("Portuguese",Qt::CaseInsensitive)==0)
            cols[4]=col;
        else if (h.compare("Italiano",Qt::CaseInsensitive)==0 || h.compare("Italian",Qt::CaseInsensitive)==0)
            cols[5]=col;
        else if (h.compare("Nederlands",Qt::CaseInsensitive)==0)
            cols[6]=col;
        else if (h.compare("Deutsch",Qt::CaseInsensitive)==0)
            cols[7]=col;
        else if (h.compare("Polski",Qt::CaseInsensitive)==0)
            cols[8]=col;
        else if (h.compare("Chinese",Qt::CaseInsensitive)==0 || h.compare("中國人",Qt::CaseInsensitive)==0)
            cols[9]=col;
        col++;
    }
    if (cols[0]<0 || cols[1]<0)
    {
        QMessageBox::critical(this,windowTitle(),tr("Invalid z-anatomy Translations file"));
        return false;
    }

    for (auto &r:trans.table())
    {
        for (int c=0;c<10 && c<r.size();c++)
        {
            if (cols[c]>=0)
            {
                auto str=formatString(r[c]);
                if (str.startsWith('(') && str.endsWith(')'))
                    str=str.mid(1,str.size()-2).trimmed();
                langs[cols[c]].push_back(str);
                if (c==0)
                    langs[10].push_back(langs[0].back().toLower());
                else if (c==1)
                    langs[11].push_back(langs[1].back().toLower());
            }
            else
                langs[c].push_back({});
        }
    }
    AnaTA1::IdMap cur;
    cur=*m_ta1Lat.idMap();
    int wordsFromTrans=langs[0].size();

    for (auto it=cur.begin();it!=cur.end();)
    {
        auto ft=m_ta1En.data(it->first);
        langs[0].push_back(QString::fromStdString(ft.term()));
        ft=ta1Es.data(it->first);
        langs[3].push_back(QString::fromStdString(ft.term()));
        langs[1].push_back(QString::fromStdString(it->second->data().term()));
        langs[2].push_back({});
        langs[4].push_back({});
        langs[5].push_back({});
        langs[6].push_back({});
        langs[7].push_back({});
        langs[8].push_back({});
        langs[9].push_back({});
        auto f=tarmap.find(it->first);
        if (f!=tarmap.end())
            it=cur.erase(it);
        else
            ++it;
    }
    std::vector<AnaTA1::Node*> insert;
    for (auto &it:cur)
    {
        auto term=m_terms.findTA1(it.first);
        if (!term.atEnd())
            continue;
        insert.push_back(it.second);
        auto p=it.second->parent();
        while (p && !p->id().empty())
        {
            term=m_terms.findTA1(p->id());
            if (term.atEnd())
            {
                insert.push_back(p);
                p=p->parent();
            }
            else
                break;
        }
        if (term.atEnd())
            term.moveToRoot();
        for (auto &i:insert)
        {
            AnaTerm r;
            r.setId(AnaTermId::fromTa1Index(m_ta1Lat.idIndex(i->id())));
            r.setTa1Code(i->id());
            r.setLatin(formatString(i->data().term()).toStdString());
            r.setEnglish(formatString(m_ta1En.data(i->id()).term()).toStdString());
            term.append(r,true);
            auto &us=(*trs[8])[r.id()];
            auto &uk=(*trs[9])[r.id()];
            auto &la=(*trs[10])[r.id()];
            auto &sp=(*trs[1])[r.id()];
            us.setTerm(r.english());
            uk.setTerm(r.english());
            la.setTerm(r.latin());
            sp.setTerm(formatString(ta1Es.data(i->id()).term()).toStdString());
        }
        insert.clear();
    }

    std::map<int,int> usecount;
    for (auto &t:*(m_terms.idMap()))
    {
        for (int c=0;c<8;c++)
            (*trs[c])[t.first];//make all terms to have a translation entry

        int found=findTr(QString::fromStdString(t.second->data().latin()),
                         QString::fromStdString(t.second->data().english()),
                         langs[11],langs[10]);
        if (found>=0)
        {
            usecount[found]++;
            for (int c=0;c<8;c++)
            {
                auto &term=(*trs[c])[t.first];
//                term.setId(t.first);
                term.setTerm(langs[c+2][found].toStdString());
                term.setStatus(Ana::TrStatus::Translated);
            }
        }
        auto &latSyn=(*trs[10])[t.first].synonyms();
        auto &enSyn=(*trs[8])[t.first].synonyms();
        for (auto &w:latSyn)
        {
            found=findTr(QString::fromStdString(w),{},langs[1],langs[0]);
            if (found>0)
            {
                usecount[found]++;
                for (int c=0;c<8;c++)
                {
                    auto &term=(*trs[c])[t.first];
//                    term.setId(t.first);
                    term.addSynonym(langs[c+2][found].toStdString());
                    term.setStatus(Ana::TrStatus::Translated);
                }
            }
        }
        for (auto &w:enSyn)
        {
            found=findTr({},QString::fromStdString(w),langs[1],langs[0]);
            if (found>0)
            {
                usecount[found]++;
                for (int c=0;c<8;c++)
                {
                    auto &term=(*trs[c])[t.first];
//                    term.setId(t.first);
                    term.addSynonym(langs[c+2][found].toStdString());
                    term.setStatus(Ana::TrStatus::Translated);
                }
            }
        }
    }
    int newcount=30001;
    AnaTermTree::Node *unode;
    {
        AnaTerm t;
        t.setId(AnaTermId::fromInteger(newcount));
        newcount++;
        t.setEnglish("Parent unresolved");
        t.setLatin("Parent unresolved");
        unode=new AnaTermTree::Node(std::move(t));
        m_terms.appendChild(unode);
    }

    for (int r=0;r<wordsFromTrans;r++)
    {
        if (usecount[r]==0)
        {
            AnaTerm t;
            t.setId(AnaTermId::fromInteger(newcount));
            newcount++;
            t.setEnglish(langs[0][r].toStdString());
            t.setLatin(langs[1][r].toStdString());
            for (int c=0;c<8;c++)
            {
                auto &term=(*trs[c])[t.id()];
//                term.setId(t.id());
                term.setTerm(langs[c+2][r].toStdString());
                term.setStatus(Ana::TrStatus::Translated);
            }
            for (int c=8;c<=9;c++)
            {
                auto &term=(*trs[c])[t.id()];
//                term.setId(t.id());
                term.setTerm(t.english());
                term.setStatus(Ana::TrStatus::Translated);
            }
            auto &term=(*trs[10])[t.id()];
//            term.setId(t.id());
            term.setTerm(t.latin());
            term.setStatus(Ana::TrStatus::Translated);
            unode->appendChild(new AnaTermTree::Node(std::move(t)));
        }
    }
    return true;
}
static const char*EnsureTranslations[]={"en","en_GB","la","de","fr","it","la","nl","pl","pt","zh"};

bool TermsEditor::open(const QString &s, AnaTermTranslations &trs)
{
    m_terms.clear();
    m_ta1En.clear();
    m_ta1Lat.clear();
    m_ta2.clear();

    AnqJson json;
    AnaTA1 ta1Es;
    std::map<int,QString> tamap;
    if (!json.loadTa1(s+"/sources/ta/ta1-la.json",m_ta1Lat))
    {
        QMessageBox::critical(this,windowTitle(),tr("Invalid ta1-la.json file"));
        return false;
    }
    if (!json.loadTa1(s+"/sources/ta/ta1-en.json",m_ta1En))
    {
        QMessageBox::critical(this,windowTitle(),tr("Invalid ta1-la.json file"));
        return false;
    }
    if (!json.loadTa2(s+"/sources/ta/ta2.json",m_ta2))
    {
        QMessageBox::critical(this,windowTitle(),tr("Invalid ta2.json file"));
        return false;
    }
    if (!json.loadTaMap(s+"/sources/ta/ta-map.json",tamap))
    {
        QMessageBox::critical(this,windowTitle(),tr("Invalid ta-map.json file"));
        return false;
    }



    json.loadTa1(s+"/sources/ta/ta1-es.json",ta1Es);


    for (auto &ti:EnsureTranslations)
        trs.addLanguage(ti);
    auto e=QDir(s+"/content").entryList();
    for (auto &fe:e)
    {
        auto idx=fe.lastIndexOf('.');
        if (idx>11 && fe.startsWith("ana_terms-"))
        {
            auto lang=fe.mid(10,idx-10);
            trs.addLanguage(lang.toStdString());
        }
    }




    parseTa2(&m_ta2,&m_terms,trs.language("en"),trs.language("en_GB"),trs.language("la"));

    std::map<std::string,int> tarmap;
    for (auto &m:tamap)
    {
        tarmap[m.second.toStdString()]=m.first;
        auto n=m_terms.node(AnaTermId::fromTa2(m.first));
        if (n)
            (*n)->setTa1Code(m.second.toStdString());
    }

    parseTa1(&m_ta1Lat,&m_terms,ta1Es,trs.language("en"),trs.language("la"),trs.language("es"));

    AnqCSV defs;
    if (defs.load(s+"/content/ana_terms_definitions.csv").isEmpty())
    {
        if (defs.header().size()!=8)
        {
            QMessageBox::critical(this,windowTitle(),tr("Invalid ana_terms_definitions.csv file"));
            return false;
        }
        for (int c=0;c<defs.table().size();c++)
        {
            auto ret=parseDefs(defs,c,trs.language("en"),trs.language("la"));
            if (!ret.isEmpty())
            {
                QMessageBox::critical(this,windowTitle(),tr("Error parsing definitions: %1").arg(ret));
                return false;
            }
        }
    }
    //ensure all terms are available in translations
    for (auto &term:*m_terms.idMap())
    {
        for (auto &tr:trs)
        {
            tr[term.first];
        }
    }

//    AnqCSV trans;
//    if (trans.load(s+"/sources/z-anatomy/Translations.csv").isEmpty())
//        mergeZAnatomyTranslations(trans,trs,m_ta1Lat,m_ta1En,ta1Es,tarmap);


    m_tree->loadTerms(m_terms);
    m_tree->generateStats(1,6);
    return true;
}
QString TermsEditor::parseDefs(const class AnqCSV &defs,int row,class AnaTermStringsMap &enUs,class AnaTermStringsMap &lat)
{
    auto &r=defs.table()[row];

    auto id=AnaTermId::fromSeqId(r[0].toInt());
    auto f=m_terms.node(id);
    if (f)
        return {};

    AnaTermId pid;
    auto ipid=r[1].toInt();
    if (ipid<100000)
    {
        if (ipid>=0 && ipid>=defs.table().size())//parent not in definitions
            return tr("Parent row %1 not included, at row %2").arg(ipid).arg(row);
        if (ipid==row)
            return tr("Parent error, same value, at row %1").arg(row);
        if (ipid>row)
        {
            auto ret=parseDefs(defs,ipid,enUs,lat);
            if (!ret.isEmpty())
                return ret;
        }
        if (ipid>=0)
            pid=AnaTermId::fromSeqId(ipid);
    }
    else
        pid.setIdValue(ipid);
    auto parent=m_terms.node(pid);
    if (!parent)
    {
        if (!pid.isNull())
            return tr("Parent error, not found %1, at row %2").arg(pid.toInt()).arg(row);
        parent=&m_terms;
    }

    AnaTerm nt;
    nt.setId(id);
    nt.setParentId(pid);
    nt.setLatin(formatString(r[2]).toStdString());
    nt.setEnglish(formatString(r[3]).toStdString());
    nt.setType(Ana::termTypeFromCode(r[6].toStdString()));
    nt.setSex(Ana::sexFromCode(r[7].toStdString()));
    auto &us=enUs[nt.id()];
    auto &la=lat[nt.id()];
    la.setTerm(nt.latin());
    us.setTerm(nt.english());
    la.setSynonyms(splitFormat(r[4]));
    us.setSynonyms(splitFormat(r[5]));
    parent->appendChild(new AnaTermTree::Node(std::move(nt)));
    return {};
}
void TermsEditor::parseTa1(AnaTA1Cursor ta1, AnaTermCursor term, const AnaTA1 &taEs, class AnaTermStringsMap &enUs, class AnaTermStringsMap &lat,class AnaTermStringsMap &esp)
{
    do
    {
        auto fc=ta1.firstChildCursor();
        if (!ta1->id().empty())
        {
            auto old=m_terms.findTA1(ta1->id());
            if (!old.atEnd())
            {
                if (!fc.atEnd())
                    parseTa1(fc,old,taEs,enUs,lat,esp);
            }
            else
            {
                AnaTerm nt;
                nt.setId(AnaTermId::fromTa1Index(m_ta1Lat.idIndex(ta1->id())));
                nt.setTa1Code(ta1->id());
                nt.setLatin(ta1->term());
                nt.setEnglish(m_ta1En.data(ta1->id()).term());

                auto &us=enUs[nt.id()];
                auto &la=lat[nt.id()];
                auto &es=esp[nt.id()];

                la.setTerm(nt.latin());
                la.setStatus(Ana::TrStatus::Translated);
                if (!nt.english().empty())
                {
                    us.setTerm(nt.english());
                    us.setStatus(Ana::TrStatus::Translated);
                }
                auto str=taEs.data(ta1->id()).term();
                if (!str.empty())
                {
                    es.setTerm(str);
                    es.setStatus(Ana::TrStatus::Translated);
                }

                auto n=term.append(std::move(nt),false);
                if (!fc.atEnd())
                    parseTa1(fc,n,taEs,enUs,lat,esp);
            }

        }
        else
        {
            if (!fc.atEnd())
                parseTa1(fc,term,taEs,enUs,lat,esp);
        }
        ta1=ta1.nextSiblingCursor();
    }while (!ta1.atEnd());
}
std::vector<std::string> TermsEditor::formatStrings(const std::vector<std::string> &v)
{
    std::vector<std::string> ret;
    for (auto &i:v)
    {
        auto ns=formatString(i).toStdString();
        if (std::find(ret.begin(),ret.end(),ns)==ret.end())
            ret.push_back(ns);
    }
    return ret;
}
std::vector<std::string> TermsEditor::splitFormat(const QString &s)
{
    auto l=s.split(";");
    std::vector<std::string> ret;
    for (auto &i:l)
    {
        auto ns=formatString(i).toStdString();
        if (std::find(ret.begin(),ret.end(),ns)==ret.end())
            ret.push_back(ns);
    }
    return ret;
}
QString TermsEditor::formatString(const std::string &s)
{
    if (!s.size())
        return {};
    auto r=QString::fromStdString(s).toLower().trimmed();
    r[0]=r[0].toUpper();
    return r;
}

void TermsEditor::parseTa2(AnaTA2Cursor ta2,AnaTermCursor term,class AnaTermStringsMap &enUs,class AnaTermStringsMap &enUk,class AnaTermStringsMap &lat)
{
    do
    {
        auto fc=ta2.firstChildCursor();
        if (ta2->id())
        {
            AnaTerm nt;
            nt.setId(AnaTermId::fromTa2(ta2->id()));
            nt.setParentId(term->id());
            nt.setLatin(formatString(ta2->latin()).toStdString());
            nt.setEnglish(formatString(ta2->us()).toStdString());
            nt.setSex(ta2->sex());
            nt.setType(ta2->type()==Ana::RecordType::Heading?Ana::TermType::Group:Ana::TermType::Unknown);
            nt.setTa2Inconsistent(ta2->inconsistent());

            auto &us=enUs[nt.id()];
            auto &uk=enUk[nt.id()];
            auto &la=lat[nt.id()];

            us.setTerm(formatString(ta2->us()).toStdString());
            us.setSynonyms(formatStrings(ta2->syn()));
            uk.setTerm(formatString(ta2->gb()).toStdString());
            uk.setSynonyms(formatStrings(ta2->syn()));
            la.setTerm(formatString(ta2->latin()).toStdString());
            la.setSynonyms(formatStrings(ta2->latinSyn()));
            us.setStatus(Ana::TrStatus::Translated);
            uk.setStatus(Ana::TrStatus::Translated);
            la.setStatus(Ana::TrStatus::Translated);



            auto n=term.append(std::move(nt),false);
            if (!fc.atEnd())
                parseTa2(fc,n,enUs,enUk,lat);
        }
        else
        {
            if (!fc.atEnd())
                parseTa2(fc,term,enUs,enUk,lat);
        }
        ta2=ta2.nextSiblingCursor();
    }while (!ta2.atEnd());
}
