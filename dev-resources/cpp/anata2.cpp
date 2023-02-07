#include "anata2.h"
#include "qutools.h"
AnaTA2Record::AnaTA2Record(int id,
                   const std::string &lat,
                   const std::vector<std::string> &lats,
                   const std::string &us,
                   const std::string &uk,
                   const std::vector<std::string> &syn,
                   const std::vector<std::string> &other,
                   Ana::Sex sex,
                   Ana::RecordType rt,
                   bool inconsistent
                   )
    :m_id(id),m_lat(lat),m_us(us),m_gb(uk),m_sex(sex),m_rtype(rt),m_inc(inconsistent)
{
    setLatinSyn(lats);
    setSyn(syn);
    setOther(other);
}
void AnaTA2Record::setLatinSyn(const std::string &s)
{
    m_lats=Qu::split(s,';');
    for (auto &i:m_lats)
        Qu::trim(i);
}
void AnaTA2Record::setSyn(const std::string &s)
{
    m_syn=Qu::split(s,';');
    for (auto &i:m_lats)
        Qu::trim(i);
}
void AnaTA2Record::setOther(const std::string &s)
{
    m_other=Qu::split(s,';');
    for (auto &i:m_lats)
        Qu::trim(i);
}
