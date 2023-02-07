#ifndef ANATERMSTREE_H
#define ANATERMSTREE_H
#include <anatomyqt.h>
#include <QWidget>
#include <anaterm.h>
#include <anqtreewidget.h>
class QTreeWidget;
class QTreeWidgetItem;
class ANAQT_PUBLIC AnqTermsTree : public AnqTreeWidget
{
    Q_OBJECT
public:
    explicit AnqTermsTree(QWidget *parent = nullptr);
    void loadTerms(const class AnaTermTree &);
public slots:
private:
    void selLang(int);
signals:
private:
    void loadTerms(AnaTermConstCursor ,QTreeWidgetItem *);
};


#endif // ANATERMSTREE_H
