#include "anatermstrings.h"

AnaTermStrings::AnaTermStrings()
{

}
AnaTermStringsMap &AnaTermTranslations::addLanguage(const std::string &l)
{
    for (auto &t:*this)
        if (t.language()==l)
            return t;
    push_back(AnaTermStringsMap(l));
    return back();
}
const AnaTermStringsMap &AnaTermTranslations::language(const std::string &l) const
{
    for (auto &t:*this)
        if (t.language()==l)
            return t;
    static const AnaTermStringsMap n;
    return n;
}
