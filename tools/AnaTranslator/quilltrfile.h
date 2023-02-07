#ifndef QUILLTRFILE_H
#define QUILLTRFILE_H
#include <trfile.h>


class QuillTrFile:public TrFile
{
public:
    virtual QString load(const QString &) override;
    virtual int count() const override {return m_csv.table().size();}
    virtual TrStatus status(int) const override;
    virtual QString source(int) const override;
    virtual QString context(int) const override;
    virtual QString translation(int,int plural=0) const override;
    virtual bool needPlural(int) const override;
    virtual int pluralCount(int) const override ;
    virtual void setTranslation(int,const QString &,int plural=0) override;
    virtual void setStatus(int,TrStatus) override;
    virtual QString save(const QString &s) override;
    virtual QString fileFilter()const override {return "Quill Translation Sources (*.qts)";}
    virtual void clearTranslations() override;
private:
    AnqCSV m_csv;
};

#endif // QUILLTRFILE_H
