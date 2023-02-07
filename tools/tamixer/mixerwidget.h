#ifndef MIXERWIDGET_H
#define MIXERWIDGET_H

#include <QWidget>
#include <anata1.h>
#include <anata2.h>

class MixerWidget : public QWidget
{
    Q_OBJECT
public:
    enum Lang {Lat=0,Uk,Us};

    explicit MixerWidget(QWidget *parent = nullptr);

    void loadTa1(class AnaTA1 &);
    void loadTa2(class AnaTA2 &);
    void setMixFile(const QString &s);

    void unmatchTa1(const QString &);
    void unmatchTa2(int);

    void match(const QString &ta1,int ta2,bool hl=true);

    void setLanguage(Lang l);
    void highlightTa1(const QString &ta1);

public slots:
    void matchSelected();
    void unmatchSelected();
    void clearMatches();
    void save();
    void reset();
    void autoMatch(int);
    void clearHighlight();
signals:
private slots:
    void langCmb(int);
private:

    void updateInfo();
    void reportMatched(int);
    void autoOneLevel();
    void autoOneLevelOnMatched();
    void autoRecLevel();
    void autoRecOnMatched();
    void autoAll();
    void autoLeafLevel();
    void autoLeafOnMatched();
    void autoLeaf();
    void autoFindLeft();
    void autoFindRight();
    void autoParentFiltered();
    void autoPfOnMatched();
    void autoPfRecOnMatched();
    void autoPfFindLeft();
    void autoPfFindRight();

    void pfFirstMatch(std::vector<class QTreeWidgetItem *> &list1,std::vector<QTreeWidgetItem *> &list2);
    static QString removeShorts(const QString  &s);
    QString parentFiltered(const QString &s,class QTreeWidgetItem *parent);
    void autoMatchItems(std::vector<class QTreeWidgetItem *> &list1,std::vector<QTreeWidgetItem *> &list2);
    void ta1Activated(class QTreeWidgetItem *item, int column);
    void ta2Activated(class QTreeWidgetItem *item, int column);
    bool matches(class QTreeWidgetItem *ta1,class QTreeWidgetItem *ta2,bool tryParentFiltered=true);
    class QTreeWidgetItem *findTa1(const QString &);
    class QTreeWidgetItem *findTa2(int id);
//    void autoMatchItems(QTreeWidgetItem *ta1,QTreeWidgetItem *ta2);
    void setModified(bool t=true);
    void clearMatches(class QTreeWidgetItem *);
    void updateLang(class QTreeWidgetItem *);
    void updateMatches();
    bool matchItems(class QTreeWidgetItem *ta1,class QTreeWidgetItem *ta2,bool warn,bool hl=true);
    void loadTa1(AnaTA1Cursor ,class QTreeWidgetItem *);
    void loadTa2(AnaTA2Cursor ,class QTreeWidgetItem *);
    class MixerTreeWidget *m_ta1,*m_ta2;
    QString m_mixFile;
    std::map<QString,int> m_match;
    Lang m_l=Lat;
    class QComboBox *m_langCmb;
    class QPushButton *m_save,*m_reset;
    class QLabel *m_report;
};

#endif // MIXERWIDGET_H
