#ifndef __MPEG_ISOBASE_SHADOW_SYNC_SAMPLE_BOX__
#define __MPEG_ISOBASE_SHADOW_SYNC_SAMPLE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ShadowSyncSampleBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'stsh';
    static constexpr const char *box_name = "ShadowSyncSampleBox";

    UInt32<0> entry_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<ShadowSyncSampleBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<ShadowSyncSampleBox, Entry>;
    };

    ShadowSyncSampleBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    ShadowSyncSampleBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(entry_count);
        //assert(entry_count > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct ShadowSyncSampleBox::Entry : Box::Entry {
    union { Footprint<2 * 32> header;
        UInt32< 0> shadowed_sample_number;
        UInt32<32> sync_sample_number;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const ShadowSyncSampleBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SHADOW_SYNC_SAMPLE_BOX__

