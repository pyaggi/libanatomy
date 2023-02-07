#include "anqaboutdialog.h"
#include <QApplication>
AnqAboutDialog::AnqAboutDialog(QWidget *parent)
    :QMessageBox(parent)
{
    setIcon(Icon::Information);
    setWindowTitle(tr("About %1").arg(qApp->applicationName()));
    setText(tr("This tool is part of the libanatomy project.<br>"
               "Copyright 2023 - Pablo Yaggi<br>"
               "License: GPL V3.0<br>"
               "Project: https://github.com/pyaggi/libanatomy"
               ));
    addButton(StandardButton::Ok);
}
