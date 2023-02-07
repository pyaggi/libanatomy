#ifndef TRWORKSPACE_H
#define TRWORKSPACE_H

#include <QWidget>

class TrWorkspace : public QWidget
{
    Q_OBJECT
public:
    explicit TrWorkspace(QWidget *parent = nullptr);
    ~TrWorkspace();
    void reloadFile();
    void setFile(class TrFile *);
    TrFile *file() const {return m_file;}
signals:
private slots:
    void edFin();
    void edWork();
    void edLeave();
    void edNext();
    void edNextUn();
private:
    QString updateCurrent();
    void act(int);
    class TrFile *m_file=nullptr;
    class TrList *m_list;
    class QLineEdit *m_language,*m_context,*m_filePath;
    class QTextBrowser *m_source;
    class TrEdit *m_tr;
    bool m_qmode=false;
    int m_curidx=-1;
};

#endif // TRWORKSPACE_H
