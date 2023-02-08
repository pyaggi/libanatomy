#include "mainwindow.h"
#include <QMenuBar>
#include <QApplication>
#include "trworkspace.h"
#include "anqaboutdialog.h"
#include <QFileDialog>
#include "quilltrfile.h"
#include "anatrfile.h"
#include <QInputDialog>
#include "trauto.h"
#include <QStyle>
#include <QSettings>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/icons/logo.svg"));
    auto menu=menuBar()->addMenu(tr("&File"));
    auto action=menu->addAction(tr("&Open LibAnatomy Translation file"),this,&MainWindow::openAna);
    action->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_DialogOpenButton));
    menu->addAction(tr("Open &Quill Translation file"),this,&MainWindow::openQuill);
    m_recent=menu->addMenu(tr("Open Recent"));

    action=menu->addAction(tr("&Save"),this,&MainWindow::save);
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    action->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_DialogSaveButton));

    menu->addAction(tr("Save &as"),this,&MainWindow::saveAs);
    menu->addSeparator();
    menu->addAction(tr("&Translate to other language"),this,&MainWindow::newTr);

    menu=menuBar()->addMenu(tr("&Tools"));
    menu->addAction(tr("String list auto-translate"),this,&MainWindow::autoTrStrList);


    menu=menuBar()->addMenu(tr("&Help"));
    menu->addAction(tr("A&bout"),this,&MainWindow::about);
    menu->addAction(tr("About &Qt"),qApp,&QApplication::aboutQt);

    setCentralWidget(new TrWorkspace );

    loadSettings();
    updateRecentMenu();
}

MainWindow::~MainWindow()
{
}
void MainWindow::loadSettings()
{
    QSettings s;
    s.beginGroup("Files");
    auto max=s.beginReadArray("Recent");
    m_recentFiles.clear();
    m_recentTypes.clear();
    for (int c=0;c<max;c++)
    {
        s.setArrayIndex(c);
        m_recentFiles.push_back(s.value("file").toString());
        m_recentTypes.push_back(s.value("type").toInt());
    }
}
void MainWindow::saveSettings()
{
    QSettings s;
    s.beginGroup("Files");
    s.beginWriteArray("Recent",m_recentFiles.size());
    for (int c=0;c<m_recentFiles.size();c++)
    {
        s.setArrayIndex(c);
        s.setValue("file",m_recentFiles[c]);
        s.setValue("type",m_recentTypes[c]);
    }
    s.sync();
}
void MainWindow::addRecent()
{
    auto ws=static_cast<TrWorkspace*>(centralWidget());
    auto f=ws->file()->file();
    for (int c=0;c<m_recentFiles.size();c++)
    {
        if (m_recentFiles[c]==f)
        {
            if (c==0)
                return;
            m_recentFiles.removeAt(c);
            m_recentTypes.removeAt(c);
            break;
        }
    }
    m_recentFiles.push_front(f);
    m_recentTypes.push_front(dynamic_cast<QuillTrFile*>(ws->file())!=nullptr);
    while (m_recentFiles.size()>5)
    {
        m_recentFiles.pop_back();
        m_recentTypes.pop_back();
    }
    saveSettings();
    updateRecentMenu();
}
void MainWindow::updateRecentMenu()
{
    m_recent->clear();
    for (int c=0;c<m_recentFiles.size();c++)
    {
        auto action=m_recent->addAction(m_recentFiles[c],this,&MainWindow::openRecent);
        action->setData(c);
    }
}
void MainWindow::autoTrStrList()
{
    auto ws=static_cast<TrWorkspace*>(centralWidget());
    if (!ws->file())
        return;
    TrAuto a;
    a.setFile(ws->file());
    if (a.exec()==TrAuto::Accepted)
        ws->reloadFile();
}
void MainWindow::about()
{
    AnqAboutDialog diag;
    diag.exec();
}
void MainWindow::newTr()
{
    auto ws=static_cast<TrWorkspace*>(centralWidget());
    if (!ws->file())
        return;
    auto lang=QInputDialog::getText(this,windowTitle(),tr("Language in ISO format"));
    if (lang.isEmpty())
        return;
    ws->file()->createLanguage(lang);
    ws->reloadFile();
}
void MainWindow::saveAs()
{
    auto ws=static_cast<TrWorkspace*>(centralWidget());
    if (!ws->file())
        return;
    auto f=QFileDialog::getSaveFileName(this,windowTitle(),ws->file()->file(),ws->file()->fileFilter());
    if (f.isEmpty())
        return;
    auto s=ws->file()->save(ws->file()->file());
    if  (!s.isEmpty())
        QMessageBox::critical(this,windowTitle(),s);
    else
        addRecent();
}
void MainWindow::save()
{
    auto ws=static_cast<TrWorkspace*>(centralWidget());
    if (!ws->file())
        return;
    auto s=ws->file()->save(ws->file()->file());
    if  (!s.isEmpty())
        QMessageBox::critical(this,windowTitle(),s);
    else
        addRecent();
}
void MainWindow::openRecent()
{
    auto act=static_cast<QAction*>(sender());
    auto idx=act->data().toInt();
    TrFile *file;
    if (m_recentTypes[idx])
        file=new QuillTrFile;
    else
        file=new AnaTrFile;
    auto r=file->load(m_recentFiles[idx]);
    if (!r.isEmpty())
    {
        delete file;
        QMessageBox::critical(this,windowTitle(),r);
        return;
    }
    auto ws=static_cast<TrWorkspace*>(centralWidget());
    ws->setFile(file);
    addRecent();
}
void MainWindow::openQuill()
{
    auto tr=new QuillTrFile;
    auto f=QFileDialog::getOpenFileName(this,windowTitle(),{},tr->fileFilter());
    if (f.isEmpty())
    {
        delete tr;
        return;
    }
    auto r=tr->load(f);
    if (!r.isEmpty())
    {
        delete tr;
        QMessageBox::critical(this,windowTitle(),r);
        return;
    }
    auto ws=static_cast<TrWorkspace*>(centralWidget());
    ws->setFile(tr);
    addRecent();
}
void MainWindow::openAna()
{
    auto tr=new AnaTrFile;
    auto f=QFileDialog::getOpenFileName(this,windowTitle(),{},tr->fileFilter());
    if (f.isEmpty())
    {
        delete tr;
        return;
    }
    auto r=tr->load(f);
    if (!r.isEmpty())
    {
        delete tr;
        QMessageBox::critical(this,windowTitle(),r);
        return;
    }
    auto ws=static_cast<TrWorkspace*>(centralWidget());
    ws->setFile(tr);
    addRecent();
}
