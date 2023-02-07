#ifndef ANATOOLS_H
#define ANATOOLS_H
#include <string>
#include <anatomyqt.h>
#include <QStringList>
namespace Ana
{
ANAQT_PUBLIC QStringList toStringList(const std::vector<std::string> &);
ANAQT_PUBLIC std::vector<std::string> toVector(const QStringList &);
}
#endif // ANATOOLS_H
