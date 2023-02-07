#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void about();
    void checkTa1();
    void fixTa1();
    void viewTa1();
    void viewTa2();

private:
    void collectTa1Errors(std::vector<std::string> &);
    void ta1Mod();
    void ta2Mod();
    class FilesWidget *m_files;
    class MixerWidget *m_mixer;
};
#endif // MAINWINDOW_H
