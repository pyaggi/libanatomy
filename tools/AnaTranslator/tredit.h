#ifndef TREDIT_H
#define TREDIT_H

#include <QWidget>

class TrEdit : public QWidget
{
    Q_OBJECT
public:
    explicit TrEdit(QWidget *parent = nullptr);

    void setTranslation(const QString &);
    void setOther(const QStringList &);

    void setOtherTitle(const QString &);

    QString translation() const;
    QStringList other() const;


signals:
    void finished();
    void leave();

private slots:
    void leaveAct();
    void finAct();

private:
    void ch(int);
    void add();
    void remove();
    std::vector<class TrTextEdit *>m_tr;
};

#endif // TREDIT_H
