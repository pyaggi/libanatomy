#ifndef ANATERMSTRINGS_H
#define ANATERMSTRINGS_H
#include <anatermid.h>
#include <vector>
#include <map>
class ANACPP_PUBLIC AnaTermStrings
{
public:
    AnaTermStrings();

    void setTerm(const std::string &s) {m_term=s;}
    const std::string &term() const {return m_term;}

    void addSynonym(const std::string &r) {m_synonyms.push_back(r);}
    void setSynonyms(const std::vector<std::string> &v) {m_synonyms=v;}
    const std::vector<std::string>& synonyms() const {return m_synonyms;}

    void setStatus(Ana::TrStatus st) {m_st=st;}
    Ana::TrStatus status() const {return m_st;}

private:
    std::string m_term;
    std::vector<std::string> m_synonyms;
    Ana::TrStatus m_st=Ana::TrStatus::New;
};

class ANACPP_PUBLIC AnaTermStringsMap:public std::map<AnaTermId,AnaTermStrings>
{
public:
    AnaTermStringsMap(){}
    AnaTermStringsMap(const std::string &lang):m_language(lang){}

    void setLanguage(const std::string &s) {m_language=s;}
    const std::string& language() const {return m_language;}
private:
    std::string m_language;
};

class ANACPP_PUBLIC AnaTermTranslations:public std::vector<AnaTermStringsMap>
{
public:
    AnaTermStringsMap &addLanguage(const std::string &s) ;
    AnaTermStringsMap &language(const std::string &s) {return addLanguage(s);}
    const AnaTermStringsMap &language(const std::string &) const;
};

#endif // ANATERMSTRINGS_H
