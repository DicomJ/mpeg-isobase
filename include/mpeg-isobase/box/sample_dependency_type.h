#ifndef __MPEG_ISOBASE_SAMPLE_DEPENDENCY_TYPE_BOX__
#define __MPEG_ISOBASE_SAMPLE_DEPENDENCY_TYPE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SampleDependencyTypeBox: FullBox {

    static constexpr Type container_type[] = {'stbl', 'traf'};
    static constexpr Type box_type = 'sdtp';
    static constexpr const char *box_name = "SampleDependencyTypeBox";

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<SampleDependencyTypeBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<SampleDependencyTypeBox, Entry>;
    };

    //sample_count  is taken from the sample_count in the Sample Size Box ('stsz') or Compact Sample Size Box (‘stz2’).
    inline uint32_t entries() const;

    SampleDependencyTypeBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0)) {
    }

    SampleDependencyTypeBox(Parser &parser): FullBox(parser, Flags(0)) {
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};


struct SampleDependencyTypeBox::Entry : Box::Entry {
    union { Footprint<8> header;
        UInt<2, 0> is_leading;
        UInt<2, 2> sample_depends_on;
        UInt<2, 4> sample_is_depended_on;
        UInt<2, 6> sample_has_redundancy;
        UInt8<> entire_mask;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const SampleDependencyTypeBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};

inline uint32_t SampleDependencyTypeBox::entries() const {
    return parser().box_payload_size(*this) / (decltype(Entry::header)::size / 8);    // to the end of box
}


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SAMPLE_DEPENDENCY_TYPE_BOX__

