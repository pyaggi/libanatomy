#ifndef TERMSEDITOR_H
#define TERMSEDITOR_H

#include <QWidget>
#include <anaterm.h>
#include <anatermstrings.h>
#include <anata1.h>
#include <anata2.h>

class TermsEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TermsEditor(QWidget *parent = nullptr);
    bool open(const QString &s,AnaTermTranslations &);
    const AnaTermTree& terms() const {return m_terms;}
    bool csvFullSave(const QString &loc,const AnaTermTranslations &);
    bool jsonFullSave(const QString &loc,const AnaTermTranslations &);
    bool csvSave(const QString &loc);
    bool jsonSave(const QString &loc);
    const AnaTA1 &ta1La() const {return m_ta1Lat;}
    const AnaTA1 &ta1En() const {return m_ta1En;}
    const AnaTA2 &ta2() const {return m_ta2;}

signals:
private:
    bool mergeZAnatomyTranslations(const class AnqCSV &trans,AnaTermTranslations &,const AnaTA1 &ta1Lat,const AnaTA1 &ta1En,const AnaTA1 &ta1Es,const std::map<std::string,int> &tarmap);
    static QStringList basicCsvHeaders();
    static QStringList termToCsv(AnaTermTree::Node *);
    static QString formatString(const std::string &);
    static std::vector<std::string> splitFormat(const QString &);
    static QString formatString(const QString &s) {return formatString(s.toStdString());}
    static std::vector<std::string> formatStrings(const std::vector<std::string> &);
    void parseTa1(AnaTA1Cursor,AnaTermCursor term,const AnaTA1 &taEs,class AnaTermStringsMap &enUs,class AnaTermStringsMap &lat,class AnaTermStringsMap &es);
    void parseTa2(AnaTA2Cursor,AnaTermCursor,class AnaTermStringsMap &enUs,class AnaTermStringsMap &enUk,class AnaTermStringsMap &lat);
    QString parseDefs(const class AnqCSV &defs,int row,class AnaTermStringsMap &enUs,class AnaTermStringsMap &lat);
    AnaTermTree m_terms;
    class AnqTermsTree *m_tree;
    AnaTA1 m_ta1Lat,m_ta1En;
    AnaTA2 m_ta2;
};

#endif // TERMSEDITOR_H
