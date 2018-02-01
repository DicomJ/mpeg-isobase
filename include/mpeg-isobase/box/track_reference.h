#ifndef __MPEG_ISOBASE_TRACK_REFERENCE_BOX__
#define __MPEG_ISOBASE_TRACK_REFERENCE_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct TrackReferenceBox: Box {

    static constexpr Type container_type = 'trak';
    static constexpr Type box_type = 'tref';

    TrackReferenceBox(Composer &composer, bool largesize = false): Box(composer, box_type, largesize) {
    }

    TrackReferenceBox(Parser &parser): Box(parser) {
    }

    virtual void parse_payload(Remainder left_payload);
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TRACK_REFERENCE_BOX__

