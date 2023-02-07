#ifndef QUTOOLS_H
#define QUTOOLS_H
#include <vector>
#include <algorithm>
#include <string>
namespace Qu
{
template <typename VT,typename IT,typename V>
std::vector<VT> split(IT begin,IT end, const V & v,bool keepEmpty)
{
    std::vector<VT> ret;
    if (begin==end)
        return ret;
    IT found=std::find(begin,end,v);
    do
    {
        if (keepEmpty || found!=begin)
        {
            ret.push_back(VT(begin,found));
            begin=found;
        }
        if (begin!=end)
            begin++;
        found=std::find(begin,end,v);
    }while (begin!=end);
    return ret;
}
template <typename V,typename S>
std::vector<V> split(const V& str,const S & v,bool keepEmpty=false)
{
    return split<V>(str.begin(),str.end(),v,keepEmpty);
}

void leftTrim(std::string &s) ;
void rightTrim(std::string &s) ;
void trim(std::string &s) ;
bool isSeparator(uint8_t t);
void justifyLeft(std::string &s,int width,std::string::value_type c=' ');
void justifyRight(std::string &s,int width,std::string::value_type c=' ');

}
#endif // QUTOOLS_H
