#include "anqtools.h"

QStringList Ana::toStringList(const std::vector<std::string> &l)
{
    QStringList ret;
    for (auto &i:l)
        if (!i.empty())
            ret.push_back(QString::fromStdString(i));
    return ret;
}
std::vector<std::string> Ana::toVector(const QStringList &l)
{
    std::vector<std::string> ret;
    for (auto &i:l)
        if (!i.isEmpty())
            ret.push_back(i.toStdString());
    return ret;
}
