#ifndef ANATA1_H
#define ANATA1_H
#include <string>
#include <quuidtree.h>
#include <vector>
#include <anatomy.h>
class ANACPP_PUBLIC AnaTA1Record
{
public:
    AnaTA1Record(){}
    AnaTA1Record(const std::string &id,const std::string &term)
        :m_id(id),m_term(term){}

    void setId(const std::string &s) {m_id=s;}
    void setTerm(const std::string &s) {m_term=s;}

    const std::string &id() const {return m_id;}
    const std::string &term() const {return m_term;}

private:
    std::string m_id,m_term;

};
class ANACPP_PUBLIC AnaTA1Id
{
public:
    AnaTA1Id();
    AnaTA1Id(int l0,int l1,int l2,int l3);
    AnaTA1Id(const std::string &s) {setFromString(s);}

    bool isNull() const {return m_levels[0]==0; }
    bool setFromString(const std::string &);
    std::string toString() const;

    bool prevId() ;
    void nextId() ;


private:
        int m_levels[4]={0};

};

class ANACPP_PUBLIC AnaTA1:public QuUidTree<AnaTA1Record,std::string>
{
public:
    Node *findPreviousId(const std::string &);
    Node *findNextId(const std::string &);
};

typedef QuUidTreeCursor<AnaTA1Record,std::string> AnaTA1Cursor ;
typedef QuUidTreeConstCursor<AnaTA1Record,std::string> AnaTA1ConstCursor ;

#endif // ANATA1_H
