#include "trauto.h"
#include <QSplitter>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include "anatrfile.h"
#include <QStyle>
#include <QMessageBox>
TrAuto::TrAuto(QWidget *parent)
    : QDialog{parent}
{
    auto lay=new QGridLayout(this);
    m_alt=new QPushButton(tr("A.Source"));
    m_alt->setCheckable(true);
    connect(m_alt,&QPushButton::clicked,this,&TrAuto::scanFile);
    lay->addWidget(m_alt,0,0);
    lay->addWidget(new QLabel(tr("Characters per page")),0,1);
    lay->addWidget(new QLabel(tr("Page")),0,3);

    m_chars=new QLineEdit;
    m_chars->setText(QString::number(m_curmax));

    lay->addWidget(m_chars,0,2);
    m_page=new QSpinBox;
    lay->addWidget(m_page,0,4);

    connect(m_chars,&QLineEdit::editingFinished,this,&TrAuto::updateFragsCond);
    connect(m_page,qOverload<int>(&QSpinBox::valueChanged),this,&TrAuto::selectPage);

    auto sp=new QSplitter;
    sp->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    lay->addWidget(sp,1,0,1,5);

    m_in=new QTextEdit;
    m_in->setToolTip(tr("Sources"));
    m_out=new QTextEdit;
    m_out->setToolTip(tr("Translations"));
    sp->addWidget(m_in);
    sp->addWidget(m_out);

    auto w=new QWidget;
    lay->addWidget(w,2,0,1,5);
    lay->setAlignment(Qt::AlignmentFlag::AlignRight);
    auto butl=new QHBoxLayout(w);
    auto but=new QPushButton(tr("Mark Auto-Translated"));
    but->setIcon(QIcon(":/icons/tr_auto.svg"));
    connect(but,&QPushButton::clicked,this,&TrAuto::autoTranslated);
    butl->addWidget(but);
    but=new QPushButton(tr("Mark Translated"));
    but->setIcon(QIcon(":/icons/tr_fin.svg"));
    connect(but,&QPushButton::clicked,this,&TrAuto::translated);
    butl->addWidget(but);
    but=new QPushButton(tr("Close"));
    but->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    connect(but,&QPushButton::clicked,this,&TrAuto::reject);
    butl->addWidget(but);
}
void TrAuto::autoTranslated()
{
    updateStrings(TrStatus::AutoTr);
    accept();
}
void TrAuto::translated()
{
    auto r=QMessageBox::warning(this,windowTitle(),
                                tr("If you used an automatic translation service you should use the Auto-Translated mark, continue ?"),
                                QMessageBox::Button::Yes,QMessageBox::No);
    if (r!=QMessageBox::Yes)
        return;
    updateStrings(TrStatus::Finished);
    accept();
}
void TrAuto::updateStrings(TrStatus st)
{
    savePage();
    int max=m_file->count();
    int curpage=0;
    int curidx=0;
    auto ps=pageSize(curpage);

    for (int c=0;c<max;c++)
    {
        if (m_file->status(c)!=TrStatus::NewTr)
            continue;
        if (curidx==0 && m_outList[curpage].size()>ps)
        {
            auto r=QMessageBox::warning(this,windowTitle(),
                                        tr("Page %1 has more translations than sources, extra will be discarded.\nTranslate anyway ?").arg(curpage+1),
                                        QMessageBox::Button::Yes,QMessageBox::Abort);
            if (r!=QMessageBox::Yes)
            {
                if (curpage)
                    scanFile(m_alt->isChecked());
                return;
            }
        }
        if (curidx<m_outList[curpage].size())
        {
            auto str=m_outList[curpage][curidx];
            if (!str.isEmpty())
            {
                m_file->setTranslation(c,str);
                m_file->setStatus(c,st);
            }
        }
        if (++curidx>=ps)
        {
            curidx=0;
            curpage++;
            ps=pageSize(curpage);
        }
    }
    scanFile(m_alt->isChecked());
}
void TrAuto::scanFile(bool t)
{
    if (!m_file)
        return;
    QStringList l;
    int max=m_file->count();
    for (int c=0;c<max;c++)
    {
        if (m_file->status(c)==TrStatus::NewTr)
            l.push_back(t?m_file->alternativeSource(c):m_file->source(c));
    }
    setStrings(l);
}
void TrAuto::setFile(class TrFile *file)
{
    m_file=nullptr;
    if (dynamic_cast<AnaTrFile*>(file))
        m_alt->setChecked(true);
    m_file=file;
    scanFile(m_alt->isChecked());
}

void TrAuto::setStrings(const QStringList &l)
{
    m_list=l;
    updateFrags();
}
void TrAuto::savePage()
{
    if (m_curpage<0)
        return;
    auto t=m_out->toPlainText().split("\n");
    for (auto &s:t)
        s=s.trimmed();
    m_outList[m_curpage]=t;
}
void TrAuto::loadPage()
{
    if (m_curpage<0)
        return;
    m_out->setText(m_outList[m_curpage].join("\n"));
}
void TrAuto::updateFragsCond()
{
    auto maxchars=m_chars->text().toInt();
    if (!maxchars)
        maxchars=DEFAULT_MAX;
    if (maxchars==m_curmax)
        return;
    m_curmax=maxchars;
    updateFrags();
}
void TrAuto::updateFrags()
{
    savePage();
    m_frags.clear();
    m_outList.clear();
    m_curpage=-1;

    m_outList.push_back({});

    int count=0;
    int idx=0;
    for (auto &s:m_list)
    {
        count+=s.length();
        count+=1;
        if (count>m_curmax)
        {
            m_frags.push_back(idx);
            m_outList.push_back({});
            count=s.length();
            count+=1;
        }
        idx++;
    }
    m_outList.push_back({});
    m_ign=true;
    m_page->setRange(1,m_frags.size()+1);
    m_page->setValue(1);
    m_ign=false;
    selectPage(1);
}
void TrAuto::selectPage(int p)
{
    if (m_ign)
        return;
    savePage();
    p--;
    m_curpage=p;
    auto range=pageRange(p);
    m_in->clear();
    for (int c=range.first;c<range.second;c++)
        m_in->append(m_list[c]);
    loadPage();
}
std::pair<int,int> TrAuto::pageRange(int p) const
{
    int from=0,to;
    if (p<m_frags.size())
        to=m_frags[p];
    else
        to=m_list.size();
    if (p>0)
        from=m_frags[p-1];
    else
        from=0;
    return {from,to};
}
int TrAuto::pageSize(int idx) const
{
    auto r=pageRange(idx);
    return r.second-r.first;
}
