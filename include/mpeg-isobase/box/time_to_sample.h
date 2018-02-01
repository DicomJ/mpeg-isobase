#ifndef __MPEG_ISOBASE_TIME_TO_SAMPLE_BOX__
#define __MPEG_ISOBASE_TIME_TO_SAMPLE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct TimeToSampleBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'stts';
    static constexpr const char *box_name = "TimeToSampleBox";

    UInt32<0> entry_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<TimeToSampleBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<TimeToSampleBox, Entry>;
    };

    TimeToSampleBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    TimeToSampleBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(this->entry_count);
        //assert(entry_count > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct TimeToSampleBox::Entry : Box::Entry {
    union { Footprint<2 * 32> header;
        UInt32< 0> sample_count;
        UInt32<32> sample_delta;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const TimeToSampleBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TIME_TO_SAMPLE_BOX__

