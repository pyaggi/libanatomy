#ifndef FILESWIDGET_H
#define FILESWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <anata1.h>
#include <anata2.h>
class FilesWidget:public QWidget
{
    Q_OBJECT
public:
    FilesWidget();
    AnaTA1 &ta1() {return m_ta1;}
    AnaTA2 &ta2() {return m_ta2;}
    void loadOld();
    QString ta1File() const;
    QString ta2File() const;

signals:
    void ta1Changed();
    void ta2Changed();
    void mixChanged(const QString &s);

private slots:
    void selectTa1(const QString &s);
    void selectTa2(const QString &s);
private:
    std::vector<std::string> toStringList(QJsonArray);
    AnaTA1 m_ta1;
    AnaTA2 m_ta2;
};

#endif // FILESWIDGET_H
