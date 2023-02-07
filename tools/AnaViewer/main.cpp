#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
int main(int argc, char *argv[])
{
    qputenv("QT_FORCE_STDERR_LOGGING", QByteArray("1"));
    QApplication a(argc, argv);
    a.setApplicationName("AnaViewer");
    a.setOrganizationName("LibAnatomy");
    a.setApplicationVersion("1.0");
    MainWindow w;
    w.show();
    return a.exec();
}
