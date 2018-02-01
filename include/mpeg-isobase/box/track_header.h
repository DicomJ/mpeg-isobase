#ifndef __MPEG_ISOBASE_TRACK_HEADER_BOX__
#define __MPEG_ISOBASE_TRACK_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct TrackHeaderBox: FullBox {

    static constexpr Type container_type = 'trak';
    static constexpr Type box_type = 'tkhd';
    static constexpr const char *box_name = "TrackHeaderBox";

    template <FullBox::Version version, int w = (version == 1 ? 64 : 32)>
    struct Header {
        union { Footprint<3 * w + 2 * 32> header;
            UInt<w,  0>         creation_time;
            UInt<w, w>          modification_time;
            UInt32<2*w>         track_ID;
            UInt32<2*w+32>      reserved;           // = 0;
            UInt<w, 2*w+2*32>   duration;
        };
        Header() {}

        template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
        void Header_(Composer &composer) {
            composer.get(header);
            const_field<verify>(reserved, 0);
        }

        void output(bitstream::output::meta::field::Stream &stream_) const;
    };
    Header<0> v0;
    Header<1> v1;

    union { Footprint<2 * 32 + 4 * 16 + (9 + 2) * 32> header;
        UInt32<  0>::Static::Array<2>   reserved2;          // = 0;
         Int16< 64>                     layer;              // = 0;
         Int16< 80>                     alternate_group;    // = 0;
         Int16< 96>                     volume;             // = {if track_is_audio 0x0100 else 0};
        UInt16<112>                     reserved3;          // = 0;
         Int32<128>::Static::Array<9>   matrix;             // = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 }; // Unity matrix
        UInt32<416>                     width;
        UInt32<448>                     height;
    };

    uint64_t creation_time() const      { return version == 1 ? uint64_t(v1.creation_time):     uint64_t(v0.creation_time);     }
    uint64_t modification_time() const  { return version == 1 ? uint64_t(v1.modification_time): uint64_t(v0.modification_time); }
    uint32_t reserved() const           { return version == 1 ? uint32_t(v1.reserved):          uint32_t(v0.reserved);          }
    uint32_t track_ID() const           { return version == 1 ? uint32_t(v1.track_ID):          uint32_t(v0.track_ID);          }
    uint64_t duration() const           { return version == 1 ? uint64_t(v1.duration):          uint64_t(v0.duration);          }

    void creation_time(uint64_t creation_time)          { if (version == 1) v1.creation_time = creation_time;           else  v0.creation_time = creation_time; }
    void modification_time(uint64_t modification_time)  { if (version == 1) v1.modification_time = modification_time;   else  v0.modification_time = modification_time; }
    void track_ID(uint32_t track_ID)                    { if (version == 1) v1.track_ID = track_ID;                     else  v0.track_ID = track_ID; }
    void duration(uint64_t duration)                    { if (version == 1) v1.duration = duration;                     else  v0.duration = duration; }

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void TrackHeaderBox_(Composer &composer) {
        if (version == 1) {
            v1.Header_(composer);
        } else {
            v0.Header_(composer);
        }
        composer.get(header);

        const_field<verify>(reserved2, {0, 0});
        const_field<verify>(layer, 0);
        const_field<verify>(alternate_group, 0);
        //const_field<verify>(volume, 0 || volume == 0x100);    // TODO
        const_field<verify>(reserved3, 0);
        const_field<verify>(matrix, {0x00010000, 0, 0, 0, 0x00010000, 0, 0, 0, 0x40000000});
    }

    TrackHeaderBox(Composer &composer, Version version = 0, Flags flags = 0, bool largesize = false): FullBox(composer, box_type, version, flags, largesize) {
        TrackHeaderBox_(composer);
    }

    TrackHeaderBox(Parser &parser): FullBox(parser) {
        TrackHeaderBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TRACK_HEADER_BOX__

