#include "anqcsv.h"
#include <QFile>
AnqCSV::AnqCSV()
{

}
QString AnqCSV::write(class QFile *f,const QStringList &l)
{
    QString ret;
    for (auto &i:l)
    {
        if (!ret.isEmpty())
            ret+=',';
        ret+=encode(i);
    }
    ret+="\n";
    if (f->write(ret.toUtf8())<0)
        return f->errorString();
    return {};
}
QString AnqCSV::encode(const QString &l)
{
    QString out;
    out+='"';
    for (auto &i:l)
    {
        switch (i.toLatin1())
        {
        case '\n':
            out+="\\n";
            break;
        case '\t':
            out+="\\t";
            break;
        case '\r':
            out+="\\r";
            break;
        case '\\':
            out+="\\\\";
            break;
        case '"':
            out+="\\\"";
            break;
        default:
            out+=i;
            break;
        }
    }
    out+='"';
    return out;
}
QString AnqCSV::save(const QString &f) const
{
    QFile file(f);
    if (!file.open(QFile::WriteOnly|QFile::Truncate))
        return file.errorString()+"("+f+")";
    QString ret;
    if (!m_header.isEmpty())
    {
        file.write("#");
        ret=write(&file,m_header);
        if (!ret.isEmpty())
            return ret;
    }
    for (auto &r:m_table)
    {
        ret=write(&file,r);
        if (!ret.isEmpty())
            return ret;
    }
    return {};
}
QString AnqCSV::load(const QString &f,bool fillGaps)
{
    m_header.clear();
    m_table.clear();
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return file.errorString()+"("+f+")";
    m_columnCount=0;
    do
    {
        auto line=QString::fromUtf8(file.readLine());
        line=line.trimmed();
        bool header=false;
        if (line.startsWith("#"))
        {
            header=true;
            line=line.mid(1);
        }
        bool inquote=false,inescape=false;
        QStringList tmp;
        tmp.push_back("");
        for (auto &c:line)
        {
            if (inescape)
            {
                switch (c.toLatin1())
                {
                case 't':
                    tmp.back()+="\t";
                    break;
                case '\\':
                    tmp.back()+="\\";
                    break;
                case 'r':
                    tmp.back()+="\r";
                    break;
                case 'n':
                    tmp.back()+="\n";
                    break;
                default:
                    tmp.back()+=c;
                    break;
                }
                inescape=false;
                continue;
            }
            if (c=='"')
            {
                if (inquote)
                    inquote=false;
                else
                    inquote=true;
            }
            else if (c=='\\')
            {
                if (inquote)
                    inescape=true;
                else
                    tmp.back()+=c;
            }
            else if (c==',')
            {
                if (inquote)
                    tmp.back()+=c;
                else
                    tmp.push_back("");
            }
            else
                tmp.back()+=c;
        }
        if (m_columnCount==0)
            m_columnCount=tmp.size();
        else if (m_columnCount<tmp.size())
        {
            if (fillGaps)
            {
                QStringList add;
                while (add.size()<tmp.size())
                    add.push_back({});
                m_header+=add;
                for (auto &t:m_table)
                    t+=add;
            }
            m_columnCount=tmp.size();
        }
        else
        {
            if (fillGaps)
                while (m_columnCount>tmp.size())
                    tmp.push_back({});
        }
        if (header)
            m_header=tmp;
        else
            m_table.push_back(tmp);
    }while (!file.atEnd());
    return {};
}
void AnqCSV::setCell(int row,int col,const QString &s,bool fillGaps)
{
    if (m_columnCount<col+1)
        m_columnCount=col+1;
    if (fillGaps)
        while (m_table[row].size()<m_columnCount)
            m_table[row].push_back({});
    else
        while (m_table[row].size()<=col)
            m_table[row].push_back({});

    m_table[row][col]=s;
}
void AnqCSV::setRowSize(int row,int size)
{
    auto &r=m_table[row];
    while (r.size()<size)
        r.push_back({});
    while (r.size()>size)
        r.pop_back();

}
bool AnqCSV::clearCell(int row,int col)
{
    if (m_table[row].size()<=col)
        return false;
    m_table[row][col]=QString();
    return true;
}
