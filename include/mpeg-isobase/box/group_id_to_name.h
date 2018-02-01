#ifndef __MPEG_ISOBASE_GROUP_ID_TO_NAME_BOX__
#define __MPEG_ISOBASE_GROUP_ID_TO_NAME_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct GroupIdToNameBox: FullBox {

    static constexpr Type container_type = 'fiin';
    static constexpr Type box_type = 'gitn';
    static constexpr const char *box_name = "GroupIdToNameBox";

    UInt16<0> entry_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<GroupIdToNameBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<GroupIdToNameBox, Entry>;
    };

    GroupIdToNameBox(Composer &composer, uint16_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    GroupIdToNameBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(this->entry_count);
        //assert(entry_count > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct GroupIdToNameBox::Entry : Box::Entry {
    UInt32< 0> group_ID;
    String group_name;

    Entry(const Entry &entry): Box::Entry(entry) {}

    template <typename Composer>
    Entry(const GroupIdToNameBox &, Composer &composer) {
        composer.get(group_ID);
        composer.get(group_name);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_GROUP_ID_TO_NAME_BOX__

