#include "anata1.h"
#include <regex>
#include "qutools.h"
AnaTA1Id::AnaTA1Id(int l0,int l1,int l2,int l3)
{
    m_levels[0]=l0;
    m_levels[1]=l1;
    m_levels[2]=l2;
    m_levels[3]=l3;
}
bool AnaTA1Id::setFromString(const std::string &v)
{
    static const std::regex reg("A([0-9]{2,2})\\.([0-9])\\.([0-9]{2,2})\\.([0-9]{3,3})\\");
    std::smatch m;
    if (std::regex_match(v,m,reg))
    {
        for (int c=0;c<4;c++)
            m_levels[c]=atoi(m[c+1].str().data());
        return true;
    }
    m_levels[0]=0;
    return false;
}
std::string AnaTA1Id::toString() const
{
    if (m_levels[0]==0)
        return {};

    std::string ret;
    ret+='A';

    std::string tmp=std::to_string(m_levels[0]);
    Qu::justifyRight(tmp,2,'0');
    ret+=tmp;

    ret+='.';
    ret+=std::to_string(m_levels[1]);

    ret+='.';
    tmp=std::to_string(m_levels[2]);
    Qu::justifyRight(tmp,2,'0');
    ret+=tmp;

    ret+='.';
    tmp=std::to_string(m_levels[3]);
    Qu::justifyRight(tmp,3,'0');
    ret+=tmp;
    return ret;
}
bool AnaTA1Id::prevId()
{
    if (m_levels[0]==0)
        return false;
    if (m_levels[3]==0)
    {
        if (m_levels[2]==0)
        {
            if (m_levels[1]==0)
            {
                if (m_levels[0]<=1)
                    return false;
                m_levels[0]--;
            }
            else
                m_levels[1]--;
        }
        else
            m_levels[2]--;
    }
    else
        m_levels[3]--;
    return true;
}
void AnaTA1Id::nextId()
{
    m_levels[3]++;
}

AnaTA1::Node *AnaTA1::findPreviousId(const std::string &id)
{

    auto f=idMap()->lower_bound(id);
    if (f==idMap()->end() || f==idMap()->begin())
        return nullptr;
    f--;
    return f->second;
}
AnaTA1::Node *AnaTA1::findNextId(const std::string &id)
{
    auto f=idMap()->upper_bound(id);
    if (f==idMap()->end())
        return nullptr;
    return f->second;
}
