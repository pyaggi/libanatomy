#include "anaterm.h"

AnaTerm::AnaTerm()
{

}
AnaTermCursor AnaTermTree::findTA1(const std::string &s)
{
    for (auto &n:*idMap())
    {
        if (n.second->data().ta1Code()==s)
            return AnaTermCursor(this,n.second);
    }
    return AnaTermCursor(this).endCursor();
}
AnaTermConstCursor AnaTermTree::findTA1(const std::string &s) const
{
    for (auto &n:*idMap())
    {
        if (n.second->data().ta1Code()==s)
            return AnaTermConstCursor(this,n.second);
    }
    return AnaTermConstCursor(this).endCursor();
}
