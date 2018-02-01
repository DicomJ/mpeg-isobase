#ifndef __MPEG_ISOBASE_PROTECTION_SCHEME_INFO_BOX__
#define __MPEG_ISOBASE_PROTECTION_SCHEME_INFO_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ProtectionSchemeInfoBox: Box {

    static constexpr Type container_type = 'ipro';
    static constexpr Type box_type = 'sinf';
    static constexpr const char *box_name = "ProtectionSchemeInfoBox";

    //OriginalFormatBox(fmt) original_format;
    //SchemeTypeBox scheme_type_box; // optional
    //SchemeInformationBox info; // optional

    ProtectionSchemeInfoBox(Composer &composer): Box(composer, box_type) {}
    ProtectionSchemeInfoBox(Parser &parser): Box(parser) {}

    // TODO: PROCESS PAYLOAD
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_PROTECTION_SCHEME_INFO_BOX__

