#include "mainwindow.h"
#include <fileswidget.h>
#include <mixerwidget.h>
#include <QLayout>
#include <filesystem>
#include <QMenuBar>
#include "anqaboutdialog.h"
#include <QApplication>
#include "anqta1tree.h"
#include "anqta2tree.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/icons/logo.svg"));
    setWindowTitle("TA Mixer");
    auto w=new QWidget;
    auto l=new QVBoxLayout(w);
    m_files=new FilesWidget;
    l->addWidget(m_files);
    m_mixer=new MixerWidget;
    l->addWidget(m_mixer);
    setCentralWidget(w);


    connect(m_files,&FilesWidget::ta1Changed,this,&MainWindow::ta1Mod);
    connect(m_files,&FilesWidget::ta2Changed,this,&MainWindow::ta2Mod);
    connect(m_files,&FilesWidget::mixChanged,m_mixer,&MixerWidget::setMixFile);

    m_files->loadOld();

    auto file=menuBar()->addMenu(tr("Lib&Anatomy"));
    file->addAction(tr("TA1 Viewer"),this,&MainWindow::viewTa1);
    file->addAction(tr("TA2 Viewer"),this,&MainWindow::viewTa2);
    file->addSeparator();
    file->addAction(tr("About"),this,&MainWindow::about);
    file->addAction(tr("About Qt"),qApp,&QApplication::aboutQt);
    file->addSeparator();
    file->addAction(tr("Check TA1"),this,&MainWindow::checkTa1);
//    file->addAction(tr("Fix TA1"),this,&MainWindow::fixTa1);


}

MainWindow::~MainWindow()
{
}
void MainWindow::collectTa1Errors(std::vector<std::string> &errors)
{
    for (auto &n:*m_files->ta1().idMap())
    {
        auto p=n.second->parent();
        if (!p || p->id().empty())
            continue;
        if (p->id()>n.first)
            errors.push_back(n.first);
        else
        {
            QStringList cl=QString::fromStdString(n.first).split('.');
            QStringList pl=QString::fromStdString(p->id()).split('.');
            if (cl.size()!=4 || pl.size()!=4)
                errors.push_back(n.first);
            else
            {
                if (cl[0]!=pl[0])
                    errors.push_back(n.first);
            }
        }
    }
}
static bool containsError(const std::vector<std::string> &errors,const AnaTA1::Node *node)
{
    while (node)
    {
        if (std::find(errors.begin(),errors.end(),node->id())!=errors.end())
            return true;
        node=node->parent();
    }
    return false;
}
void MainWindow::fixTa1()
{
    std::vector<std::string> errors;
    collectTa1Errors(errors);
    if (!errors.size())
        QMessageBox::information(this,windowTitle(),tr("Nothing to fix"));
    auto &ta=m_files->ta1();
    int fix=0;
    for (auto &e:errors)
    {
        auto id=QString::fromStdString(e);
        auto node=ta.node(e);
        auto prevNode=ta.findPreviousId(e);

        while (prevNode && containsError(errors,prevNode))
            prevNode=ta.findPreviousId(prevNode->id());

        auto nextNode=ta.findNextId(e);
        while (nextNode && containsError(errors,nextNode))
            nextNode=ta.findNextId(nextNode->id());
        if (prevNode && nextNode)
        {
            if (prevNode->parent()==nextNode->parent())
            {
                qDebug()<<"moved "<<id<<" prevNode" <<QString::fromStdString(prevNode->parent()->id())<<"/"<<QString::fromStdString(prevNode->id())<<" nextNode "<<QString::fromStdString(nextNode->parent()->id())<<"/"<<QString::fromStdString(nextNode->id());
                prevNode->appendSibling(node);
                fix++;
                continue;
            }
            qDebug()<<id<<" prevNode" <<QString::fromStdString(prevNode->id())<<" nextNode "<<QString::fromStdString(nextNode->id());
        }
        else if (prevNode)
            qDebug()<<id<<" prevNode" <<QString::fromStdString(prevNode->id());
        else if (nextNode)
            qDebug()<<id<<" nextNode" <<QString::fromStdString(nextNode->id());
        else
            qDebug()<<id<<" ???";
    }
    m_mixer->loadTa1(ta);
    for (auto &e:errors)
    {
        auto ta1=QString::fromStdString(e);
        m_mixer->highlightTa1(ta1);
    }
    QMessageBox::critical(this,windowTitle(),tr("Fixed %1 of %2 errors").arg(fix).arg(errors.size()));

}
void MainWindow::checkTa1()
{
    std::vector<std::string> errors;
    collectTa1Errors(errors);
    if (!errors.size())
        QMessageBox::information(this,windowTitle(),tr("TA1 file checked out"));
    else
    {
        for (auto &e:errors)
        {
            auto ta1=QString::fromStdString(e);
            qDebug()<<ta1;
            m_mixer->highlightTa1(ta1);
        }
        QMessageBox::critical(this,windowTitle(),tr("Found %1 errors").arg(errors.size()));
    }
}
void MainWindow::about()
{
    AnqAboutDialog diag;
    diag.exec();
}
void MainWindow::ta1Mod()
{
    m_mixer->loadTa1(m_files->ta1());
    std::filesystem::path f(m_files->ta1File().toStdString());
    QString fn=QString::fromStdString(f.filename().stem().u8string());
    if (fn.endsWith("-la"))
        m_mixer->setLanguage(MixerWidget::Lat);
    else if (fn.endsWith("-us"))
        m_mixer->setLanguage(MixerWidget::Us);
    else if (fn.endsWith("-uk"))
        m_mixer->setLanguage(MixerWidget::Uk);
    else if (fn.endsWith("-en"))
        m_mixer->setLanguage(MixerWidget::Uk);

}
void MainWindow::ta2Mod()
{
    m_mixer->loadTa2(m_files->ta2());
}
void MainWindow::viewTa1()
{
    auto n=new AnqTa1Tree;
    n->loadTerms(m_files->ta1());
    n->setWindowTitle(tr("%1 TA Viewer").arg(windowTitle()));
    n->show();
}
void MainWindow::viewTa2()
{
    auto n=new AnqTa2Tree;
    n->loadTerms(m_files->ta2());
    n->setWindowTitle(tr("%2 TA Viewer").arg(windowTitle()));
    n->show();
}
