#ifndef __MPEG_ISOBASE_ORIGINAL_FORMAT_BOX__
#define __MPEG_ISOBASE_ORIGINAL_FORMAT_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct OriginalFormatBox: Box {

    static constexpr Type container_type = 'ipro';
    static constexpr Type box_type = 'frma';
    static constexpr const char *box_name = "OriginalFormatBox";

    UInt32<> data_format;

    OriginalFormatBox(Composer &composer, uint32_t codingname): Box(composer, box_type) {
        composer.get(data_format);
        data_format = codingname;
    }

    OriginalFormatBox(Parser &parser): Box(parser) {
        parser.get(data_format);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_ORIGINAL_FORMAT_BOX__

