#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_FORCE_STDERR_LOGGING", QByteArray("1"));
    QApplication a(argc, argv);
    a.setApplicationName("TaMixer");
    a.setOrganizationName("LibAnatomy");
    a.setApplicationVersion("1.0");
    MainWindow w;
    w.show();
    return a.exec();
}
