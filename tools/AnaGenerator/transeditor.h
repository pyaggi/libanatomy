#ifndef TRANSEDITOR_H
#define TRANSEDITOR_H

#include <QWidget>
#include <anqtreewidget.h>
#include <anatermstrings.h>
class TransEditor : public AnqTreeWidget
{
    Q_OBJECT
public:
    explicit TransEditor(QWidget *parent = nullptr);

    void setup(const class AnaTermTree & ,const class AnaTermStringsMap &,const QString &csv);
    const AnaTermStringsMap& translations() const {return m_tr;}
    bool csvSave(const QString &loc,bool data);
    bool jsonSave(const QString &loc);

signals:
private:
    class AnqTreeWidget *m_tree;
    QString m_csv;
    AnaTermStringsMap m_tr;
};

#endif // TRANSEDITOR_H
