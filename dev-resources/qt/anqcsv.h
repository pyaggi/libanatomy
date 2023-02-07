#ifndef ANACSV_H
#define ANACSV_H
#include <anatomyqt.h>
#include <QStringList>

class ANAQT_PUBLIC AnqCSV
{
public:
    AnqCSV();
    QString save(const QString &) const;
    QString load(const QString &,bool fillGaps=true);
    void setHeader(const QStringList& s) {m_header=s;}
    void addRow(const QStringList &r) {m_table.push_back(r);}
    void clear() {m_table.clear();}

    int columnCount() const {return m_columnCount;}

    const QStringList& header() const {return m_header;}
    const std::vector<QStringList> &table() const {return m_table;}
    void setCell(int row,int col,const QString &,bool fillGaps=true);
    bool clearCell(int row,int col);
    void setRowSize(int row,int size);
private:
    static QString write(class QFile *,const QStringList &);
    static QString encode(const QString &);
    QStringList m_header;
    std::vector<QStringList> m_table;
    int m_columnCount=-1;
};

#endif // ANACSV_H
