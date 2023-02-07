#ifndef ANAJSON_H
#define ANAJSON_H
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <anatomyqt.h>
#include "anata1.h"
#include "anata2.h"
#include "anaterm.h"
#include "anatermstrings.h"

class ANAQT_PUBLIC  AnqJson
{
public:
    AnqJson();

    QJsonDocument loadDocument(const QString &file);
    QJsonArray loadArray(const QString &file);
    bool saveDocument(const QString &file,const QJsonDocument &);

    int loadTa1(const QString &file,class AnaTA1 & );
    int loadTa2(const QString &file,class AnaTA2 & );
    int loadTaMap(const QString &file,std::map<int,QString> & map);
    int loadTerms(const QString &file,class AnaTermTree & ,std::vector<class AnaTermStringsMap> &trs );

    bool save(const QString &file,const class AnaTermTree &,const std::vector<class AnaTermStringsMap> &m={});
    bool save(const QString &file,const class AnaTermStringsMap &);
    bool save(const QString &file,const class AnaTermTree &,const class AnaTA1 &latin,const class AnaTA1 &english ,const std::vector<class AnaTermStringsMap> &m={});
    bool save(const QString &file,const class AnaTermTree &,const class AnaTA2 &,const std::vector<class AnaTermStringsMap> &m={});
    QString errorString() const {return m_error;}

    static QStringList toQStringList(const QJsonArray& a);
    static std::vector<std::string> toStringList(const QJsonArray& a);
    static QJsonArray toArray(const std::vector<std::string>&);
    static QString toString(const QJsonValue &);
private:
    void save(QJsonArray &, AnaTermConstCursor, const std::vector<AnaTermStringsMap> &m={});
    void save(QJsonArray &, const class AnaTermTree &, AnaTA2ConstCursor, const std::vector<AnaTermStringsMap> &m={});
    void save(QJsonArray &, const class AnaTermTree &, AnaTA1ConstCursor, const class AnaTA1 &english, const std::vector<AnaTermStringsMap> &m={});
    int loadTa2(const QJsonArray &,AnaTA2Cursor  );
    int loadTa1(const QJsonArray &,AnaTA1Cursor  );
    int loadTerms(const QJsonArray &,AnaTermCursor, std::vector<class AnaTermStringsMap> &trs );
    QString m_error;

};

#endif // ANAJSON_H
