#include "anqfilewidget.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QFileDialog>
#include "anqinfolabel.h"
#include <QSettings>
AnqFileWidget::AnqFileWidget()
{
    auto lay=new QGridLayout(this);
    m_line=new QLineEdit;
    m_but=new QPushButton(tr("Select"));
    m_label=new AnqInfoLabel;

    lay->addWidget(m_line,0,0);
    lay->addWidget(m_but,0,1);
    lay->addWidget(m_label,1,0,1,2);
    m_label->setHidden(true);

    connect(m_line,&QLineEdit::editingFinished,this,&AnqFileWidget::lefin);
    connect(m_but,&QPushButton::clicked,this,qOverload<>(&AnqFileWidget::selectFile));
}
void AnqFileWidget::lefin()
{
    selectFile(m_line->text());
}
void AnqFileWidget::setTitle(const QString &s)
{
    m_line->setPlaceholderText(s);
}
void AnqFileWidget::selectFile()
{
    QString f;
    if (m_dir)
    {
        f=QFileDialog::getExistingDirectory(this,m_line->placeholderText());
    }
    else
    {
        if (m_save)
            f=QFileDialog::getSaveFileName(this,m_line->placeholderText());
        else
            f=QFileDialog::getOpenFileName(this,m_line->placeholderText());
    }
    selectFile(f);
}
void AnqFileWidget::setError(const QString &s)
{
    m_label->setError(s);
    m_label->setHidden(!s.length());
}
void AnqFileWidget::setInfo(const QString &s)
{
    m_label->setInfo(s);
    m_label->setHidden(!s.length());
}
void AnqFileWidget::selectFile(const QString &f)
{
    if (m_cur==f)
        return;
    m_cur=f;
    m_line->setText(f);
    if (!m_ign && !m_setting.isEmpty())
    {
        QSettings sets;
        sets.beginGroup("files");
        sets.setValue(m_setting,f);
    }
    emit fileSelected(f);
}
void AnqFileWidget::setSetting(const QString &s)
{
    m_setting=s;
    QSettings sets;
    sets.beginGroup("files");
    m_ign=true;
    selectFile(sets.value(s).toString());
    m_ign=false;
}
