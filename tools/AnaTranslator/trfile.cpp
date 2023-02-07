#include "trfile.h"
#include <QCoreApplication>
#include <QLocale>
TrFile::TrFile()
{

}
void TrFile::createLanguage(const QString &lang)
{
    m_language=lang;
    auto dash=m_file.lastIndexOf('-');
    auto dot=m_file.lastIndexOf('.');
    if (dash>0 && dot>dash)
    {
        QString nf=m_file.mid(0,dash+1);
        nf+=lang;
        nf+=m_file.midRef(dot);
        m_file=nf;
    }
    else if (dot>0)
    {
        QString nf=m_file.mid(0,dot);
        nf+="-";
        nf+=lang;
        nf+=m_file.midRef(dot);
        m_file=nf;
    }
    else
        m_file+="-"+lang;
    clearTranslations();
}

QStringList TrFile::plurals(int idx) const
{
    QStringList ret;
    auto max=pluralCount(idx);
    for (int c=0;c<max;c++)
        ret.push_back(translation(idx,c+1));
    return ret;
}

QString TrFile::languageName() const
{
    if (m_language.isEmpty())
        return {};
    QLocale l(m_language);
    auto ret=l.languageToString(l.language());
    if (m_language.contains('_'))
    {
        ret+=" (";
        ret+=l.countryToString(l.country());
        ret+=")";
    }
    return ret;
}
QString TrFile::extractLanguage(const QString &s)
{
    auto dash=s.lastIndexOf('-');
    auto dot=s.lastIndexOf('.');
    if (dash>0 && dot>dash)
        return s.mid(dash+1,dot-(dash+1));
    return {};
}
