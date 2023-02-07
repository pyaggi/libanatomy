#ifndef TREEWITHCONTROLSWIDGET_H
#define TREEWITHCONTROLSWIDGET_H
#include <anatomyqt.h>
#include <QWidget>

class ANAQT_PUBLIC AnqTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnqTreeWidget(QWidget *parent=nullptr );

    static void collectLeaf(class QTreeWidgetItem *from,std::vector<class QTreeWidgetItem *>&,bool includeFrom=true);
    void collectLeaf(std::vector<class QTreeWidgetItem *>&);
    static void collectLeaf(int checkUserRole1,class QTreeWidgetItem *from,std::vector<class QTreeWidgetItem *>&,bool includeFrom=true);
    void collectLeaf(int checkUserRole1,std::vector<class QTreeWidgetItem *>&);
    static void collect(class QTreeWidgetItem *from,std::vector<class QTreeWidgetItem *>&,bool includeFrom=true);
    void collect(std::vector<class QTreeWidgetItem *>&);
    static void collectChildren(class QTreeWidgetItem *from,std::vector<class QTreeWidgetItem *>&,bool includeFrom=true);

    class QTreeWidget *tree() {return m_tree;}

    void showSelected(class QTreeWidgetItem *);
    void expand(class QTreeWidgetItem*);
    void clear();
    void addControlWidget(QWidget *);
    QWidget *controlWidget(int);

    void addStandardControls();

public slots:
    void search(const QString &);

    void expandVisible();
    void collapseVisible();
    void clearHighlight(int col=0);
    void clearHighlight(class QTreeWidgetItem*,int col=0);
    void highlight(class QTreeWidgetItem*,int col=0);
    void generateStats(int colin,int colout);
signals:

private:
    struct Stats
    {
        int t=0,w=0,l=0;
    };
    Stats genStats(class QTreeWidgetItem*,int colin,int colout);
    class QTreeWidget *m_tree;
};

#endif // TREEWITHCONTROLSWIDGET_H
