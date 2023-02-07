#include "anatomy.h"
uint8_t Ana::digitDammInt(const std::string_view &number)
{
    static const char table[] = "0317598642709215486342068713591750983426612304597836742095815869720134894536201794386172052581436790";
    uint8_t interim=0,index;
    for (auto &digit:number)
    {
        index=(unsigned char)(digit - '0');
        if (index>9)
            return 0xff;
        interim = table[index + interim * 10]-'0';
    }
    return interim;
}
char Ana::digitDamm(const std::string_view &number)
{
    return digitDammInt(number)+'0';
}
bool Ana::validateDamm(const std::string_view &number)
{
    return digitDammInt(number)==0;
}
std::string Ana::latinUnshort(const std::string_view &v)
{
    if (v=="a.")
        return "arteria";
    if (v=="aa.")
        return "arteriae";
    if (v=="v.")
        return "vena";
    if (v=="vv.")
        return "venae";
    if (v=="m.")
        return "musculus";
    if (v=="mm.")
        return "musculi";
    if (v=="n.")
        return "nervus";
    if (v=="nn.")
        return "nervi";
    if (v=="n.")
        return "nervus";
    if (v=="nl.")
        return "nodus lymphoideus";
    if (v=="nll.")
        return "nodi lymphiodei";
    if (v=="lig.")
        return "ligamentum";
    if (v=="ligg.")
        return "ligamenta";
    if (v=="r.")
        return "ramus";
    if (v=="rr.")
        return "rami";
    if (v=="art.")
        return "articulatio";
    if (v=="artt.")
        return "articulationes";
    return std::string(v);
}
static const char *sexCodes[]={"","M","F","B"};
static const char *termCodes[]={"","T","G","M","S"};
static const char *trStatusCodes[]={"N","A","T","O"};
static const char *rectCodes[]={"H","T"};
std::string_view Ana::termTypeCode(Ana::TermType t)
{
    return termCodes[static_cast<int>(t)];
}
std::string_view Ana::sexCode(Ana::Sex t)
{
    return sexCodes[static_cast<int>(t)];
}
std::string_view Ana::recordTypeCode(RecordType rt)
{
    return rectCodes[static_cast<int>(rt)];
}
std::string_view Ana::trStatusCode(TrStatus trst)
{
    return trStatusCodes[static_cast<int>(trst)];
}
Ana::TermType Ana::termTypeFromCode(const std::string_view &s)
{
    for (int c=0;c<=static_cast<int>(TermType::_Last);c++)
        if (termCodes[c]==s)
            return static_cast<Ana::TermType>(c);
    return Ana::TermType::Unknown;
}
Ana::Sex Ana::sexFromCode(const std::string_view &s)
{
    for (int c=0;c<=static_cast<int>(Sex::Both);c++)
        if (sexCodes[c]==s)
            return static_cast<Ana::Sex>(c);
    return Ana::Sex::Unknown;

}
Ana::TrStatus Ana::trStatusFromCode(const std::string_view &s)
{
    for (int c=0;c<=static_cast<int>(TrStatus::Obsolete);c++)
        if (trStatusCodes[c]==s)
            return static_cast<Ana::TrStatus>(c);
    return Ana::TrStatus::New;

}
Ana::RecordType Ana::recTypeFromCode(const std::string_view &s)
{
    for (int c=0;c<=static_cast<int>(RecordType::Term);c++)
        if (rectCodes[c]==s)
            return static_cast<Ana::RecordType>(c);
    return Ana::RecordType::Heading;
}
