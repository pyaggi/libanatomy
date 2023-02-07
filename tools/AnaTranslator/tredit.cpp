#include "tredit.h"
#include "trtextedit.h"
#include <QLayout>
#include <QSpinBox>
#include <QLabel>
TrEdit::TrEdit(QWidget *parent)
    : QWidget{parent}
{
    auto grid=new QGridLayout(this);
    grid->addWidget(new QLabel(tr("Translation")),0,0);
    grid->addWidget(new QLabel(tr("Plurals")),0,2,Qt::AlignRight);
    QSpinBox *box=new QSpinBox;
    box->setRange(0,10);
    grid->addWidget(box,0,3);
    connect(box,qOverload<int>(&QSpinBox::valueChanged),this,&TrEdit::ch);
    add();
}
void TrEdit::setTranslation(const QString &s)
{
    m_tr[0]->setText(s);
}
void TrEdit::setOther(const QStringList &l)
{
    auto b=static_cast<QGridLayout*>(layout())->itemAtPosition(0,3)->widget();
    static_cast<QSpinBox*>(b)->setValue(l.size());
    int idx=1;
    for (auto &i:l)
        m_tr[idx++]->setText(i);
}

void TrEdit::setOtherTitle(const QString &s)
{
    auto l=static_cast<QGridLayout*>(layout())->itemAtPosition(0,2)->widget();
    static_cast<QLabel*>(l)->setText(s);
}

void TrEdit::finAct()
{
    int max=m_tr.size();
    for (int c=0;c<max;c++)
    {
        if (m_tr[c]==sender())
        {
            if (c==max-1)
                finished();
            else
                m_tr[c+1]->setFocus();
            return;
        }
    }
}
void TrEdit::leaveAct()
{
    int max=m_tr.size();
    for (int c=0;c<max;c++)
    {
        if (m_tr[c]==sender())
        {
            if (c==max-1)
                leave();
            else
                m_tr[c+1]->setFocus();
            return;
        }
    }
}
void TrEdit::ch(int val)
{
    val++;
    while (val<m_tr.size())
        remove();
    while (val>m_tr.size())
        add();
}
void TrEdit::add()
{
    auto tr=new TrTextEdit;
    connect (tr,&TrTextEdit::finished,this,&TrEdit::finAct);
    connect (tr,&TrTextEdit::leave,this,&TrEdit::leaveAct);
    m_tr.push_back(tr);
    static_cast<QGridLayout*>(layout())->addWidget(tr,m_tr.size(),0,1,4);
}
void TrEdit::remove()
{
    if (m_tr.size()>1)
    {
        delete m_tr.back();
        m_tr.pop_back();
    }
}
QString TrEdit::translation() const
{
    return m_tr[0]->toPlainText();
}
QStringList TrEdit::other() const
{
    QStringList ret;
    auto max=m_tr.size();
    for (int c=1;c<max;c++)
        ret.push_back(m_tr[c]->toPlainText());
    return ret;
}
