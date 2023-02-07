#ifndef TRFILE_H
#define TRFILE_H
#include <QString>
#include <anqcsv.h>

enum class TrStatus {NewTr=0,AutoTr,WorkInProgress,Finished,Obsolete};

class TrFile
{
public:
    TrFile();
    virtual ~TrFile(){}
    virtual int count() const =0;
    virtual QString load(const QString &)=0;
    virtual TrStatus status(int) const=0;
    virtual QString source(int) const=0;
    virtual QString alternativeSource(int) const {return {};}
    virtual QString context(int) const {return {};}
    virtual QString alternativeContext(int) const {return {};}
    virtual QString translation(int,int plural=0) const=0;
    virtual QStringList synonyms(int) const {return {};}
    virtual bool needPlural(int) const {return false;}
    virtual int pluralCount(int) const {return 0;}

    virtual void setTranslation(int,const QString &,int plural=0)=0;
    virtual void setSynonyms(int,const QStringList &) {}
    virtual void setStatus(int,TrStatus)=0;

    QStringList plurals(int idx) const;

    QString language() const {return m_language;}
    void setLanguage(const QString &s) {m_language=s;}
    static QString extractLanguage(const QString &file);

    QString languageName() const;
    const QString &file() const {return m_file;}

    virtual QString save(const QString &s)=0;
    virtual QString fileFilter()const=0;

    virtual void createLanguage(const QString &lang);
    virtual void clearTranslations() =0;
protected:
    void setFile(const QString &s) {m_file=s;}
private:
    QString m_file;
    QString m_language;
};




#endif // TRFILE_H
