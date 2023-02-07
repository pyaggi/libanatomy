#include "trworkspace.h"
#include "trlist.h"
#include <QSplitter>
#include <QLayout>
#include "quilltrfile.h"
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QTextBrowser>
#include "tredit.h"
#include <QPushButton>
TrWorkspace::TrWorkspace(QWidget *parent)
    : QWidget{parent}
{
    auto lay=new QVBoxLayout(this);
    auto sp=new QSplitter(this);
    lay->addWidget(sp);
    sp->addWidget((m_list=new TrList));

    auto w=new QWidget;
    auto wl=new QGridLayout(w);

    wl->addWidget(new QLabel(tr("File")),0,0);
    wl->addWidget(m_filePath=new QLineEdit,0,1);
    wl->addWidget(new QLabel(tr("Language")),1,0);
    wl->addWidget(m_language=new QLineEdit,1,1);

    wl->addWidget(new QLabel(tr("Context")),2,0);
    wl->addWidget(m_context=new QLineEdit,2,1);

    wl->addWidget(new QLabel(tr("Source")),3,0);
    wl->addWidget(m_source=new QTextBrowser,4,0,1,2);


    wl->addWidget(m_tr=new TrEdit,5,0,1,2);



    sp->addWidget(w);

    m_filePath->setReadOnly(true);
    m_language->setReadOnly(true);
    m_context->setReadOnly(true);
    m_source->setReadOnly(true);
    m_source->setAcceptRichText(true);

    sp->setStretchFactor(0,1);
    sp->setStretchFactor(1,4);

    connect(m_list,&TrList::activated,this,&TrWorkspace::act);
    connect(m_tr,&TrEdit::finished,this,&TrWorkspace::edFin);
    connect(m_tr,&TrEdit::leave,this,&TrWorkspace::edLeave);

    auto butw=new QWidget;
    auto butl=new QHBoxLayout(butw);
    butl->setAlignment(Qt::AlignmentFlag::AlignRight);
    wl->addWidget(butw,6,0,1,2);

    auto but=new QPushButton(tr("Next"));
//    but->setIcon(QIcon(":/icons/tr_fin.svg"));
    connect(but,&QPushButton::clicked,this,&TrWorkspace::edNext);
    butl->addWidget(but);
    but=new QPushButton(tr("Next Unfinished"));
    //    but->setIcon(QIcon(":/icons/tr_fin.svg"));
    connect(but,&QPushButton::clicked,this,&TrWorkspace::edNextUn);
    butl->addWidget(but);
    but=new QPushButton(tr("Work in progress"));
    but->setIcon(QIcon(":/icons/tr_work.svg"));
    connect(but,&QPushButton::clicked,this,&TrWorkspace::edWork);
    butl->addWidget(but);
    but=new QPushButton(tr("Finish"));
    but->setIcon(QIcon(":/icons/tr_fin.svg"));
    connect(but,&QPushButton::clicked,this,&TrWorkspace::edFin);
    butl->addWidget(but);

}
TrWorkspace::~TrWorkspace()
{
    delete m_file;
}

QString TrWorkspace::updateCurrent()
{
    auto trans=m_tr->translation();
    m_file->setTranslation(m_curidx,trans);
    auto o=m_tr->other();
    if (m_qmode)
    {
        int pl=1;
        for (auto &tr:o)
            m_file->setTranslation(m_curidx,tr,pl++);
    }
    else
        m_file->setSynonyms(m_curidx,o);
    return trans;
}
void TrWorkspace::edFin()
{
    if (m_curidx<0)
        return;
    updateCurrent();
    m_file->setStatus(m_curidx,TrStatus::Finished);
    m_list->setStatus(m_curidx,TrStatus::Finished);
    m_list->next();
}
void TrWorkspace::edWork()
{
    if (m_curidx<0)
        return;
    updateCurrent();
    m_file->setStatus(m_curidx,TrStatus::WorkInProgress);
    m_list->setStatus(m_curidx,TrStatus::WorkInProgress);
    m_list->next();
}
void TrWorkspace::edLeave()
{
    if (m_curidx<0)
        return;
    auto trans=updateCurrent();
    if (!trans.isEmpty() && m_file->status(m_curidx)==TrStatus::NewTr)
    {
        m_file->setStatus(m_curidx,TrStatus::WorkInProgress);
        m_list->setStatus(m_curidx,TrStatus::WorkInProgress);
    }
    m_list->next();
}
void TrWorkspace::edNext()
{
    if (m_curidx<0)
        return;
    m_list->next(false);
}
void TrWorkspace::edNextUn()
{
    if (m_curidx<0)
        return;
    m_list->next();
}
void TrWorkspace::reloadFile()
{
    m_list->clear();
    int max=m_file->count();
    for (int c=0;c<max;c++)
        m_list->addWord(m_file->status(c),m_file->source(c));
    m_language->setText(m_file->languageName());

    m_qmode=dynamic_cast<QuillTrFile*>(m_file);
    if (m_qmode)
        m_tr->setOtherTitle(tr("Plurals"));
    else
        m_tr->setOtherTitle(tr("Synonims"));

}
void TrWorkspace::setFile(class TrFile *f)
{
    m_list->clear();
    if (m_file)
        delete m_file;
    m_file=f;
    m_filePath->setText(f->file());
    reloadFile();
}
void TrWorkspace::act(int idx)
{
    if (m_curidx>=0)
    {
        auto trans=updateCurrent();
        if (!trans.isEmpty() && m_file->status(m_curidx)==TrStatus::NewTr)
        {
            m_file->setStatus(m_curidx,TrStatus::WorkInProgress);
            m_list->setStatus(m_curidx,TrStatus::WorkInProgress);
        }
    }
    m_curidx=idx;
    m_context->setText(m_file->context(idx));
    m_source->setText(m_file->source(idx).toHtmlEscaped());
    auto alt=m_file->alternativeSource(idx);
    auto altc=m_file->alternativeContext(idx);
    if (!alt.isEmpty())
    {
        m_source->append("<br><u>"+tr("Alternative")+"</u><br>");
        m_source->append(alt.toHtmlEscaped());
    }
    if (!altc.isEmpty())
    {
        m_source->append("<br><u>"+tr("Alternative Context")+"</u><br>");
        m_source->append(altc.toHtmlEscaped());
    }
    m_tr->setTranslation(m_file->translation(idx,0));
    if (m_qmode)
        m_tr->setOther(m_file->plurals(idx));
    else
        m_tr->setOther(m_file->synonyms(idx));
}
