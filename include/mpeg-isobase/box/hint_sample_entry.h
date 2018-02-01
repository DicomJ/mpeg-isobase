#ifndef __MPEG_ISOBASE_HINT_SAMPLE_ENTRY__
#define __MPEG_ISOBASE_HINT_SAMPLE_ENTRY__

#include <mpeg-isobase/box/sample_entry.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct HintSampleEntry: SampleEntry {

    static constexpr Type box_type = 'hint';
    static constexpr const char *box_name = "HintSampleEntry";

    UInt8<>::Array data;

    HintSampleEntry(Composer &composer, long data_size, bool largesize = false): SampleEntry(composer, box_type, largesize) {
        composer.get(data, data_size);
    }

    HintSampleEntry(Parser &parser): SampleEntry(parser) {
        parser.get(*this, data);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_HINT_SAMPLE_ENTRY__

