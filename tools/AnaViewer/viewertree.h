#ifndef VIEWERTREE_H
#define VIEWERTREE_H

#include <anqtreewidget.h>

class ViewerTree : public AnqTreeWidget
{
    Q_OBJECT
public:
    enum Mode {TA1,TA2,Terms,Generic,ZAnatomy};
    explicit ViewerTree(Mode m,QWidget *parent = nullptr);
    void load(const QJsonArray &r) {return load(r,nullptr);}
    void setMode(Mode m);
    const QString &nameKey() const {return m_nameKey;}
signals:
private slots:
    void load(const QJsonArray &r,class QTreeWidgetItem *);
    void setLang(int);
    int lIndex(const QString &);
private:
    Mode m_mode;
    QString explain(const QString &suffix);
    QString m_nameKey;
};

#endif // VIEWERTREE_H
