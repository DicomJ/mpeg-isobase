#ifndef __MPEG_ISOBASE_COMPOSITION_TO_DECODE_BOX__
#define __MPEG_ISOBASE_COMPOSITION_TO_DECODE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct CompositionToDecodeBox: FullBox {

    static constexpr Type container_type[] = {'stbl', 'trep'};
    static constexpr Type box_type = 'trep';
    static constexpr const char *box_name = "CompositionToDecodeBox";

    template <FullBox::Version version, int w = (version == 0 ? 32 : 64)>
    struct Header {

        union { Footprint<5 * w> header;
            UInt<w,  0*w> compositionToDTSShift;
            UInt<w,  1*w> leastDecodeToDisplayDelta;
            UInt<w,  2*w> greatestDecodeToDisplayDelta;
            UInt<w,  3*w> compositionStartTime;
            UInt<w,  4*w> compositionEndTime;
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

    uint64_t compositionToDTSShift() const          { return version == 0 ? uint64_t(v0.compositionToDTSShift):          uint64_t(v1.compositionToDTSShift);         }
    uint64_t leastDecodeToDisplayDelta() const      { return version == 0 ? uint64_t(v0.leastDecodeToDisplayDelta):      uint64_t(v1.leastDecodeToDisplayDelta);     }
    uint64_t greatestDecodeToDisplayDelta() const   { return version == 0 ? uint64_t(v0.greatestDecodeToDisplayDelta):   uint64_t(v1.greatestDecodeToDisplayDelta);  }
    uint64_t compositionStartTime() const           { return version == 0 ? uint64_t(v0.compositionStartTime):           uint64_t(v1.compositionStartTime);          }
    uint64_t compositionEndTime() const             { return version == 0 ? uint64_t(v0.compositionEndTime):             uint64_t(v1.compositionEndTime);            }

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void CompositionToDecodeBox_(Composer &composer) {
        if (version == 1) {
            v1.Header_(composer);
        } else {
            v0.Header_(composer);
        }
    }

    CompositionToDecodeBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        CompositionToDecodeBox_(composer);
    }

    CompositionToDecodeBox(Parser &parser): FullBox(parser, Flags(0)) {
        CompositionToDecodeBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_COMPOSITION_TO_DECODE_BOX__

