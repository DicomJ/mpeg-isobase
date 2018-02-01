#ifndef __MPEG_ISOBASE_MOVIE_HEADER_BOX__
#define __MPEG_ISOBASE_MOVIE_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct MovieHeaderBox: FullBox {

    static constexpr Type container_type = 'moov';
    static constexpr Type box_type = 'mvhd';
    static constexpr const char *box_name = "MovieHeaderBox";

    template <FullBox::Version version, int w = (version == 1 ? 64 : 32)>
    struct Header {
        union {
            Footprint<3 * w + 32> header;
            UInt<w, 0>      creation_time;
            UInt<w, 1*w>    modification_time;
            UInt32< 2*w>    timescale;
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

    union { Footprint<2 * 16 + (1 + 2 + 9 + 6 + 1) * 32> header;
         Int32<  0>           rate;                     // = 0x00010000; // typically 1.0
         Int16< 32>           volume;                   // = 0x0100; // typically, full volume
        UInt16< 48>           reserved;                 // = 0;
        UInt32< 64>::Static::Array<2> reserved2;        // = 0;
         Int32<128>::Static::Array<9> matrix;           // = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 }; // Unity matrix
        UInt32<416>::Static::Array<6> pre_defined;      // = 0;
        UInt32<608>           next_track_ID;
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
    void MovieHeaderBox_(Composer &composer) {
        if (version == 1) {
            v1.Header_(composer);
        } else {
            v0.Header_(composer);
        }
        composer.get(header);

        const_field<verify>(reserved,       0);
        const_field<verify>(reserved2,      {0, 0});
        const_field<verify>(matrix,         {0x00010000, 0, 0, 0, 0x00010000, 0, 0, 0, 0x40000000});
        const_field<verify>(pre_defined,    {0, 0, 0, 0, 0, 0});
    }

    MovieHeaderBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        MovieHeaderBox_(composer);
    }

    MovieHeaderBox(Parser &parser): FullBox(parser, Flags(0)) {
        MovieHeaderBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_MOVIE_HEADER_BOX__

