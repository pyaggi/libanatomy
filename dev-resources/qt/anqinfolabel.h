#ifndef INFOLABEL_H
#define INFOLABEL_H

#include <QLabel>

class AnqInfoLabel : public QLabel
{
    Q_OBJECT
public:
    explicit AnqInfoLabel(QWidget *parent = nullptr);

    void setError(const QString &);
    void setInfo(const QString &);
signals:

};

#endif // INFOLABEL_H
