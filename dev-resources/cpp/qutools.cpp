#ifndef QUTOOLS_CPP
#define QUTOOLS_CPP
#include <qutools.h>
#include <ctype.h>
namespace Qu
{
bool isSeparator(uint8_t t)
{
    return std::isblank(t) || t==0x0d || t==0x0a;
}
void leftTrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](uint8_t ch) {
        return !Qu::isSeparator(ch);
    }));
}

void rightTrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](uint8_t ch) {
        return !Qu::isSeparator(ch);
    }).base(), s.end());
}
void trim(std::string &s)
{
    leftTrim(s);
    rightTrim(s);
}
void justifyLeft(std::string &s,int width,std::string::value_type c)
{
    width-=static_cast<int>(s.size());
    if (width>0)
        s.append(width,c);
}
void justifyRight(std::string &s,int width,std::string::value_type c)
{
    width-=static_cast<int>(s.size());
    if (width>0)
        s.insert(0,width,c);
}
}
#endif // QUTOOLS_CPP
