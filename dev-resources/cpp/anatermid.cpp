#include "anatermid.h"
#include <anatomy.h>

AnaTermId::AnaTermId()
{

}
bool AnaTermId::isValid() const
{
    if (m_id<100014 || m_id>999999)
        return false;
    return Ana::validateDamm(toString());
}
std::string AnaTermId::toString() const
{
    return std::to_string(m_id);
}

AnaTermId &AnaTermId::operator+=(int a)
{
    m_id=addId(m_id,a);
    return *this;
}
AnaTermId AnaTermId::operator+(int a)
{
    AnaTermId ret;
    ret.m_id=addId(m_id,a);
    return ret;
}
int AnaTermId::addId(int curId,int add)
{
    auto id=curId/10;
    id+=add;
    auto dig=Ana::digitDammInt(std::to_string(id));
    return id*10+dig;
}
void AnaTermId::setFromInteger(int v)
{
    auto dig=Ana::digitDammInt(std::to_string(v));
    m_id=v*10+dig;
}
AnaTermId AnaTermId::fromInteger(int v)
{
    AnaTermId ret;
    ret.setFromInteger(v);
    return ret;
}
AnaTermId AnaTermId::fromTa2(int v)
{
    AnaTermId ret;
    ret.setFromInteger(v+10000);
    return ret;
}
AnaTermId AnaTermId::fromTa1Index(int v)
{
    AnaTermId ret;
    ret.setFromInteger(v+20000);
    return ret;
}
AnaTermId AnaTermId::fromSeqId(int v)
{
    AnaTermId ret;
    ret.setFromInteger(v+40000);
    return ret;
}
int AnaTermId::ta2Number() const
{
    auto id=m_id/10;
    if (id>10000 && id<20000)
        return id-10000;
    return 0;
}
int AnaTermId::ta1Index() const
{
    auto id=m_id/10;
    if (id>20000 && id<30000)
        return id-20000;
    return 0;
}
