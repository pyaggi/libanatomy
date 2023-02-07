#ifndef ANATRFILE_H
#define ANATRFILE_H
#include <trfile.h>
#include <map>
class AnaTrFile:public TrFile
{
public:
    virtual QString load(const QString &) override;
    virtual int count() const override {return m_csv.table().size();}
    virtual TrStatus status(int) const override;
    virtual QString source(int) const override;
    virtual QString alternativeSource(int) const override;
    virtual QString translation(int,int plural=0) const override;
    virtual QStringList synonyms(int) const override;
    virtual void setTranslation(int,const QString &,int plural=0) override;
    virtual void setSynonyms(int,const QStringList &) override;
    virtual void setStatus(int,TrStatus) override;
    virtual QString save(const QString &s) override;
    virtual QString fileFilter()const override {return "LibAnatomy Translation Files (*.csv)";}
    virtual void clearTranslations() override;
    virtual QString context(int) const override;
    virtual QString alternativeContext(int) const override;
private:
    AnqCSV m_csv;
    std::map<int,int> m_id_idx;
};
#endif // ANATRFILE_H
