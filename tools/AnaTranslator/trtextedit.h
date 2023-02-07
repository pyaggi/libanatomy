#ifndef TRTEXTEDIT_H
#define TRTEXTEDIT_H

#include <QTextEdit>

class TrTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit TrTextEdit(QWidget *parent = nullptr);

signals:
    void finished();
    void leave();
private:
    bool event(QEvent* ev) override;

};

#endif // TRTEXTEDIT_H
