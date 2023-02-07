#include "anqinfolabel.h"

AnqInfoLabel::AnqInfoLabel(QWidget *parent)
    : QLabel{parent}
{

}
void AnqInfoLabel::setError(const QString &s)
{
    setText(s);
    auto pal=palette();
    pal.setColor(foregroundRole(),QColor(0xff,0x80,0x80));
    setPalette(pal);
}
void AnqInfoLabel::setInfo(const QString &s)
{
    setText(s);
    auto pal=palette();
    pal.setColor(foregroundRole(),QColor(0xe0,0xe0,0xe0));
    setPalette(pal);
}
