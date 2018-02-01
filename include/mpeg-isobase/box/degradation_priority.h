#ifndef __MPEG_ISOBASE_DEGRADATION_PRIORITY_BOX__
#define __MPEG_ISOBASE_DEGRADATION_PRIORITY_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct DegradationPriorityBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'stdp';
    static constexpr const char *box_name = "DegradationPriorityBox";

    uint32_t entry_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<DegradationPriorityBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<DegradationPriorityBox, Entry>;
    };

    DegradationPriorityBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize), entry_count(entry_count) {
    }

    DegradationPriorityBox(Parser &parser): FullBox(parser, Flags(0)) {
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct DegradationPriorityBox::Entry : Box::Entry {
    union { Footprint<16> header;
        UInt16<0> priority;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const DegradationPriorityBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_DEGRADATION_PRIORITY_BOX__

