#ifndef __MPEG_ISOBASE_COMPOSITION_OFFSET_BOX__
#define __MPEG_ISOBASE_COMPOSITION_OFFSET_BOX__

#include <bitstream/signedness.h>
#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct CompositionOffsetBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'ctts';
    static constexpr const char *box_name = "CompositionOffsetBox";

    UInt32<0> entry_count;

    template <FullBox::Version version>
    struct Entry;

    struct V0 {
        using Entry = CompositionOffsetBox::Entry<0>;
        struct Parsed {
            using Entries = Box::Parsed::Entries<CompositionOffsetBox, Entry>;
        };
        struct Composed {
            using Entries = Box::Composed::Entries<CompositionOffsetBox, Entry>;
        };
    };

    struct V1 {
        using Entry = CompositionOffsetBox::Entry<1>;
        struct Parsed {
            using Entries = Box::Parsed::Entries<CompositionOffsetBox, Entry>;
        };
        struct Composed {
            using Entries = Box::Composed::Entries<CompositionOffsetBox, Entry>;
        };
    };

    CompositionOffsetBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    CompositionOffsetBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(entry_count);
        //assert(entry_count > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

template <FullBox::Version version_>
struct CompositionOffsetBox::Entry: Box::Entry {
    union { Footprint<2 * 32> header;
        UInt32< 0> sample_count;
        Field<32, 32,  typename signedness::template FromBoolTo<version_ == 1>::type> sample_offset;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const CompositionOffsetBox&, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_COMPOSITION_OFFSET_BOX__

