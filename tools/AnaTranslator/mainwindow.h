#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void about();
    void openQuill();
    void openAna();
    void save();
    void saveAs();
    void newTr();
    void autoTrStrList();
private slots:
    void openRecent();
private:
    void addRecent();
    void updateRecentMenu();
    void loadSettings();
    void saveSettings();
    class QMenu *m_recent;
    QStringList m_recentFiles;
    QList<int> m_recentTypes;
};
#endif // MAINWINDOW_H
