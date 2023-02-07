#include "anqjson.h"
#include "anatomy.h"
#include "anatermstrings.h"
#include <QFile>
AnqJson::AnqJson()
{

}
std::vector<std::string> AnqJson::toStringList(const QJsonArray& a)
{
    std::vector<std::string> r;
    for (const auto &i:a)
        r.push_back(toString(i).toStdString());
    return r;
}
QStringList AnqJson::toQStringList(const QJsonArray& a)
{
    QStringList r;
    for (const auto &i:a)
        r.push_back(toString(i));
    return r;
}
QJsonArray AnqJson::toArray(const std::vector<std::string>&v)
{
    QJsonArray a;
    for (auto &i:v)
        a.push_back(QString::fromStdString(i));
    return a;
}
QString AnqJson::toString(const QJsonValue &v)
{
    if (v.isString())
        return v.toString();
    if  (v.isBool())
        return v.toBool()?"true":"false";
    if (v.isDouble())
    {
        auto i=v.toInt();
        auto f=v.toDouble();
        if (f==double(i))
            return QString::number(i);
        return QString::number(f);
    }
    if (v.isArray())
        return toQStringList(v.toArray()).join("; ");
    return v.toString();
}
int AnqJson::loadTa1(const QJsonArray &a,AnaTA1Cursor c )
{
    int ret=0;
    for (const auto &i:a)
    {
        auto o=i.toObject();
        if (o.isEmpty())
            continue;
        auto id=o["c"].toString().toStdString();
        auto term=o["t"].toString().toStdString();
        auto a=o["z"].toArray();
        auto n=c.append({id,term},false);
        ret++;
        if (a.size())
            ret+=loadTa1(a,n);
    }
    return ret;
}
int AnqJson::loadTa2(const QJsonArray &a,AnaTA2Cursor c )
{
    int ret=0;
    for (const auto &i:a)
    {
        auto o=i.toObject();
        if (o.isEmpty())
            continue;
        auto id=o["tai"].toInt();
        if (!id)
            id=o["id"].toInt();
        if (!id)
            continue;
        auto lat=o["la"].toString().toStdString();
        auto lats=toStringList(o["ls"].toArray());
        auto syn=toStringList(o["es"].toArray());
        auto uk=o["uk"].toString().toStdString();
        auto us=o["us"].toString().toStdString();
        auto ot=toStringList(o["ot"].toArray());
        auto sex=o["sex"].toString().toStdString();
        auto in=o["in"].toBool();
        auto ty=o["ty"].toString().toStdString();
        auto a=o["ch"].toArray();
        if (a.isEmpty())
            a=o["z"].toArray();
        if  (sex.empty())
            sex='B';
        auto n=c.append({id,lat,lats,us,uk,syn,ot,
                         Ana::sexFromCode(sex),
                         ty=="T"?Ana::RecordType::Term:Ana::RecordType::Heading,in},false);
        ret++;
        if (a.size())
            ret+=loadTa2(a,n);
    }
    return ret;

}
int AnqJson::loadTa2(const QString &file,AnaTA2 &ta2 )
{

    auto a=loadArray(file);
    if (a.isEmpty())
        return {};
    return loadTa2(a,AnaTA2Cursor(&ta2));

}
int AnqJson::loadTerms(const QString &file,class AnaTermTree &tt ,std::vector<class AnaTermStringsMap> &trs )
{
    auto a=loadArray(file);
    if (a.isEmpty())
        return {};
    return loadTerms(a,AnaTermCursor(&tt),trs);
}
int AnqJson::loadTaMap(const QString &file,std::map<int,QString> & map)
{
    auto a=loadArray(file);
    if (a.isEmpty())
        return {};
    for (const auto &i:a)
    {
        auto o=i.toObject();
        if (o.isEmpty())
            continue;
        auto ta1=o["ta1"].toString();
        auto ta2=o["ta2"].toInt();
        if (ta1.isEmpty() || !ta2)
            continue;
        map[ta2]=ta1;
    }
    return map.size();

}
int AnqJson::loadTa1(const QString &file,AnaTA1 &ta1 )
{

    auto a=loadArray(file);
    if (a.isEmpty())
        return {};
    return loadTa1(a,AnaTA1Cursor(&ta1));

}
QJsonDocument AnqJson::loadDocument(const QString &file)
{
    QFile f(file);
    if (!f.open(QFile::ReadOnly))
    {
        m_error=f.errorString()+"("+file+")";
        return {};
    }
    QJsonParseError err;
    auto doc=QJsonDocument::fromJson(f.readAll(),&err);
    if (err.error!=QJsonParseError::NoError)
    {
        m_error=err.errorString();
        return {};
    }
    return doc;
}
QJsonArray AnqJson::loadArray(const QString &file)
{
    auto doc=loadDocument(file);
    if (doc.isNull())
        return {};
    if (!doc.isArray())
    {
        m_error="Not a json array";
        return {};
    }
    auto a=doc.array();
    if (a.empty())
    {
        m_error="Empty json array";
        return {};
    }
    return a;
}
bool AnqJson::saveDocument(const QString &file,const QJsonDocument &doc)
{
    QFile f(file);
    if (!f.open(QFile::WriteOnly|QFile::Truncate))
    {
        m_error=f.errorString()+"("+file+")";
        return false;
    }
    if (f.write(doc.toJson())<0)
    {
        m_error=f.errorString();
        return false;
    }
    return true;
}
void AnqJson::save(QJsonArray &a,const class AnaTermTree &terms,AnaTA1ConstCursor cur,const class AnaTA1 &english, const std::vector<AnaTermStringsMap> &m)
{
    while (!cur.atEnd())
    {
        if (!cur->id().empty())
        {
            QJsonObject o;
            o["id"]=QString::fromStdString(cur->id());
            auto p=cur.parentCursor();
            if (!p.atEnd())
                o["pid"]=QString::fromStdString(p->id());
            o["la"]=QString::fromStdString(cur.data().term());
            auto f=english.node(cur->id());
            if (f)
                o["en"]=QString::fromStdString(f->data().term());

            auto ta=terms.findTA1(cur->id());

            for (auto &t:m)
            {
                auto f=t.find(ta->id());
                if (f!=t.end())
                {
                    QJsonObject tra;
                    tra["tr"]=QString::fromStdString(f->second.term());
                    tra["sy"]=toArray(f->second.synonyms());
                    o["t-"+QString::fromStdString(t.language())]=std::move(tra);
                }
            }

            p=cur.firstChildCursor();
            if (!p.atEnd())
            {
                QJsonArray ch;
                save(ch,terms,p,english,m);
                o["z"]=std::move(ch);
            }
            a.push_back(std::move(o));
        }
        cur=cur.nextSiblingCursor();
    }
}
void AnqJson::save(QJsonArray &a,const class AnaTermTree &terms,AnaTA2ConstCursor cur, const std::vector<AnaTermStringsMap> &m)
{
    while (!cur.atEnd())
    {
        if (cur->id())
        {
            QJsonObject o;
            o["id"]=cur->id();
            auto p=cur.parentCursor();
            if (!p.atEnd())
                o["pid"]=p->id();
            o["la"]=QString::fromStdString(cur.data().latin());
            o["en"]=QString::fromStdString(cur.data().us());
            o["sex"]=QString::fromStdString(std::string(Ana::sexCode(cur.data().sex())));
            o["type"]=QString::fromStdString(std::string(Ana::recordTypeCode(cur.data().type())));
            if (cur.data().inconsistent())
                o["inc"]=true;

            auto tid=AnaTermId::fromTa2(cur->id());

            for (auto &t:m)
            {
                auto f=t.find(tid);
                if (f!=t.end())
                {
                    QJsonObject tra;
                    tra["tr"]=QString::fromStdString(f->second.term());
                    tra["sy"]=toArray(f->second.synonyms());
                    o["t-"+QString::fromStdString(t.language())]=std::move(tra);
                }
            }

            p=cur.firstChildCursor();
            if (!p.atEnd())
            {
                QJsonArray ch;
                save(ch,terms,p,m);
                o["z"]=std::move(ch);
            }
            a.push_back(std::move(o));
        }
        cur=cur.nextSiblingCursor();
    }
}
int AnqJson::loadTerms(const QJsonArray &a,AnaTermCursor c, std::vector<class AnaTermStringsMap> &trs )
{
    int ret=0;
    for (const auto &i:a)
    {
        auto o=i.toObject();
        if (o.isEmpty())
            continue;
        AnaTermId id,pid;
        id.setIdValue(o["id"].toInt());
        if (!id.isValid())
            continue;
        pid.setIdValue(o["pid"].toInt());
        if (!pid.isValid())
        {
            if (c.parentNode())
                pid=c.parentNode()->id();
        }
        auto lat=o["la"].toString().toStdString();
        auto en=o["en"].toString().toStdString();
        auto sex=o["sex"].toString().toStdString();
        auto type=o["type"].toString().toStdString();
        auto a=o["ch"].toArray();
        if (a.isEmpty())
            a=o["z"].toArray();

        AnaTerm nt;
        nt.setId(id);
        nt.setParentId(pid);
        nt.setTa1Code(o["ta1"].toString().toStdString());
        nt.setLatin(lat);
        nt.setEnglish(en);
        nt.setSex(Ana::sexFromCode(sex));
        nt.setType(Ana::termTypeFromCode(type));
        nt.setTa2Inconsistent(o["tainc"].toBool());

        const auto &tra=o["trs"].toArray();
        for (const auto &tr:tra)
        {
            auto lang=tr["l"].toString().toStdString();
            auto term=tr["t"].toString().toStdString();
            auto syn=tr["s"].toArray();
            if (!lang.empty() && !term.empty())
            {
                AnaTermStrings *ts=nullptr;
                for (auto &t:trs)
                {
                    if (t.language()==lang)
                    {
                        ts=&t[id];
                        break;
                    }
                }
                if (!ts)
                {
                    trs.push_back(AnaTermStringsMap(lang));
                    ts=&trs.back()[id];
                }
                ts->setSynonyms({});
                ts->setTerm(term);
                for (const auto &s:syn)
                    ts->addSynonym(s.toString().toStdString());
            }
        }

        auto n=c.append(nt,false);
        ret++;
        if (a.size())
            ret+=loadTerms(a,n,trs);
    }
    return ret;
}
void AnqJson::save(QJsonArray &a,AnaTermConstCursor cur,const std::vector<AnaTermStringsMap> &m)
{
    while (!cur.atEnd())
    {
        if (!cur->id().isNull())
        {
            QJsonObject o;
            o["id"]=cur->id().toInt();
            auto p=cur.parentCursor();
            if (!p.atEnd())
                o["pid"]=p->id().toInt();
            o["ta1"]=QString::fromStdString(cur.data().ta1Code());
            auto ta2id=p->id().ta2Number();
            if (ta2id)
                o["ta2"]=ta2id;
            o["la"]=QString::fromStdString(cur.data().latin());
            o["en"]=QString::fromStdString(cur.data().english());
            o["sex"]=QString::fromStdString(std::string(Ana::sexCode(cur.data().sex())));
            o["type"]=QString::fromStdString(std::string(Ana::termTypeCode(cur.data().type())));
            if (cur.data().ta2Inconsistent())
                o["tainc"]=true;

            QJsonArray tra;
            for (auto &t:m)
            {
                auto f=t.find(cur->id());
                if (f!=t.end())
                {
                    QJsonObject tr;
                    tr["l"]=QString::fromStdString(t.language());
                    tr["t"]=QString::fromStdString(f->second.term());
                    tr["s"]=toArray(f->second.synonyms());
                    tra.push_back(tr);
                }
            }
            if (tra.size())
                o["trs"]=std::move(tra);

            p=cur.firstChildCursor();
            if (!p.atEnd())
            {
                QJsonArray ch;
                save(ch,p,m);
                o["z"]=std::move(ch);
            }
            a.push_back(std::move(o));
        }
        cur=cur.nextSiblingCursor();
    }
}
bool AnqJson::save(const QString &file,const class AnaTermTree &terms,const class AnaTA1 &ta,const class AnaTA1 &english,const std::vector<class AnaTermStringsMap> &m)
{
    AnaTA1ConstCursor t(&ta);
    QJsonArray a;
    save(a,terms,t.firstChildCursor(),english,m);
    return saveDocument(file,QJsonDocument(a));
}
bool AnqJson::save(const QString &file,const class AnaTermTree &terms,const class AnaTA2 &ta,const std::vector<class AnaTermStringsMap> &m)
{
    AnaTA2ConstCursor t(&ta);
    QJsonArray a;
    save(a,terms,t.firstChildCursor(),m);
    return saveDocument(file,QJsonDocument(a));
}
bool AnqJson::save(const QString &file,const class AnaTermTree &tt,const std::vector<AnaTermStringsMap> &m)
{
    AnaTermConstCursor t(&tt);
    QJsonArray a;
    save(a,t.firstChildCursor(),m);
    return saveDocument(file,QJsonDocument(a));
}
bool AnqJson::save(const QString &file,const class AnaTermStringsMap &map)
{
    QJsonObject o;
    o["lang"]=QString::fromStdString(map.language());
    QJsonArray a;
    for (auto &m:map)
    {
        QJsonObject t;
        t["st"]=QString::fromStdString(std::string(Ana::trStatusCode(m.second.status())));
        t["id"]=m.first.toInt();
        t["tr"]=QString::fromStdString(m.second.term());
        t["sy"]=toArray(m.second.synonyms());
        a.push_back(std::move(t));
    }
    o["terms"]=std::move(a);
    return saveDocument(file,QJsonDocument(o));
}
