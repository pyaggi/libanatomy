#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
#include <anatomyqt.h>
#include <QMessageBox>

class ANAQT_PUBLIC AnqAboutDialog:public QMessageBox
{
    Q_OBJECT
public:
    AnqAboutDialog(QWidget *parent=nullptr);
};

#endif // ABOUTDIALOG_H
