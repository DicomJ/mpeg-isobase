#ifndef __MPEG_ISOBASE_LEVEL_ASSIGNMENT_BOX__
#define __MPEG_ISOBASE_LEVEL_ASSIGNMENT_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct LevelAssignmentBox: FullBox {

    static constexpr Type container_type = 'mvex';
    static constexpr Type box_type = 'leva';
    static constexpr const char *box_name = "LevelAssignmentBox";

    UInt8<> level_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<LevelAssignmentBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<LevelAssignmentBox, Entry>;
    };

    LevelAssignmentBox(Composer &composer, uint32_t level_count, bool largesize = false): FullBox(composer, box_type, Version(0), Flags(0), largesize) {
        composer.get(this->level_count) = level_count;
    }

    LevelAssignmentBox(Parser &parser): FullBox(parser, Version(0), Flags(0)) {
        parser.get(level_count);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct LevelAssignmentBox::Entry : Box::Entry {
    union { Footprint<32+1+7> header;
        UInt32<>    track_id;
        UInt<1, 32> padding_flag;
        UInt<7, 33> assignment_type;
    };

    union { Footprint<2*32> grouping_type_header;
        UInt32< 0> grouping_type;
        UInt32<32> grouping_type_parameter;
    };

    UInt32<> sub_track_id;

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const LevelAssignmentBox &, Composer &composer) {
        composer.get(header);
        if (assignment_type == 0) {
            composer.get(grouping_type);
        } else if (assignment_type == 1) {
            composer.get(grouping_type_header);
        } else if (assignment_type == 2) {  // no further syntax elements needed
        } else if (assignment_type == 3) {  // no further syntax elements needed
        } else if (assignment_type == 4) {
            composer.get(sub_track_id);
        }
        // other assignment_type values are reserved
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_LEVEL_ASSIGNMENT_BOX__

