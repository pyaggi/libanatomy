#ifndef ANATOMY_H
#define ANATOMY_H
#include <string>
#include <qudefs.h>

#if defined(anatomycpp_EXPORTS)
#  define ANACPP_PUBLIC QUILL_DECL_PUBLIC
#  define ANACPP_PRIVATE QUILL_DECL_PRIVATE
#else
#  define ANACPP_PUBLIC QUILL_IMPORT
#  define ANACPP_PRIVATE
#endif

namespace Ana
{
enum class Sex {Unknown=0,Male=1,Female=2,Both=3};
enum class RecordType {Heading=0,Term=1};
enum class TermType {Unknown=0,Term,Group,Movement,Selection,_Last=Selection};
enum class TrStatus {New=0,AutoTranslated,Translated,Obsolete};

ANACPP_PUBLIC std::string_view termTypeCode(TermType);
ANACPP_PUBLIC std::string_view sexCode(Sex);
ANACPP_PUBLIC std::string_view trStatusCode(TrStatus);
ANACPP_PUBLIC std::string_view recordTypeCode(RecordType);

ANACPP_PUBLIC TermType termTypeFromCode(const std::string_view &);
ANACPP_PUBLIC Sex sexFromCode(const std::string_view &);
ANACPP_PUBLIC TrStatus trStatusFromCode(const std::string_view &);
ANACPP_PUBLIC RecordType recTypeFromCode(const std::string_view &);

ANACPP_PUBLIC uint8_t digitDammInt(const std::string_view &);
ANACPP_PUBLIC char digitDamm(const std::string_view &);
ANACPP_PUBLIC bool validateDamm(const std::string_view &);
ANACPP_PUBLIC std::string latinUnshort(const std::string_view &);
}
#endif // ANATOMY_H
