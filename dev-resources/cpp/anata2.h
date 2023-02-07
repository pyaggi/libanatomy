#ifndef ANATA2_H
#define ANATA2_H
#include <string>
#include <quuidtree.h>
#include <anatomy.h>
#include <vector>
class ANACPP_PUBLIC AnaTA2Record
{
public:
    AnaTA2Record():m_id(0){}
    AnaTA2Record(int id,
           const std::string &lat,
           const std::vector<std::string> &lats,
           const std::string &us,
           const std::string &uk,
           const std::vector<std::string> &syn,
           const std::vector<std::string> &other,
                 Ana::Sex sex,
                 Ana::RecordType rtype,
                 bool inconsistent
                 );

    void setId(int id) {m_id=id;}
    void setLatin(const std::string &s) {m_lat=s;}
    void setLatinSyn(const std::vector<std::string> &s) {m_lats=s;}
    void setLatinSyn(const std::string &s);
    void setUs(const std::string &s) {m_us=s;}
    void setGb(const std::string &s) {m_gb=s;}
    void setSyn(const std::vector<std::string> &s) {m_syn=s;}
    void setSyn(const std::string &s) ;
    void setOther(const std::vector<std::string> &s) {m_other=s;}
    void setOther(const std::string &s);
    void setSex(Ana::Sex s) {m_sex=s;}
    void setInconsistent(bool t) {m_inc=t;}
    void setType(Ana::RecordType t) {m_rtype=t;}

    const int& id() const {return m_id;}
    const std::string &latin() const {return m_lat;}
    const std::vector<std::string> &latinSyn() const {return m_lats;}
    const std::string &us() const {return m_us;}
    const std::string &gb() const {return m_gb;}
    const std::vector<std::string> &syn() const {return m_syn;}
    const std::vector<std::string> &other() const {return m_other;}
    Ana::Sex sex() const {return m_sex;}
    bool inconsistent() const {return m_inc;}
    Ana::RecordType type() const {return m_rtype;}
private:
    int m_id;
    std::string m_lat,m_gb,m_us;
    std::vector<std::string> m_lats,m_syn,m_other;
    Ana::Sex m_sex=Ana::Sex::Both;
    Ana::RecordType m_rtype=Ana::RecordType::Heading;
    bool m_inc=false;
};

class ANACPP_PUBLIC AnaTA2:public QuUidTree<AnaTA2Record,int>
{

};
typedef QuUidTreeCursor<AnaTA2Record,int> AnaTA2Cursor ;
typedef QuUidTreeConstCursor<AnaTA2Record,int> AnaTA2ConstCursor ;

#endif // ANATA2_H
