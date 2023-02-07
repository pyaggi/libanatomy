#ifndef ANATERM_H
#define ANATERM_H
#include <anatomy.h>
#include <anatermid.h>
#include <quuidtree.h>
#include <map>
#include <memory>
class ANACPP_PUBLIC AnaTerm
{
public:
    AnaTerm();

    void setId(const AnaTermId &id) {m_id=id;}
    const AnaTermId &id() const {return m_id;}

    void setParentId(const AnaTermId &id) {m_pid=id;}
    const AnaTermId &parentId() const {return m_pid;}

    void setTa1Code(const std::string &s) {m_ta1=s;}
    const std::string ta1Code() const {return m_ta1;}

    void setLatin(const std::string &s) {m_latin=s;}
    const std::string &latin() const {return m_latin;}

    void setEnglish(const std::string &s) {m_english=s;}
    const std::string &english() const {return m_english;}

    void setSex(Ana::Sex s) {m_sex=s;}
    Ana::Sex sex() const {return m_sex;}

    void setType(Ana::TermType t) {m_type=t;}
    Ana::TermType type() const {return m_type;}

    void setTa2Inconsistent(bool t) {m_ta2Inc=t;}
    bool ta2Inconsistent() const {return m_ta2Inc;}

private:
    AnaTermId m_id,m_pid;
    std::string m_ta1,m_latin,m_english;
    Ana::Sex m_sex=Ana::Sex::Unknown;
    Ana::TermType m_type=Ana::TermType::Unknown;
    bool m_ta2Inc=false;
};

typedef QuUidTreeCursor<AnaTerm,AnaTermId> AnaTermCursor ;
typedef QuUidTreeConstCursor<AnaTerm,AnaTermId> AnaTermConstCursor ;

class ANACPP_PUBLIC AnaTermTree:public QuUidTree<AnaTerm,AnaTermId>
{
public:
    AnaTermCursor findTA1(const std::string &);
    AnaTermConstCursor findTA1(const std::string &) const;
};


#endif // ANATERM_H
