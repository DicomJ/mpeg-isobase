#ifndef __MPEG_ISOBASE_SAMPLE_ENTRY__
#define __MPEG_ISOBASE_SAMPLE_ENTRY__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SampleEntry: Box {

    static constexpr Type container_type = 'stsd';
    static constexpr const char *box_name = "SampleEntry";

    union { Footprint<8 * 6 + 16> header;
        UInt8 < 0>::Static::Array<6>    reserved;             // = 0
        UInt16<48>                      data_reference_index;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void SampleEntry_(Composer &composer) {
        composer.get(header);
        const_field<verify>(reserved, {0, 0, 0, 0, 0, 0});
    }

    inline SampleEntry(Composer &composer, uint32_t format, bool largesize = false): Box(composer, format, largesize) {
        SampleEntry_(composer);
    }

    inline SampleEntry(Parser &parser): Box(parser) {
        SampleEntry_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SAMPLE_ENTRY__

