#ifndef __MPEG_ISOBASE_HINT_MEDIA_HEADER_BOX__
#define __MPEG_ISOBASE_HINT_MEDIA_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct HintMediaHeaderBox: FullBox {

    static constexpr Type container_type = 'minf';
    static constexpr Type box_type = 'hmhd';
    static constexpr const char *box_name = "HintMediaHeaderBox";

    union { Footprint<2 * 16 + 3 * 32> header;
        UInt16< 0> maxPDUsize;
        UInt16<16> avgPDUsize;
        UInt32<32> maxbitrate;
        UInt32<64> avgbitrate;
        UInt32<96> reserved;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void HintMediaHeaderBox_(Composer &composer) {
        composer.get(header);

        const_field<verify>(reserved, 0);
    }

    HintMediaHeaderBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        HintMediaHeaderBox_(composer);
    }

    HintMediaHeaderBox(Parser &parser): FullBox(parser, Flags(0)) {
        HintMediaHeaderBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_HINT_MEDIA_HEADER_BOX__

