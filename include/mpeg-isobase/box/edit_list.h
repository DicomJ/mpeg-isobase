#ifndef __MPEG_ISOBASE_EDIT_LIST_BOX__
#define __MPEG_ISOBASE_EDIT_LIST_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct EditListBox: FullBox {

    static constexpr Type container_type = 'edts';
    static constexpr Type box_type = 'elst';
    static constexpr const char *box_name = "EditListBox";

    UInt32<0> entry_count;

    template <FullBox::Version version, int w = version == 1? 64: 32>
    struct Entry;


    struct V0 {
        using Entry = EditListBox::Entry<0>;
        struct Parsed {
            using Entries = Box::Parsed::Entries<EditListBox, Entry>;
        };
        struct Composed {
            using Entries = Box::Composed::Entries<EditListBox, Entry>;
        };
    };

    struct V1 {
        using Entry = EditListBox::Entry<1>;
        struct Parsed {
            using Entries = Box::Parsed::Entries<EditListBox, Entry>;
        };
        struct Composed {
            using Entries = Box::Composed::Entries<EditListBox, Entry>;
        };
    };

    EditListBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    EditListBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(entry_count);
        //assert(entry_count > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

template <FullBox::Version version, int w>
struct EditListBox::Entry : Box::Entry {
    union { Footprint<2*w+2*16> header;
        UInt<w, 0>      segment_duration;
        UInt<w, w>      media_time;
        UInt16<2*w>     media_rate_integer;
        UInt16<2*w+16>  media_rate_fraction;
        UInt32<2*w>     media_rate; // 16.16 fixed-point
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    Entry(const EditListBox&, Composer &composer) {
        composer.get(header);
        const_field<verify>(media_rate_fraction, 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_EDIT_LIST_BOX__

