#ifndef __MPEG_ISOBASE_TRACK_RUN_BOX__
#define __MPEG_ISOBASE_TRACK_RUN_BOX__

#include <bitstream/signedness.h>
#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct TrackRunBox: FullBox {

    static constexpr Type container_type = 'traf';
    static constexpr Type box_type = 'trun';
    static constexpr const char *box_name = "TrackRunBox";

    UInt32<> sample_count;
    // the following are optional fields
    UInt32<> data_offset;           // flags & 0x000001
    UInt32<> first_sample_flags;    // flags & 0x000004

    template <FullBox::Version version>
    struct Entry;

    struct V0 {
        using Entry = TrackRunBox::Entry<0>;
        struct Parsed {
            using Entries = Box::Parsed::Entries<TrackRunBox, Entry>;
        };
        struct Composed {
            using Entries = Box::Composed::Entries<TrackRunBox, Entry>;
        };
    };

    struct V1 {
        using Entry = TrackRunBox::Entry<1>;
        struct Parsed {
            using Entries = Box::Parsed::Entries<TrackRunBox, Entry>;
        };
        struct Composed {
            using Entries = Box::Composed::Entries<TrackRunBox, Entry>;
        };
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void TrackRunBox_(Composer &composer) {
        composer.get(sample_count);

        if (flags & 0x000001) {
            composer.get(data_offset);
        }
        if (flags & 0x000004) {
            composer.get(first_sample_flags);
        }
    }

    TrackRunBox(Composer &composer, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, flags, largesize) {
        TrackRunBox_(composer);
    }

    TrackRunBox(Parser &parser): FullBox(parser) {
        TrackRunBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

template <FullBox::Version version_>
struct TrackRunBox::Entry : Box::Entry {

    UInt32< 0> sample_duration;
    UInt32<32> sample_size;
    UInt32<64> sample_flags;
    Field<32, 96,  typename signedness::template FromBoolTo<version_ != 0>::type> sample_composition_time_offset;

    const TrackRunBox &box;

    template <typename Composer>
    Entry(const TrackRunBox &box, Composer &composer): box(box) {
        if (box.flags & 0x000100) {
            composer.get(sample_duration);
        }
        if (box.flags & 0x000200) {
            composer.get(sample_size);
        }
        if (box.flags & 0x000400) {
            composer.get(sample_flags);
        }
        if (box.flags & 0x000800) {
            composer.get(sample_composition_time_offset);
        }
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TRACK_RUN_BOX__

