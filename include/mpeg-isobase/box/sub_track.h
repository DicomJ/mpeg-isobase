#ifndef __MPEG_ISOBASE_SUB_TRACK__
#define __MPEG_ISOBASE_SUB_TRACK__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SubTrack: Box {

    static constexpr Type container_type[] = {'udta', 'trak'};
    static constexpr Type box_type = 'strk';
    static constexpr const char *box_name = "SubTrack";

    SubTrack(Composer &composer, bool largesize = false): Box(composer, box_type, largesize) {}
    SubTrack(Parser &parser): Box(parser) {}
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SUB_TRACK__

