#ifndef __MPEG_ISOBASE_IPMPCONTROL_BOX__
#define __MPEG_ISOBASE_IPMPCONTROL_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct IPMPControlBox: Box {

    static constexpr Type container_type[] = {'moov', 'meta'};
    static constexpr Type box_type = 'ipmc';
    static constexpr const char *box_name = "IPMPControlBox";

    // TODO: Find definition
    IPMPControlBox(Parser &parser): Box(parser) {
    }
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_IPMPCONTROL_BOX__

