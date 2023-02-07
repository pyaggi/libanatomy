#ifndef TRAUTO_H
#define TRAUTO_H

#include <trfile.h>
#include <QDialog>
#define DEFAULT_MAX 4900
class TrAuto : public QDialog
{
    Q_OBJECT
public:
    explicit TrAuto(QWidget *parent = nullptr);

    void setStrings(const QStringList &);
    void setFile(class TrFile *);
signals:
private slots:
    void autoTranslated();
    void translated();
private:
    std::pair<int,int> pageRange(int idx) const;
    int pageSize(int idx) const;
    void updateStrings(TrStatus);
    void scanFile(bool);
    void loadPage();
    void savePage();
    void selectPage(int);
    void updateFrags();
    void updateFragsCond();
    class QLineEdit *m_chars;
    class QSpinBox *m_page;
    QStringList m_list;
    std::vector<int> m_frags;
    std::vector<QStringList> m_outList;
    bool m_ign=false;
    class QTextEdit *m_in,*m_out;
    int m_curpage=-1;
    int m_curmax=DEFAULT_MAX;
    TrFile *m_file=nullptr;
    class QPushButton *m_alt;
};

#endif // TRAUTO_H
