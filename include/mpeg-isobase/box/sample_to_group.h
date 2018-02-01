#ifndef __MPEG_ISOBASE_SAMPLE_TO_GROUP_BOX__
#define __MPEG_ISOBASE_SAMPLE_TO_GROUP_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SampleToGroupBox: FullBox {

    static constexpr Type container_type[] = {'stbl', 'traf'};
    static constexpr Type box_type = 'sbgp';
    static constexpr const char *box_name = "SampleToGroupBox";

    UInt32<> grouping_type;
    UInt32<> grouping_type_parameter;   // version == 1
    UInt32<> entry_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<SampleToGroupBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<SampleToGroupBox, Entry>;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void SampleToGroupBox_(Composer &composer) {
        composer.get(grouping_type);

        if (version == 1) {
            composer.get(grouping_type_parameter);
        }
        composer.get(entry_count);
    }

    SampleToGroupBox(Composer &composer, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        SampleToGroupBox_(composer);
    }

    SampleToGroupBox(Parser &parser): FullBox(parser, Flags(0)) {
        SampleToGroupBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct SampleToGroupBox::Entry : Box::Entry {
    union { Footprint<2*32> header;
        UInt32<0> sample_count;
        UInt32<32> group_description_index;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const SampleToGroupBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SAMPLE_TO_GROUP_BOX__

