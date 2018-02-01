#ifndef __MPEG_ISOBASE_FD_SESSION_GROUP_BOX__
#define __MPEG_ISOBASE_FD_SESSION_GROUP_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct FDSessionGroupBox: Box {

    static constexpr Type box_type = 'segr';
    static constexpr const char *box_name = "FDSessionGroupBox";

    UInt16<0> num_session_groups;

    struct Group;
    struct Channel;
    struct Parsed {
        using Groups = Box::Parsed::Entries<FDSessionGroupBox, Group>;
        using Channels = Box::Parsed::Entries<FDSessionGroupBox, Channel>;
    };
    struct Composed {
        using Groups = Box::Composed::Entries<FDSessionGroupBox, Group>;
        using Channels = Box::Composed::Entries<FDSessionGroupBox, Channel>;
    };

    FDSessionGroupBox(Composer &composer, uint32_t num_session_groups, bool largesize = false): Box(composer, box_type, largesize) {
        composer.get(this->num_session_groups) = num_session_groups;
    }

    FDSessionGroupBox(Parser &parser): Box(parser) {
        parser.get(num_session_groups);
        //assert(num_session_groups > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};


struct FDSessionGroupBox::Group : Box::Entry {
    UInt8<> entry_count;

    virtual const char *name() const { return "Group"; }

    Group(const Group &) = default;

    template <typename Composer>
    Group(const FDSessionGroupBox &, Composer &composer) {
        composer.get(entry_count);
    }

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<Group, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<Group, Entry>;
    };

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};

struct FDSessionGroupBox::Group::Entry : Box::Entry {
    UInt32<> group_ID;

    Entry(const Entry &) = default;

    template <typename Composer>
    Entry(const Group &, Composer &composer) {
        composer.get(group_ID);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};

struct FDSessionGroupBox::Channel : Box::Entry {
    UInt8<> num_channels_in_session_group;

    virtual const char *name() const { return "Channel"; }

    Channel(const Channel &) = default;

    template <typename Composer>
    Channel(const FDSessionGroupBox &, Composer &composer) {
        composer.get(num_channels_in_session_group);
    }

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<Channel, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<Channel, Entry>;
    };

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};

struct FDSessionGroupBox::Channel::Entry : Box::Entry {
    UInt32<> hint_track_id;

    Entry(const Entry &) = default;

    template <typename Composer>
    Entry(const Channel &, Composer &composer) {
        composer.get(hint_track_id);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_FD_SESSION_GROUP_BOX__

