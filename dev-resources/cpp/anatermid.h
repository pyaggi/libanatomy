#ifndef ANATERMID_H
#define ANATERMID_H
#include <string>
#include <anatomy.h>
class ANACPP_PUBLIC AnaTermId
{
public:
    AnaTermId();

    bool isNull() const {return m_id==0;}
    bool isValid() const;
    std::string toString() const;
    int toInt() const {return m_id;}
    void setIdValue(int i) {m_id=i;}

    AnaTermId &operator+=(int) ;
    AnaTermId operator+(int) ;

    bool operator<(const AnaTermId &b) const { return m_id/10<b.m_id/10;}
    bool operator==(const AnaTermId &b) const { return m_id==b.m_id;}

    static int addId(int curId,int add);

    void setFromInteger(int);
    void setFromTa2(int ta2Number) {setFromInteger(ta2Number+10000);}
    static AnaTermId fromInteger(int v) ;
    static AnaTermId fromTa2(int ta2Number) ;
    static AnaTermId fromTa1Index(int ta1Index) ;
    static AnaTermId fromSeqId(int defIndex) ;

    int ta2Number() const;
    int ta1Index() const;
private:
    int m_id=0;
};

#endif // ANATERMID_H
