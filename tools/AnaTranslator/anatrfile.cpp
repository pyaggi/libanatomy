#include "anatrfile.h"
#include <QCoreApplication>
QString AnaTrFile::load(const QString &s)
{
    m_id_idx.clear();
    setLanguage(extractLanguage(s));
    auto r=m_csv.load(s);
    if (!r.isEmpty())
        return r;
    if (!m_csv.table().size())
        return QCoreApplication::tr(QT_TRANSLATE_NOOP("TrFile","Empty translation file"));
    bool ok=m_csv.table().size()>=5 && m_csv.table().front().front().size()==1;
    if (!ok)
        return QCoreApplication::tr(QT_TRANSLATE_NOOP("TrFile","Not a Libanatomy translation source"));
    setFile(s);
    int idx=0;
    for (auto &r:m_csv.table())
    {
        m_id_idx[r[1].toInt()]=idx;
        idx++;
    }
    return {};
}
QString AnaTrFile::save(const QString &s)
{
    if (!m_csv.table().size())
        return QCoreApplication::tr(QT_TRANSLATE_NOOP("TrFile","Empty translation file"));
    auto ret=m_csv.save(s);
    if (ret.isEmpty())
        setFile(s);
    return ret;
}
void AnaTrFile::setStatus(int idx,TrStatus st)
{
    switch(st)
    {
    case TrStatus::AutoTr:
        m_csv.setCell(idx,0,"A");
        break;
    case TrStatus::NewTr:
        m_csv.setCell(idx,0,"N");
        break;
    case TrStatus::Obsolete:
        m_csv.setCell(idx,0,"O");
        break;
    case TrStatus::Finished:
        m_csv.setCell(idx,0,"T");
        break;
    case TrStatus::WorkInProgress:
        m_csv.setCell(idx,0,"W");
        break;
    }
}

TrStatus AnaTrFile::status(int idx) const
{
    auto st=m_csv.table()[idx][0][0];
    switch (st.toLatin1())
    {
    case 'A':
        return TrStatus::AutoTr;
    case 'N':
        return TrStatus::NewTr;
    case 'O':
        return TrStatus::Obsolete;
    case 'T':
        return TrStatus::Finished;
    }
    return TrStatus::WorkInProgress;
}
QString AnaTrFile::context(int idx) const
{
    QString ret;
    auto id=m_csv.table()[idx][1];
    auto parent=m_csv.table()[idx][2];
    while (!parent.isEmpty())
    {
        auto f=m_id_idx.find(parent.toInt());
        if (f==m_id_idx.end())
            break;
        QString s=source(f->second);
        s+=" (";
        s+=parent;
        s+=')';
        if (ret.size())
            s+=" / ";
        ret=s+ret;
        parent=m_csv.table()[f->second][2];
    }
    if (ret.size())
        ret+=" / ";
    ret+=id;
    return ret;
}
QString AnaTrFile::alternativeContext(int idx) const
{
    QString ret;
    auto id=m_csv.table()[idx][1];
    auto parent=m_csv.table()[idx][2];
    while (!parent.isEmpty())
    {
        auto f=m_id_idx.find(parent.toInt());
        if (f==m_id_idx.end())
            break;
        QString s=alternativeSource(f->second);
        s+=" (";
        s+=parent;
        s+=')';
        if (ret.size())
            s+=" / ";
        ret=s+ret;
        parent=m_csv.table()[f->second][2];
    }
    if (ret.size())
        ret+=" / ";
    ret+=id;
    return ret;
}
QString AnaTrFile::source(int idx) const
{
    return m_csv.table()[idx][3];
}
QString AnaTrFile::alternativeSource(int idx) const
{
    return m_csv.table()[idx][4];
}
QString AnaTrFile::translation(int idx,int) const
{
    if (m_csv.table()[idx].size()<=5)
        return {};
    return m_csv.table()[idx][5];
}
QStringList AnaTrFile::synonyms(int idx) const
{
    if (m_csv.table()[idx].size()<=6)
        return {};
    auto ret=m_csv.table()[idx][6].split(";");
    for (auto &r:ret)
        r=r.trimmed();
    for (auto it=ret.begin();it!=ret.end();)
    {
        if (it->isEmpty())
            it=ret.erase(it);
        else
            ++it;
    }
    return ret;
}
void AnaTrFile::setTranslation(int idx,const QString &s,int )
{
    m_csv.setCell(idx,5,s);
}
void AnaTrFile::setSynonyms(int idx,const QStringList &l)
{
    m_csv.setCell(idx,6,l.join("; "));
}
void AnaTrFile::clearTranslations()
{
    int max=m_csv.table().size();
    for (int c=0;c<max;c++)
    {
        m_csv.setCell(c,0,"N");
        m_csv.clearCell(c,5);
        m_csv.clearCell(c,6);
    }
}
