#ifndef __MPEG_ISOBASE_TRACK_FRAGMENT_BASE_MEDIA_DECODE_TIME_BOX__
#define __MPEG_ISOBASE_TRACK_FRAGMENT_BASE_MEDIA_DECODE_TIME_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct TrackFragmentBaseMediaDecodeTimeBox: FullBox {

    static constexpr Type container_type = 'traf';
    static constexpr Type box_type = 'tfdt';
    static constexpr const char *box_name = "TrackFragmentBaseMediaDecodeTimeBox";

    template <FullBox::Version version, int w = (version == 1 ? 64: 32)>
    struct Header {
        UInt<w> baseMediaDecodeTime;
        Header() {}

        template <typename Composer>
        void Header_(Composer &composer) {
            composer.get(baseMediaDecodeTime);
        }

        void output(bitstream::output::meta::field::Stream &stream_) const;
    };
    Header<0> v0;
    Header<1> v1;

    uint64_t baseMediaDecodeTime() const { return version == 1 ? uint64_t(v1.baseMediaDecodeTime): uint64_t(v0.baseMediaDecodeTime); }

    template <typename Composer>
    void TrackFragmentBaseMediaDecodeTimeBox_(Composer &composer) {
        if (version == 1) {
            v1.Header_(composer);
        } else {
            v0.Header_(composer);
        }
    }

    TrackFragmentBaseMediaDecodeTimeBox(Composer &composer, Version version, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        TrackFragmentBaseMediaDecodeTimeBox_(composer);
    }

    TrackFragmentBaseMediaDecodeTimeBox(Parser &parser): FullBox(parser, Flags(0)) {
        TrackFragmentBaseMediaDecodeTimeBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TRACK_FRAGMENT_BASE_MEDIA_DECODE_TIME_BOX__

