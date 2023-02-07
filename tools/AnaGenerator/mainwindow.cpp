#include "mainwindow.h"
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include "termseditor.h"
#include "anqaboutdialog.h"
#include <QTabWidget>
#include "transeditor.h"
#include <QApplication>
#include "anqjson.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    loadSettings();
    if (m_projDir.isEmpty() || !loadProject(m_projDir).isEmpty())
    {
        m_projDir.clear();
        if (!loadProject("libanatomy").isEmpty() &&
                !loadProject("../libanatomy").isEmpty() &&
                !loadProject("../../libanatomy").isEmpty() &&
                !loadProject(QDir::home().absoluteFilePath("libanatomy")).isEmpty())
        {

        }
        else
            saveSettings();
    }
    auto file=menuBar()->addMenu(tr("Lib&Anatomy"));
    file->addAction(tr("Generate all outputs"),this,&MainWindow::generate);
    file->addSeparator();
    file->addAction(tr("About"),this,&MainWindow::about);
    file->addAction(tr("About Qt"),qApp,&QApplication::aboutQt);
    file->addSeparator();
    file->addAction(tr("Select project directory"),this,&MainWindow::openProject);

//    const QIcon exitIcon = QIcon::fromTheme("application-exit");
//    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), qApp, &QApplication::closeAllWindows);
//    exitAct->setShortcuts(QKeySequence::Quit);
//    exitAct->setStatusTip(tr("Exit the application"));
//    fileMenu->addAction(exitAct);
}

MainWindow::~MainWindow()
{
}
void MainWindow::about()
{
    AnqAboutDialog diag;
    diag.exec();
}
//void MainWindow::editTerms()
//{
//    if (m_projDir.isEmpty())
//        return;
//    auto e=new TermsEditor(this);
//    e->open(m_projDir);
//    setCentralWidget(e);
//}
void MainWindow::openProject()
{
    auto f=QFileDialog::getExistingDirectory(this);
    if (f.isEmpty())
        return;
    auto t=loadProject(f);
    if (!t.isEmpty())
        QMessageBox::critical(this,windowTitle(),t);
}
void MainWindow::loadSettings()
{
    QSettings sets;
    sets.beginGroup("files");
    m_projDir=sets.value("ProjectDir").toString();
}
void MainWindow::saveSettings()
{
    QSettings sets;
    sets.beginGroup("files");
    sets.setValue("ProjectDir",m_projDir);
}
void MainWindow::configure()
{
    setCentralWidget(nullptr);
    auto e=new TermsEditor(this);
    AnaTermTranslations translations;
    if (!e->open(m_projDir,translations))
    {
        delete e;
        return;
    }
    auto tw=new QTabWidget;
    tw->addTab(e,tr("Terms"));
    for (auto &v:translations)
    {
        auto tre=new TransEditor;
        tre->setup(e->terms(),v,
                   m_projDir+"/contents/ana_terms-"+QString::fromStdString(v.language())+".csv");
        tw->addTab(tre,QString::fromStdString(v.language()));
    }
    setCentralWidget(tw);
}
QString MainWindow::loadProject(const QString &path)
{
    QDir d(path);
    if (!d.exists())
        return tr("Doesn't exists");
    if (!d.exists("sources/ta/ta1-la.json") || !d.exists("sources/ta/ta1-en.json") || !d.exists("sources/ta/ta-map.json"))
        return {"Missing TA1 source files (sources/ta)"};
    if (!d.exists("sources/ta/ta2.json"))
        return {"Missing TA2 source file (sources/ta)"};
    m_projDir=path;
    saveSettings();
    setCentralWidget(nullptr);
    statusBar()->showMessage(tr("Libanatomy at %1").arg(m_projDir));

    configure();

    return {};
}
void MainWindow::generate()
{
    if (m_projDir.isEmpty())
        return;
    auto tw=static_cast<QTabWidget*>(centralWidget());
    if (!tw)
        return;
    auto terms=static_cast<TermsEditor*>(tw->widget(0));
    terms->csvSave(m_projDir+"/data-resources/ana_terms.csv");
    terms->jsonSave(m_projDir+"/dev-resources/ana_terms.json");
    AnaTermTranslations trs;
    for (int c=1;c<tw->count();c++)
    {
        auto ed=static_cast<TransEditor*>(tw->widget(c));
        auto lang=QString::fromStdString(ed->translations().language());
        ed->csvSave(m_projDir+"/content/ana_terms-"+lang+".csv",false);
        ed->csvSave(m_projDir+"/data-resources/ana_terms-"+lang+".csv",true);
        ed->jsonSave(m_projDir+"/dev-resources/ana_terms-"+lang+".json");
        trs.push_back(ed->translations());
    }
    terms->csvFullSave(m_projDir+"/data-resources/ana_terms_full.csv",trs);
    terms->jsonFullSave(m_projDir+"/dev-resources/ana_terms_full.json",trs);
    AnqJson json;
    if (!json.save(m_projDir+"/dev-resources/ana_ta1_full.json",terms->terms(),terms->ta1La(),terms->ta1En(),trs)||
        !json.save(m_projDir+"/dev-resources/ana_ta2_full.json",terms->terms(),terms->ta2(),trs)
            )
        QMessageBox::critical(this,windowTitle(),tr("Can't save json: %1").arg(json.errorString()));

}
