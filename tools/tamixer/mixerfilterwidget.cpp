#include "mixerfilterwidget.h"
#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
MixerFilterWidget::MixerFilterWidget(QWidget *parent)
    : QWidget{parent}
{
    auto lay=new QHBoxLayout(this);
    auto but=new QPushButton(tr("Expand"));
    lay->addWidget(but);
    connect(but,&QPushButton::clicked,this,&MixerFilterWidget::expand);
    but=new QPushButton(tr("Collapse"));
    lay->addWidget(but);
    connect(but,&QPushButton::clicked,this,&MixerFilterWidget::collapse);

    m_id=new QLineEdit;
    m_id->setPlaceholderText(tr("ID filter"));
    lay->addWidget(m_id);
    m_text=new QLineEdit;
    m_text->setPlaceholderText(tr("Content filter"));
    lay->addWidget(m_text);
    m_unmatched=new QPushButton(tr("Unmatched"));
    m_unmatched->setCheckable(true);
    m_unmatched->setChecked(false);
    m_unmatched->setAutoDefault(false);
    lay->addWidget(m_unmatched);

    connect(m_id,&QLineEdit::textEdited,this,&MixerFilterWidget::filterUpdated);
    connect(m_text,&QLineEdit::textEdited,this,&MixerFilterWidget::filterUpdated);
    connect(m_unmatched,&QPushButton::toggled,this,&MixerFilterWidget::filterUpdated);


}
QString MixerFilterWidget::idFilter() const
{
    return m_id->text();
}
QString MixerFilterWidget::contentFilter() const
{
    return m_text->text();
}
bool MixerFilterWidget::unmatchedFilter() const
{
    return m_unmatched->isChecked();
}
