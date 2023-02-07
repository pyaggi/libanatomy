#include "transeditor.h"
#include <anaterm.h>
#include <anatermstrings.h>
#include <QTreeWidget>
#include "anqtools.h"
#include "anqcsv.h"
#include <QMessageBox>
#include "anqjson.h"
TransEditor::TransEditor(QWidget *parent)
    : AnqTreeWidget{parent}
{
    addStandardControls();
    tree()->setHeaderLabels({tr("Id"),tr("Status"),tr("Latin"),tr("English"),
                            tr("Term"),tr("Synonyms"),tr("Parent")});
}
void TransEditor::setup(const class AnaTermTree &terms ,const class AnaTermStringsMap &autoTr,const QString &csv)
{
    m_tr=autoTr;
    m_csv=csv;
    AnqCSV csvParser;
    if (csvParser.load(csv).isEmpty())
    {
        int cols[7]={-1,-1,-1,-1,-1,-1,-1};
        int col=0;
        for (auto &h:csvParser.header())
        {
            if (h.compare("Status",Qt::CaseInsensitive)==0)
                cols[0]=col;
            else if (h.compare("Id",Qt::CaseInsensitive)==0)
                cols[1]=col;
            else if (h.compare("Parent",Qt::CaseInsensitive)==0)
                cols[2]=col;
            else if (h.compare("LatinName",Qt::CaseInsensitive)==0)
                cols[3]=col;
            else if (h.compare("UsEnglish",Qt::CaseInsensitive)==0 || h.compare("English",Qt::CaseInsensitive)==0)
                cols[4]=col;
            else if (h.compare("Translation",Qt::CaseInsensitive)==0)
                cols[5]=col;
            else if (h.compare("Synonyms",Qt::CaseInsensitive)==0)
                cols[6]=col;
            col++;
        }
        if (cols[0]<0 || cols[1]<0 || cols[5]<0 || cols[6]<0)
        {
            QMessageBox::critical(this,windowTitle(),tr("Invalid Translations file: %1").arg(csv));
            return ;
        }
        for (auto &r:csvParser.table())
        {
            auto st=Ana::trStatusFromCode(r[cols[0]].toStdString());
            AnaTermId id;
            id.setIdValue(r[cols[1]].toInt());
            auto term=r[cols[5]].trimmed();
            auto synonyms=r[cols[6]].split(";");
            for (auto &s:synonyms)
                s=s.trimmed();
            auto &trterm=m_tr[id];
//            trterm.setId(id);
            trterm.setStatus(st);
            trterm.setTerm(term.toStdString());
            trterm.setSynonyms(Ana::toVector(synonyms));

        }

    }
    for (auto &t:m_tr)
    {
        auto i=new QTreeWidgetItem(tree());
        auto &termData=terms.node(t.first)->data();
        i->setText(0,QString::number(t.first.toInt()));
        i->setText(1,QString(Ana::trStatusCode(t.second.status()).data()));
        i->setText(2,QString::fromStdString(termData.latin()));
        i->setText(3,QString::fromStdString(termData.english()));
        i->setText(4,QString::fromStdString(t.second.term()));
        i->setText(5,Ana::toStringList(t.second.synonyms()).join("; "));
        i->setText(6,QString::fromStdString(termData.parentId().toString()));
    }
}
bool TransEditor::csvSave(const QString &loc,bool data)
{
    AnqCSV csv;
    if (data)
    {
        csv.setHeader({"Status","Id","Translation","Synonyms"});
        std::vector<QTreeWidgetItem*> items;
        collect(items);
        for (auto &i:items)
        {
            QStringList row;
            row.push_back(i->text(1));
            row.push_back(i->text(0));
            row.push_back(i->text(4));
            row.push_back(i->text(5));
            csv.addRow(row);
        }
    }
    else
    {
        csv.setHeader({"Status","Id","Parent","LatinName","UsEnglish","Translation","Synonyms"});
        std::vector<QTreeWidgetItem*> items;
        collect(items);
        for (auto &i:items)
        {
            QStringList row;
            row.push_back(i->text(1));
            row.push_back(i->text(0));
            row.push_back(i->text(6));
            row.push_back(i->text(2));
            row.push_back(i->text(3));
            row.push_back(i->text(4));
            row.push_back(i->text(5));
            csv.addRow(row);
        }
    }
    auto str=csv.save(loc);
    if (!str.isEmpty())
    {
        QMessageBox::critical(this,windowTitle(),tr("Can't save CSV: %1").arg(str));
        return false;
    }
    return true;
}
bool TransEditor::jsonSave(const QString &loc)
{
    AnqJson json;
    if (!json.save(loc,m_tr))
    {
        QMessageBox::critical(this,windowTitle(),tr("Can't savee Json: %1").arg(json.errorString()));
        return false;
    }
    return true;
}
