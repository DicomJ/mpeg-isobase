#ifndef __MPEG_ISOBASE_TRACK_EXTENDS_BOX__
#define __MPEG_ISOBASE_TRACK_EXTENDS_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct TrackExtendsBox: FullBox {

    static constexpr Type container_type = 'mvex';
    static constexpr Type box_type = 'trex';
    static constexpr const char *box_name = "TrackExtendsBox";

    union { Footprint<5 * 32> header;
        UInt32< 0> track_ID;
        UInt32<32> default_sample_description_index;
        UInt32<64> default_sample_duration;
        UInt32<96> default_sample_size;
        UInt32<128> default_sample_flags;
    };

    TrackExtendsBox(Composer &composer, bool largesize = false): FullBox(composer, box_type, Version(0), Flags(0), largesize) {
        composer.get(header);
    }

    TrackExtendsBox(Parser &parser): FullBox(parser, Version(0), Flags(0)) {
        parser.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TRACK_EXTENDS_BOX__

