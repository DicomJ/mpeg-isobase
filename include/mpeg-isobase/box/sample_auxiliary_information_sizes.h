#ifndef __MPEG_ISOBASE_SAMPLE_AUXILIARY_INFORMATION_SIZES_BOX__
#define __MPEG_ISOBASE_SAMPLE_AUXILIARY_INFORMATION_SIZES_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SampleAuxiliaryInformationSizesBox: FullBox {

    static constexpr Type container_type[] = {'stbl', 'traf'};
    static constexpr Type box_type = 'saiz';
    static constexpr const char *box_name = "SampleAuxiliaryInformationSizesBox";

    union { Footprint<2 * 32> aux_header;
        UInt32< 0> aux_info_type;
        UInt32<32> aux_info_type_parameter;
    };

    union { Footprint<8 + 32> header;
        UInt8 < 0> default_sample_info_size;
        UInt32<32> sample_count;
    };
    UInt8<>::Array sample_info_size;


    template <typename Composer>
    void SampleAuxiliaryInformationSizesBox_(Composer &composer) {
        if (flags & 1) {
            composer.get(aux_header);
        }
        composer.get(header);
    }

    template <typename SampleInfoSize = UInt8<>::Array::Vector>
    SampleAuxiliaryInformationSizesBox(Composer &composer, const SampleInfoSize sample_info_size = SampleInfoSize(), Flags flags = 0, bool largesize = false): FullBox(composer, box_type, Version(0), flags, largesize) {
        SampleAuxiliaryInformationSizesBox_(composer);
        if (sample_info_size.size() != 0) {
            default_sample_info_size = 0;
            composer.get(this->sample_info_size, sample_info_size);
        }
    }

    SampleAuxiliaryInformationSizesBox(Parser &parser): FullBox(parser, Version(0)) {
        SampleAuxiliaryInformationSizesBox_(parser);
        if (default_sample_info_size == 0) {
            parser.get(*this, sample_info_size);
            //assert(parser.box_payload_size(*this) == sample_count);
        }
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SAMPLE_AUXILIARY_INFORMATION_SIZES_BOX__

