#ifndef FILEWIDGET_H
#define FILEWIDGET_H
#include <anatomyqt.h>
#include <QWidget>

class ANAQT_PUBLIC AnqFileWidget:public QWidget
{
    Q_OBJECT
public:
    AnqFileWidget();
    void setTitle(const QString &);
    void setError(const QString &);
    void setInfo(const QString &);
    void setSetting(const QString &s);
    void setSaveMode(bool t) {m_save=t;}
    void setDirectoryMode(bool t) {m_dir=t;}
    const QString& selectedFile() const {return m_cur;}
public slots:
    void selectFile();
    void selectFile(const QString &);
signals:
    void fileSelected(const QString &);
private slots:
    void lefin();
private:
    class QLineEdit *m_line;
    class QPushButton *m_but;
    class AnqInfoLabel *m_label;
    QString m_setting,m_cur;
    bool m_ign=false;
    bool m_save=false;
    bool m_dir=false;
};

#endif // FILEWIDGET_H
