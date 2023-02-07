#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString loadProject(const QString &path);
public slots:
    void about();
    void openProject();
    void generate();
private:
    void configure();
    void saveSettings();
    void loadSettings();
    QString m_projDir;
};
#endif // MAINWINDOW_H
