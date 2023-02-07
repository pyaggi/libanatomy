#ifndef TERMMATCHER_H
#define TERMMATCHER_H

#include <viewertree.h>

class TermMatcher : public QWidget
{
    Q_OBJECT
public:
    explicit TermMatcher(QWidget *parent = nullptr);

    bool load(const QString &json,ViewerTree::Mode jsonMode,const QString &terms);
public slots:
    void autoMatchIdentical();
    void autoMatchIdenticalOnMatched();
    void abort() {m_abort=true;}
    void exportJson();
    void loadMatches();
    void loadMatches(const QString &file);

private slots:
    void jsonAct(QTreeWidgetItem *,int col);
    void termsAct(QTreeWidgetItem *,int col);

signals:
private:
    QTreeWidgetItem *findTerm(int id);
    void loadMatches(QTreeWidgetItem *,const QJsonArray &);
    void exportItemJson(QTreeWidgetItem *,QJsonArray &);
    void start(const QString &info);
    void end();
    void updateStats();
    void prepareForMatch();
    int matchIdentical(const std::vector<QTreeWidgetItem *>,const std::vector<QTreeWidgetItem *>,bool report=true);
    bool isMatched(QTreeWidgetItem *j);
    QString matched(QTreeWidgetItem *j);

    void match(QTreeWidgetItem *j,QTreeWidgetItem *t);
    ViewerTree *m_json,*m_terms;
    int m_tcol=-1;
    class QLabel *m_barinfo;
    class QProgressBar *m_bar;
    class QWidget *m_bw,*m_pw;
    bool m_abort=false;
    int m_matched=0;
    int m_total=0;
    QString m_file,m_mfile;
};

#endif // TERMMATCHER_H
