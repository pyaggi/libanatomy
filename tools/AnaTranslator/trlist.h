#ifndef WORDLIST_H
#define WORDLIST_H

#include <QWidget>
#include <trfile.h>
class TrList : public QWidget
{
    Q_OBJECT
public:

    explicit TrList(QWidget *parent = nullptr);

    void addWord(TrStatus,const QString &);
    void setStatus(int idx,TrStatus);
    TrStatus status(int idx) const;

    static QIcon icon(TrStatus);

    void clear();
    void next(bool goToNextUnfinished=true);
    int current() const {return m_current;}
signals:
    void activated(int);
private slots:
    void act(class QListWidgetItem*);
private:
    class QListWidget *m_list;
    int m_current=-1;
};

#endif // WORDLIST_H
