#ifndef ANATA2TREE_H
#define ANATA2TREE_H
#include <anatomyqt.h>
#include <anata2.h>
#include <anqtreewidget.h>
class QTreeWidget;
class QTreeWidgetItem;
class ANAQT_PUBLIC AnqTa2Tree : public AnqTreeWidget
{
    Q_OBJECT
public:
    explicit AnqTa2Tree(QWidget *parent = nullptr);
    void loadTerms(const class AnaTA2 &);
public slots:
    void search(const QString &);
private:
    void selLang(int);
signals:
private:
    void loadTerms(AnaTA2ConstCursor ,QTreeWidgetItem *);
};


#endif // ANATA2TREE_H
