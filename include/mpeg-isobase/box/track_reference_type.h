#ifndef __MPEG_ISOBASE_TRACK_REFERENCE_TYPE_BOX__
#define __MPEG_ISOBASE_TRACK_REFERENCE_TYPE_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


template <uint32_t reference_type>
struct TrackReferenceTypeBox: Box {

    static constexpr Type container_type = 'tref';
    static constexpr Type box_type = reference_type;

    UInt32<>::Array track_IDs; // to end of the box

    template<typename Track_IDs>
    TrackReferenceTypeBox(Composer &composer, const Track_IDs &track_IDs, bool largesize = false): Box(composer, box_type, largesize) {
        composer.get(this->track_IDs, track_IDs);
    }

    TrackReferenceTypeBox(Parser &parser): Box(parser) {
        parser.get(*this, track_IDs);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TRACK_REFERENCE_TYPE_BOX__

