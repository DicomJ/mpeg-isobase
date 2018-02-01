#ifndef __MPEG_ISOBASE_CHUNK_LARGE_OFFSET_BOX__
#define __MPEG_ISOBASE_CHUNK_LARGE_OFFSET_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ChunkLargeOffsetBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'co64';
    static constexpr const char *box_name = "ChunkLargeOffsetBox";

    UInt32<0> entry_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<ChunkLargeOffsetBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<ChunkLargeOffsetBox, Entry>;
    };

    ChunkLargeOffsetBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    ChunkLargeOffsetBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(entry_count);
        //assert(entry_count > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};


struct ChunkLargeOffsetBox::Entry : Box::Entry {
    union { Footprint<64> header;
        UInt64<0> chunk_offset;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const ChunkLargeOffsetBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_CHUNK_LARGE_OFFSET_BOX__

