#ifndef __MPEG_ISOBASE_SAMPLE_DESCRIPTION_BOX__
#define __MPEG_ISOBASE_SAMPLE_DESCRIPTION_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SampleDescriptionBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'stsd';
    static constexpr const char *box_name = "SampleDescriptionBox";

    UInt32<0> entry_count;

    SampleDescriptionBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    SampleDescriptionBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(entry_count);
        assert(entry_count > 0);
    }

    virtual void parse_payload(Remainder left);
    virtual void output_fields(bitstream::output::meta::field::Stream &stream_) const;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SAMPLE_DESCRIPTION_BOX__

