#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <viewertree.h>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    class ViewerTree * genericViewer(const QString &title,const QStringList &s,ViewerTree::Mode isterm);
public slots:
    void about();
    void jsonViewer();
    void zJsonViewer();
    void jsonMatcher();
    void zJsonMatcher();
    void ta1Viewer();
    void ta2Viewer();
    void termsViewer();
private:
    QString confirmLast(const QString &title,const QString &set,const QStringList &l={});
    class ViewerTree * jsonViewerC();
    class TermMatcher * jsonMatcherC();
    class ViewerTree * zJsonViewerC();
    class TermMatcher * zJsonMatcherC();
    class ViewerTree * ta1ViewerC();
    class ViewerTree * ta2ViewerC();
    class ViewerTree * termsViewerC();
    QString findResource(const QString &title,const QStringList &s,bool report=true);
};
#endif // MAINWINDOW_H
