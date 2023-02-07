#include "trtextedit.h"
#include <QKeyEvent>
TrTextEdit::TrTextEdit(QWidget *parent)
    : QTextEdit{parent}
{
    setToolTip(tr("Ctrl+Return Finish this translation\nCtrl+Shift+Return Go to next translation"));
}
bool TrTextEdit::event(QEvent* ev)
{
    if (ev->type() == QEvent::KeyPress)
    {
        auto k=static_cast<QKeyEvent*>(ev);
        if (k->key()==Qt::Key::Key_Return |k->key()==Qt::Key::Key_Enter)
        {
            if (k->modifiers() & Qt::KeyboardModifier::ControlModifier)
            {
                if (k->modifiers() & Qt::KeyboardModifier::ShiftModifier)
                    leave();
                else
                    finished();
                return true ;
            }
        }
    }
    return QTextEdit::event(ev);
}
