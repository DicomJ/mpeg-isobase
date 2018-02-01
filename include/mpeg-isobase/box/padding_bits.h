#ifndef __MPEG_ISOBASE_PADDING_BITS_BOX__
#define __MPEG_ISOBASE_PADDING_BITS_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct PaddingBitsBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'padb';
    static constexpr const char *box_name = "PaddingBitsBox";

    UInt32<> sample_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<PaddingBitsBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<PaddingBitsBox, Entry>;
    };

    PaddingBitsBox(Composer &composer, uint32_t sample_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->sample_count) = sample_count;
    }

    PaddingBitsBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(sample_count);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct PaddingBitsBox::Entry : Box::Entry {
    union { Footprint<1+3+1+3> header;
        UInt<1, 0> reserved1;
        UInt<3, 1> pad1;
        UInt<1, 4> reserved2;
        UInt<3, 5> pad2;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    Entry(const PaddingBitsBox &, Composer &composer) {
        composer.get(header);

        const_field<verify>(reserved1, 0);
        const_field<verify>(reserved2, 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_PADDING_BITS_BOX__

