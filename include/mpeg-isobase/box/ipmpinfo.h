#ifndef __MPEG_ISOBASE_IPMPINFO_BOX__
#define __MPEG_ISOBASE_IPMPINFO_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct IPMPInfoBox: Box {

    static constexpr Type container_type = 'sinf';
    static constexpr Type box_type = 'imif';
    static constexpr const char *box_name = "IPMPInfoBox";

    // TODO: Find definition
    IPMPInfoBox(Parser &parser): Box(parser) {
    }
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_IPMPINFO_BOX__

