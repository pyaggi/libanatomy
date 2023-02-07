#include "fileswidget.h"
#include "anqfilewidget.h"
#include <QLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "anqjson.h"
FilesWidget::FilesWidget()
{
    auto l=new QVBoxLayout(this);
    auto f=new AnqFileWidget;
    f->setTitle(tr("TA-1 Json File"));
    l->addWidget(f);
    connect(f,&AnqFileWidget::fileSelected,this,&FilesWidget::selectTa1);

    f=new AnqFileWidget;
    f->setTitle(tr("TA-2 Json File"));
    l->addWidget(f);
    connect(f,&AnqFileWidget::fileSelected,this,&FilesWidget::selectTa2);

    f=new AnqFileWidget;
    f->setSaveMode(true);
    f->setTitle(tr("TA-X ouput Json File"));
    l->addWidget(f);
    connect(f,&AnqFileWidget::fileSelected,this,&FilesWidget::mixChanged);


}
QString FilesWidget::ta1File() const
{
    auto lay=static_cast<QVBoxLayout*>(layout());
    return static_cast<AnqFileWidget*>(lay->itemAt(0)->widget())->selectedFile();
}
QString FilesWidget::ta2File() const
{
    auto lay=static_cast<QVBoxLayout*>(layout());
    return static_cast<AnqFileWidget*>(lay->itemAt(1)->widget())->selectedFile();
}

void FilesWidget::loadOld()
{
    auto lay=static_cast<QVBoxLayout*>(layout());
    auto f=static_cast<AnqFileWidget*>(lay->itemAt(0)->widget());
    f->setSetting("ta1");
    f=static_cast<AnqFileWidget*>(lay->itemAt(1)->widget());
    f->setSetting("ta2");
    f=static_cast<AnqFileWidget*>(lay->itemAt(2)->widget());
    f->setSetting("mix");

}
std::vector<std::string> FilesWidget::toStringList(QJsonArray a)
{
    std::vector<std::string> r;
    for (auto i:a)
        r.push_back(i.toString().toStdString());
    return r;
}
void FilesWidget::selectTa1(const QString &s)
{
    AnqJson json;
    m_ta1.clear();
    auto ret=json.loadTa1(s,m_ta1);
    if (!ret)
        qobject_cast<AnqFileWidget *>(sender())->setError(json.errorString());
    else
        qobject_cast<AnqFileWidget *>(sender())->setInfo(tr("Records: %1").arg(ret));
    ta1Changed();
}
void FilesWidget::selectTa2(const QString &s)
{
    AnqJson json;
    m_ta2.clear();
    auto ret=json.loadTa2(s,m_ta2);
    if (!ret)
        qobject_cast<AnqFileWidget *>(sender())->setError(json.errorString());
    else
        qobject_cast<AnqFileWidget *>(sender())->setInfo(tr("Records: %1").arg(ret));
    ta2Changed();
}
