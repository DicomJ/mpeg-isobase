#ifndef __MPEG_ISOBASE_TRACK_FRAGMENT_RANDOM_ACCESS_BOX__
#define __MPEG_ISOBASE_TRACK_FRAGMENT_RANDOM_ACCESS_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct TrackFragmentRandomAccessBox: FullBox {

    static constexpr Type container_type = 'mfra';
    static constexpr Type box_type = 'tfra';
    static constexpr const char *box_name = "TrackFragmentRandomAccessBox";

    union { Footprint<32+ 26 + 3 * 2 + 32> header;
        UInt32<  0> track_ID;
        UInt<26, 32> reserved;  // = 0;
        UInt<2,  58> length_size_of_traf_num;
        UInt<2,  60> length_size_of_trun_num;
        UInt<2,  62> length_size_of_sample_num;
        UInt32<> number_of_entry;
    };

    TrackFragmentRandomAccessBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0)) {
        composer.get(header);
    }

    TrackFragmentRandomAccessBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(header);
    }

    // TODO: PROCESS PAYLOAD

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TRACK_FRAGMENT_RANDOM_ACCESS_BOX__

