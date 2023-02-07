#include "quilltrfile.h"
#include <QCoreApplication>

QString QuillTrFile::load(const QString &s)
{
    setLanguage(extractLanguage(s));
    auto r=m_csv.load(s,false);
    if (!r.isEmpty())
        return r;
    if (!m_csv.table().size())
        return QCoreApplication::tr(QT_TRANSLATE_NOOP("TrFile","Empty translation file"));
    bool ok=false;
    if (m_csv.table().size()>=3)
        m_csv.table().front().front().toInt(&ok,16);
    if (!ok)
        return QCoreApplication::tr(QT_TRANSLATE_NOOP("TrFile","Not a quill translation file"));
    setFile(s);
    return {};
}
QString QuillTrFile::save(const QString &s)
{
    if (!m_csv.table().size())
        return QCoreApplication::tr(QT_TRANSLATE_NOOP("TrFile","Empty translation file"));
    auto ret=m_csv.save(s);
    if (ret.isEmpty())
        setFile(s);
    return ret;
}

void QuillTrFile::setStatus(int idx,TrStatus st)
{
    auto flags=m_csv.table()[idx][0].toInt(nullptr,16);
    switch(st)
    {
    case TrStatus::Obsolete:
        flags|=4;
        break;
    case TrStatus::Finished:
        flags|=2;
        break;
    case TrStatus::AutoTr:
        flags|=8;
        break;
    case TrStatus::NewTr:
    case TrStatus::WorkInProgress:
        flags&=~(2|4|8);
        break;
    }
    m_csv.setCell(idx,0,QString::number(flags,16));
}
TrStatus QuillTrFile::status(int idx) const
{
    auto flags=m_csv.table()[idx][0].toInt(nullptr,16);
    if (flags & 4)
        return TrStatus::Obsolete;
    if (flags & 2)
        return TrStatus::Finished;
    if (flags & 8)
        return TrStatus::AutoTr;
    if (translation(idx).isEmpty())
        return TrStatus::NewTr;
    return TrStatus::WorkInProgress;
}
QString QuillTrFile::source(int idx) const
{
    return m_csv.table()[idx][2];
}
QString QuillTrFile::context(int idx) const
{
    return m_csv.table()[idx][1];
}
int QuillTrFile::pluralCount(int idx) const
{
    auto rs=m_csv.table()[idx].size();
    auto val=rs-4;
    if (val<0)
        return 0;
    for (int c=4;c<rs;c++)
        if (m_csv.table()[idx][c].size())
            return val;
    return 0;
}
QString QuillTrFile::translation(int idx,int plural) const
{
    if (m_csv.table()[idx].size()<=3+plural)
        return {};
    return m_csv.table()[idx][3+plural];
}
bool QuillTrFile::needPlural(int idx) const
{
    auto flags=m_csv.table()[idx][0].toInt(nullptr,16);
    return flags &1;
}
void QuillTrFile::setTranslation(int idx,const QString &s,int plural)
{
    m_csv.setCell(idx,plural+3,s);
}
void QuillTrFile::clearTranslations()
{
    int max=m_csv.table().size();
    for (int r=0;r<max;r++)
    {
        m_csv.setRowSize(r,3);
        m_csv.setCell(r,0,"00");
    }
}




