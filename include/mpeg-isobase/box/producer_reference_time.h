#ifndef __MPEG_ISOBASE_PRODUCER_REFERENCE_TIME_BOX__
#define __MPEG_ISOBASE_PRODUCER_REFERENCE_TIME_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ProducerReferenceTimeBox: FullBox {

    static constexpr Type box_type = 'prft';
    static constexpr const char *box_name = "ProducerReferenceTimeBox";

    union { Footprint<32+64> header;
        UInt32<>    reference_track_ID;
        UInt16<16>  ntp_timestamp;
    };

    template <FullBox::Version version, int w = (version == 0 ? 32 : 64)>
    struct MediaTime {
        UInt<w>  media_time;

        template <typename Composer>
        void MediaTime_(Composer &composer) {
            composer.get(media_time);
        }

        void output(bitstream::output::meta::field::Stream &stream) const;
    };
    MediaTime<0> v0;
    MediaTime<1> v1;

    uint64_t media_time() const { return version == 0 ? uint64_t(v0.media_time): uint64_t(v1.media_time); }

    ProducerReferenceTimeBox(Composer &composer, Version version, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(header);
        if (version == 0) {
            v0.MediaTime_(composer);
        } else {
            v1.MediaTime_(composer);
        }
    }

    ProducerReferenceTimeBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};



}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_PRODUCER_REFERENCE_TIME_BOX__

