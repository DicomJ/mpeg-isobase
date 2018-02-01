#ifndef __MPEG_ISOBASE_TRACK_FRAGMENT_HEADER_BOX__
#define __MPEG_ISOBASE_TRACK_FRAGMENT_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct TrackFragmentHeaderBox: FullBox {

    static constexpr Type container_type = 'traf';
    static constexpr Type box_type = 'tfhd';
    static constexpr const char *box_name = "TrackFragmentHeaderBox";

    UInt32<> track_ID;
    // all the following are optional fields
    UInt64<> base_data_offset;
    UInt32<> default_sample_description_index;
    UInt32<> default_sample_duration;
    UInt32<> default_sample_size;
    UInt32<> default_sample_flags;

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void TrackFragmentHeaderBox_(Composer &composer) {

        composer.get(track_ID);

        if (flags & 0x000001) {
            composer.get(base_data_offset);
        }
        if (flags & 0x000002) {
            composer.get(default_sample_description_index);
        }
        if (flags & 0x000008) {
            composer.get(default_sample_duration);
        }
        if (flags & 0x000010) {
            composer.get(default_sample_duration);
        }
        if (flags & 0x000020) {
            composer.get(default_sample_flags);
        }
    }

    TrackFragmentHeaderBox(Composer &composer, Flags flags, bool largesize = false): FullBox(composer, box_type, Version(0), flags, largesize) {
        TrackFragmentHeaderBox_(composer);
    }

    TrackFragmentHeaderBox(Parser &parser): FullBox(parser, Version(0)) {
        TrackFragmentHeaderBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TRACK_FRAGMENT_HEADER_BOX__

