#ifndef __MPEG_ISOBASE_SAMPLE_SCALE_BOX__
#define __MPEG_ISOBASE_SAMPLE_SCALE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SampleScaleBox: FullBox {

    //static constexpr Type container_type = '';
    static constexpr Type box_type = 'stsl';
    static constexpr const char *box_name = "SampleScaleBox";

    union { Footprint<32+ 26 + 3 * 2 + 32> header;
        UInt<7, 0> reserved;  // = 0;
        UInt<1, 7> constraint_flag;
        UInt8<  8> scale_method;
        UInt16<16> display_center_x;
        UInt16<32> display_center_y;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void SampleScaleBox_(Composer &composer) {
        composer.get(header);
        const_field<verify>(reserved, 0);
    }

    SampleScaleBox(Composer &composer, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        SampleScaleBox_(composer);
    }

    SampleScaleBox(Parser &parser): FullBox(parser, Flags(0)) {
        SampleScaleBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SAMPLE_SCALE_BOX__

