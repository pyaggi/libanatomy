#include "mainwindow.h"
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include "anqaboutdialog.h"
#include <QTabWidget>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <anqjson.h>
#include <termmatcher.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto file=menuBar()->addMenu(tr("Lib&Anatomy"));
    file->addAction(tr("Terms Viewer"),this,&MainWindow::termsViewer);
    file->addAction(tr("TA1 Viewer"),this,&MainWindow::ta1Viewer);
    file->addAction(tr("TA2 Viewer"),this,&MainWindow::ta2Viewer);
    file->addSeparator();
    file->addAction(tr("Z-Anatomy Terms Viewer"),this,&MainWindow::zJsonViewer);
    file->addAction(tr("Json Viewer"),this,&MainWindow::jsonViewer);
    file->addSeparator();
    file->addAction(tr("Z-Anatomy Terms Matcher"),this,&MainWindow::zJsonMatcher);
    file->addAction(tr("Json Matcher"),this,&MainWindow::jsonMatcher);
    file->addSeparator();
    file->addAction(tr("About"),this,&MainWindow::about);
    file->addAction(tr("About Qt"),qApp,&QApplication::aboutQt);

    setCentralWidget(termsViewerC());
}

MainWindow::~MainWindow()
{
}
void MainWindow::about()
{
    AnqAboutDialog diag;
    diag.exec();
}
void MainWindow::jsonViewer()
{
    auto v=jsonViewerC();
    if (v)
        v->show();
}
void MainWindow::zJsonViewer()
{
    auto v=zJsonViewerC();
    if (v)
        v->show();
}
void MainWindow::jsonMatcher()
{
    auto v=jsonMatcherC();
    if (v)
        v->show();
}
void MainWindow::zJsonMatcher()
{
    auto v=zJsonMatcherC();
    if (v)
        v->show();
}
void MainWindow::ta1Viewer()
{
    auto v=ta1ViewerC();
    if (v)
        v->show();
}
void MainWindow::ta2Viewer()
{
    auto v=ta2ViewerC();
    if (v)
        v->show();
}
void MainWindow::termsViewer()
{
    auto v=termsViewerC();
    if (v)
        v->show();
}

ViewerTree * MainWindow::genericViewer(const QString &title,const QStringList &s,ViewerTree::Mode mode)
{
    auto p=findResource(title,s);
    if (p.isEmpty())
        return nullptr;
    AnqJson json;
    auto arr=json.loadArray(p);
    if (arr.empty())
    {
        QMessageBox::critical(this,windowTitle(),tr("Invalid resource: %1").arg(json.errorString()));
        return nullptr;
    }
    auto v=new ViewerTree(mode) ;
    v->load(arr);
    return v;
}
class ViewerTree * MainWindow::ta1ViewerC()
{
    return genericViewer(tr("TA1 Json file"),{"ana_ta1_full.json","dev-resources/ana_ta1_full.json"},ViewerTree::Mode::TA1);
}
class ViewerTree * MainWindow::ta2ViewerC()
{
    return genericViewer(tr("TA2 Json file"),{"ana_ta2_full.json","dev-resources/ana_ta2_full.json"},ViewerTree::Mode::TA2);
}
class ViewerTree * MainWindow::termsViewerC()
{
    return genericViewer(tr("Terms Json file"),{"ana_terms_full.json","dev-resources/ana_terms_full.json"},ViewerTree::Mode::Terms);
}
#include <QSettings>
QString MainWindow::confirmLast(const QString &title,const QString &set,const QStringList &l)
{
    auto f=QSettings().value(set).toString();
    if (f.isEmpty() && l.size())
        f=findResource({},l,false);
    if (!f.isEmpty())
    {
        if (QMessageBox::question(this,windowTitle(),tr("Use last file: %1 ?").arg(f),QMessageBox::Yes,QMessageBox::No)
                ==QMessageBox::Yes)
            return f;
    }
    f=QFileDialog::getOpenFileName(this,windowTitle(),title);
    if (!f.isEmpty())
        QSettings().setValue(set,f);
    return f;
}
class ViewerTree * MainWindow::jsonViewerC()
{
    auto f=confirmLast(tr("Select JSON file"),"Last/ViewJson");
    if (!f.isEmpty())
        return genericViewer(tr("JSON File"),{f},ViewerTree::Mode::Generic);
    return nullptr;
}
class ViewerTree * MainWindow::zJsonViewerC()
{
    auto f=confirmLast(tr("Select Z-Anatomy terms file"),"Last/ViewZJson");
    if (f.isEmpty())
        return nullptr;
    return genericViewer(tr("Z-Anatomy Terms File"),{f},ViewerTree::Mode::ZAnatomy);
}
class TermMatcher * MainWindow::zJsonMatcherC()
{
    auto f=confirmLast(tr("Select Z-Anatomy terms file"),"Last/ViewZJson",{"sources/z-anatomy/z-anatomy-terms.json"});
    if (f.isEmpty())
        return nullptr;
    auto p=findResource(tr("Terms Json file"),{"ana_terms_full.json","dev-resources/ana_terms_full.json"});
    if (p.isEmpty())
        return nullptr;
    auto tm=new TermMatcher;
    if (!tm->load(f,ViewerTree::Mode::ZAnatomy,p))
    {
        delete tm;
        return nullptr;
    }
    return tm;
}
class TermMatcher * MainWindow::jsonMatcherC()
{
    auto f=confirmLast(tr("Select JSON file"),"Last/ViewJson");
    if (f.isEmpty())
        return nullptr;
    auto p=findResource(tr("Terms Json file"),{"ana_terms_full.json","dev-resources/ana_terms_full.json"});
    if (p.isEmpty())
        return nullptr;
    auto tm=new TermMatcher;
    if (!tm->load(f,ViewerTree::Mode::Generic,p))
    {
        delete tm;
        return nullptr;
    }
    return tm;
}
QString MainWindow::findResource(const QString &t,const QStringList &l,bool report)
{
    for (auto &s:l)
    {
        QDir d=QDir::current();
        if (d.exists(s))
            return d.absoluteFilePath(s);
        d.cd(QCoreApplication::applicationDirPath());
        if (d.exists(s))
            return d.absoluteFilePath(s);
        if (QDir::home().exists(s))
            return QDir::home().absoluteFilePath(s);
    }
    if (report)
        QMessageBox::critical(this,windowTitle(),tr("Resource %1 not found").arg(t));
    return {};
}
