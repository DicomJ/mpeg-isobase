#ifndef __MPEG_ISOBASE_MEDIA_HEADER_BOX__
#define __MPEG_ISOBASE_MEDIA_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct MediaHeaderBox: FullBox {

    static constexpr Type container_type = 'mdia';
    static constexpr Type box_type = 'mdhd';
    static constexpr const char *box_name = "MediaHeaderBox";

    template <FullBox::Version version, int w = (version == 1 ? 64 : 32)>
    struct Header {
        union { Footprint<3 * w + 32> header;
            UInt<w,  0>     creation_time;
            UInt<w,  w>     modification_time;
            UInt32<2*w>     timescale;
            UInt<w, 2*w+32> duration;
        };

        Header() {}

        template <typename Composer>
        void Header_(Composer &composer) {
            composer.get(header);
        }

        void output(bitstream::output::meta::field::Stream &stream_) const;
    };
    Header<0> v0;
    Header<1> v1;

    union { Footprint<1+ 5 * 3 + 16> header;
        UInt<1,  0>                   pad;              // = 0;
        UInt<5,  1>::Static::Array<3> language;         // ISO-639-2/T language code
        UInt16< 16>                   pre_defined;      // = 0;
    };

    uint64_t creation_time() const      { return version == 1 ? uint64_t(v1.creation_time):     uint64_t(v0.creation_time);     }
    uint64_t modification_time() const  { return version == 1 ? uint64_t(v1.modification_time): uint64_t(v0.modification_time); }
    uint32_t timescale() const          { return version == 1 ? uint32_t(v1.timescale):         uint32_t(v0.timescale);         }
    uint64_t duration() const           { return version == 1 ? uint64_t(v1.duration):          uint64_t(v0.duration);          }

    void creation_time(uint64_t creation_time)          { if (version == 1) v1.creation_time = creation_time;           else  v0.creation_time = creation_time; }
    void modification_time(uint64_t modification_time)  { if (version == 1) v1.modification_time = modification_time;   else  v0.modification_time = modification_time; }
    void timescale(uint32_t timescale)                  { if (version == 1) v1.timescale = timescale;                   else  v0.timescale = timescale; }
    void duration(uint64_t duration)                    { if (version == 1) v1.duration = duration;                     else  v0.duration = duration; }

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void MediaHeaderBox_(Composer &composer) {
        if (version == 1) {
            v1.Header_(composer);
        } else {
            v0.Header_(composer);
        }
        composer.get(header);

        const_field<verify>(pad, 0);
        const_field<verify>(pre_defined, 0);
    }

    MediaHeaderBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        MediaHeaderBox_(composer);
    }

    MediaHeaderBox(Parser &parser): FullBox(parser, Flags(0)) {
        MediaHeaderBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_MEDIA_HEADER_BOX__

