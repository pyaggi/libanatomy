#ifndef ANATA1TREE_H
#define ANATA1TREE_H
#include <QWidget>
#include <anatomyqt.h>
#include <anata1.h>
#include <anqtreewidget.h>
class QTreeWidget;
class QTreeWidgetItem;
class ANAQT_PUBLIC  AnqTa1Tree : public AnqTreeWidget
{
    Q_OBJECT
public:
    explicit AnqTa1Tree(QWidget *parent = nullptr);
    void loadTerms(const AnaTA1 &);
public slots:
    void search(const QString &);
private:
    void selLang(int);
signals:
private:
    void loadTerms(AnaTA1ConstCursor ,QTreeWidgetItem *);
};
#endif // ANATA1TREE_H
